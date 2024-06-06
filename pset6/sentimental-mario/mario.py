from cs50 import get_int

while True:
    height = get_int("Pyramid height from 1 to 8: ")
    if height >= 1 and height <= 8:
        break
for rows in range(height):
    print(" " * (height - rows - 1), end="")
    print("#" * (rows + 1), end="  ")
    print("#" * (rows + 1))