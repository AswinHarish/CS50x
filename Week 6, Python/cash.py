from cs50 import get_float
from math import floor

# reading and validating user input
while True:
    cents = get_float("Change owed: ")
    if cents > 0:
        break

# converting dollars to cents
cents = round(cents * 100)

# calculating quarters
quarters = floor(cents/25)
cents = cents - quarters * 25

# calculating dimes
dimes = floor(cents/10)
cents = cents - dimes * 10

# calculating nickels
nickels = floor(cents/5)
cents = cents - nickels * 5

coins = cents + nickels + dimes + quarters
print(coins)
