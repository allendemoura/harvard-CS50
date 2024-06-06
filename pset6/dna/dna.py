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

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
