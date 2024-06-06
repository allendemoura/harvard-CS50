import csv
import sys

# Ensure correct usage
if len(sys.argv) != 3:
    sys.exit("Usage: python dna.py DATABASE TEXTFILE")

# Make dnaData dict with keys representing people names that contain dicts
# of a certain structure to mirror the strCount structure below
# Should look like
#
# dnaData = {
#     'Bob' : {
#         'SLDSK' : 99,
#         'SLDSK' : 99,
#         'SLDSK' : 99,
#     },
#     'Alice' : {
#         'SLDSK' : 99,
#         'ASDF' : 99,
#         'ZXCV' : 99,
#     }
# }

dnaData = {}
with open(sys.argv[1]) as file:
    reader = csv.DictReader(file)

    for row in reader:
        name = row["name"]
        dnaData[name] = {}
        dnaData[name] = row
        dnaData[name].pop("name", None)

        for item in dnaData[name]:
            dnaData[name][item] = int(dnaData[name][item])

# Make an strPattern object that mirrors the data structure of each individual person
# Should look like
#
# strCounts = {
#     'SLDSK' : 99,
#     'ASDF' : 99,
#     'ZXCV' : 99,
# }

strCounts = {}
with open(sys.argv[1]) as file:
    reader = list(csv.reader(file))

    for key in reader[0]:
        strCounts[key] = 0

    strCounts.pop("name", None)

# Read DNA sequence file into a variable
with open(sys.argv[2], "r") as file:
    sequence = file.read()

# Find longest match of each STR in DNA sequence
for strPattern in strCounts:
    strLen = len(strPattern)

    for i in range(len(sequence)):
        count = 0

        while sequence[i: i + (strLen * (count + 1))] == strPattern * (count + 1):
            count += 1

        strCounts[strPattern] = max(count, strCounts[strPattern])

# Check database for matching profiles
for person in dnaData:
    # Because we did that work above to make the data structures match, we can do this
    if dnaData[person] == strCounts:
        print(person)
        sys.exit()

print("No match")