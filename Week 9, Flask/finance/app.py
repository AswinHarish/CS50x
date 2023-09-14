import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Getting user's stocks and quantity
    stocks = db.execute(
        "SELECT symbol, SUM(quantity) as qty FROM transactions WHERE userid = ? GROUP BY  symbol HAVING qty > 0;",
        session.get("user_id"),
    )

    # Getting user's cash balance
    cash = db.execute("SELECT cash FROM users WHERE id = ?;", session.get("user_id"))[
        0
    ]["cash"]
    total = cash

    # Fetching market price and total value of user's holdings
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["value"] = stock["price"] * stock["qty"]
        total += stock["value"]

    return render_template("index.html", stocks=stocks, cash=cash, grand_total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # If user reached route via POST
    if request.method == "GET":
        return render_template("buy.html")

    # If user reached route via GET
    else:
        # Validating user input
        company = request.form.get("symbol").upper()
        quantity = request.form.get("shares")
        stock = lookup(company)
        if not company or stock is None:
            return apology("Invalid Stock Name")

        # validating the quantity entered by user
        try:
            if int(quantity) <= 0:
                return apology("Invalid Quantity")
        except ValueError:
            return apology("Cannot buy fractions of a stock")

        # Checking account balance
        balance = db.execute(
            "SELECT cash FROM users WHERE id = ?", session.get("user_id")
        )

        # Checking if user can afford no of shares at current market price
        if (stock["price"] * int(quantity)) > balance[0]["cash"]:
            return apology("Insufficient Balance")

        # Updating user's cash balance
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?;",
            (balance[0]["cash"] - (stock["price"] * int(quantity))),
            session.get("user_id"),
        )

        # Adding transaction to database
        db.execute(
            "INSERT INTO transactions (userid, symbol, quantity, price) VALUES (?,?,?,?);",
            session.get("user_id"),
            company,
            int(quantity),
            stock["price"],
        )
    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Taking transaction history from database
    history = db.execute(
        "SELECT symbol, quantity, price, timestamp FROM transactions WHERE userid = ?;",
        session.get("user_id"),
    )
    print(history)
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # If user reached route via GET
    if request.method == "GET":
        return render_template("quote.html")

    # If user reached route via POST
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Symbol can't be blank")

        stock = lookup(symbol)
        if stock == None:
            return apology("Invalid Symbol")

        return render_template("quoted.html", stock=stock)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # If user reached route via POST
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        passConf = request.form.get("confirmation")

        result = db.execute("SELECT * FROM users WHERE username = ?", username)

        # validating user inputs before storing in database
        if not username or len(result) != 0:
            return apology("Invalid Username!")
        elif not password:
            return apology("Password cannot be empty")
        elif not passConf or password != passConf:
            return apology("Passwords do not match")

        # Insert new user into database
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?,?);",
            username,
            generate_password_hash(password),
        )

        # Redirecting to login page for succesull registration
        return redirect("/login")

    # If user reached route via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Getting user data from database
    stocks = db.execute(
        "SELECT symbol, SUM(quantity) as qty FROM transactions WHERE userid = ? GROUP BY  symbol HAVING qty > 0;",
        session.get("user_id"),
    )

    # If user reached route via POST
    if request.method == "POST":
        # validating user entry
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Please fill out all fields")

        # Checking the entered stock is in user's portfolio or not
        stockSearch = None
        stockFound = False
        for stock in stocks:
            if stock["symbol"] == request.form.get("symbol"):
                stockFound = True
                stockSearch = stock
                break

        if not stockFound:
            return apology("Selected stock is not in your portfolio")

        # Checking the quantity of stocks user holds and verifying with how many they want to sell
        if int(request.form.get("shares")) <= 0:
            return apology("Invalid quantity")
        if int(request.form.get("shares")) > stockSearch["qty"]:
            return apology("You can only sell what you have")

        # Convertin the qty to negative and looking up current market price
        quantity = -1 * int(request.form.get("shares"))
        price = lookup(request.form.get("symbol"))["price"]

        # Removing the stock form user's portfolio
        db.execute(
            "INSERT INTO transactions (userid, symbol, quantity, price) VALUES (?,?,?,?);",
            session.get("user_id"),
            request.form.get("symbol"),
            quantity,
            price,
        )

        # Updating user's cash balance
        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?;",
            price * int(request.form.get("shares")),
            session.get("user_id"),
        )
        return redirect("/")

    # If user reached route via GET
    else:
        return render_template("sell.html", stocks=stocks)
