import sys
import re
from cs50 import get_string


def readinglevel(text):
    sentences = len(re.findall(r'[.?!]', text))
    words = len(re.findall(r'[ ]', text)) + 1
    letters = len(re.findall(r'[\w]', text))
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