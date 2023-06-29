# Reading size
while True:
    size_input = input("Height: ")
    try:
        size = int(size_input)
        if 1 <= size <= 8:
            break
    except ValueError:
        pass

# Printing pattern
for i in range(size):
    print(" " * (size - i - 1), end="")
    print("#" * (i + 1), end="  ")
    print("#" * (i + 1))
