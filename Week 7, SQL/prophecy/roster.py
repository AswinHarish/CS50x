from cs50 import SQL
import csv


# Collecting data for house table
def create_houses(house, houses, head):
    count = 0
    for h in houses:
        if h["house_name"] == house:
            count += 1
    if count == 0:
        houses.append({"house_name": house, "head": head})


# Data for students table
def create_students(name, students):
    students.append({"student_name": name})


# Data for assignments table
def assignments(name, house, assigns):
    assigns.append({"student_name": name, "house": house})


dataBase = SQL("sqlite:///roster.db")

# lists to store data in functions
students = []
houses = []
assigns = []

# Opening CSV file to read data
with open('students.csv', 'r') as file:
    reader = csv.DictReader(file)
    for row in reader:
        name = row["student_name"]
        house = row["house"]
        head = row["head"]

        create_houses(house, houses, head)
        create_students(name, students)
        assignments(name, house, assigns)


# Inserting data to tables
for student in students:
    dataBase.execute("INSERT INTO students (student_name) VALUES (?)", student["student_name"])

for house in houses:
    dataBase.execute("INSERT INTO houses (house_name, head) VALUES (?, ?)", house["house_name"], house["head"])

for assign in assigns:
    dataBase.execute("INSERT INTO assignments (student_id, house_id) VALUES ((SELECT id FROM students WHERE student_name = ?), (SELECT id FROM houses WHERE house_name = ?))",
                     assign["student_name"], assign["house"])
