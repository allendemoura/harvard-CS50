import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py databaseFilename.csv sequencefilename.txt")

    # TODO: Read database file into a variable
    with open(sys.argv[1], "r") as dbfile:
        buffer = csv.DictReader(dbfile)
        # read into database list of dicts
        db = list(buffer)
        # get keys from the file header (first row)
        subsequences = list(buffer.fieldnames)
        # shear 'name' outta there
        subsequences.pop(0)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as seqfile:
        sequence = str(seqfile.read())

    # TODO: Find longest match of each STR in DNA sequence
    person = []
    for i in subsequences:
        person.append(longest_match(sequence, i))

    # TODO: Check database for matching profiles
    for i in db:
        # pull values from the database to compare for a match
        dbval = list(i.values())
        # shear their name for symmetry with the input list
        dbval.pop(0)
        # convert strings to integers (seemed easier than doing it above in the initialization)
        dbval = [int(i) for i in dbval]
        if person == dbval:
            print(i['name'])
            exit()
    # if we get thru this loop we've checked the whole db and theres no match
    else:
        print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # this seemed more 'pythonic'
    count = 0
    while subsequence * (count + 1) in sequence:
        count += 1
    return count


main()
