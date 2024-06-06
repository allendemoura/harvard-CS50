import sys
from cs50 import get_string


def readinglevel(text):
    lvl = 0
    sentences = 0
    words = 1
    letters = 0
    for c in text:
        if c.isalpha():
            letters += 1
        elif c.isspace():
            words += 1
        elif ord(c) in (46, 63, 33):
            sentences += 1
    l = letters / words * 100
    s = sentences / words * 100
    lvl = round(0.0588 * l - 0.296 * s - 15.8)
    return lvl


text = get_string("Text: ")
lvl = readinglevel(text)
if lvl < 1:
    print("Before Grade 1")
elif lvl in range(1, 16):
    print("Grade", lvl)
elif lvl >= 16:
    print("Grade 16+")
else:
    sys.exit(1)