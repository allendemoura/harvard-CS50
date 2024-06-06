from re import I
from cs50 import get_float

while True:
    cents = get_float("Change owed: ") * 100
    if cents > 0: break
quarters = int(cents / 25)
cents %= 25
dimes = int(cents / 10)
cents %= 10
nickels = int(cents / 5)
cents %= 5
coins = quarters + dimes + nickels + cents
print(int(coins))

