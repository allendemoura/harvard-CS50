from cs50 import get_string


def main():
    card = get_string("Number: ")
    if chksum(card) == 0:
        if (len(card) == 13 or len(card) == 16) and card[0] == "4":
            print("VISA")
        elif len(card) == 15 and int(card[0:2]) in (34, 37):
            print("AMEX")
        elif len(card) == 16 and int(card[0:2]) in range(51, 56):
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


def chksum(card):
    chksum = 0
    count = 0
    for digit in reversed(card):
        # for every other digit starting from last
        if count % 2 == 0:
            chksum += int(digit)
            count += 1
        # if the product is 2 digits then separate them and sum
        elif int(digit) * 2 > 9:
            chksum += int(digit) * 2 // 10 + int(digit) * 2 % 10
            count += 1
        # for every other digit starting from second to last
        else:
            chksum += int(digit) * 2
            count += 1
    chksum %= 10
    return chksum


if __name__ == "__main__":
    main()