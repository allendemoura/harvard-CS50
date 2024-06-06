import csv
import sys

if len(sys.argv) != 3:
    sys.exit("Usage: python dna.py DATABASE TEXTFILE")

dnaData, strCounts = {}, {}
with open(sys.argv[1]) as file:
    reader = csv.DictReader(file)

    for row in reader:
        name = row.pop("name")
        dnaData[name] = { k: int(v) for (k, v) in row.items() }

    for item in reader.fieldnames[1:]:
        strCounts[item] = 0

with open(sys.argv[2], "r") as file:
    sequence = file.read()

for strPattern in strCounts:
    count = 0

    while (strPattern * (count + 1) in sequence):
        count += 1

    strCounts[strPattern] = count

for person in dnaData:
    if dnaData[person] == strCounts:
        print(person)
        sys.exit()

print("No match")