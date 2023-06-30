from cs50 import get_string

text = get_string("Text: ")

letter = 0
sentence = 0

# Counting letter, words and sentences
word = len(text.split())

for i in range(len(text)):
    if text[i].isalpha():
        letter += 1
    elif text[i] == "." or text[i] == "!" or text[i] == "?":
        sentence += 1

# Calculating Coleman-Liau index
L = (letter / word) * 100
S = (sentence / word) * 100

index = 0.0588 * L - 0.296 * S - 15.8
grade = round(index)

# Printing results
if grade >= 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print(f"Grade {grade}")
