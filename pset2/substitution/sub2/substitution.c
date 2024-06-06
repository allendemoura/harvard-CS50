#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // check arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // check key length is 26 char
    string key = argv[1];
    int keylen = strlen(key);
    if (keylen != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // validate key more (per character)
    double chksum = 1;
    for (int i = 0; i < keylen; i++)
    {
        char c = key[i];
        // check for only letters
        if (!isalpha(c))
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }
        // convert to uppercase for chksum purposes
        if (islower(c))
        {
            c = toupper(c);
        }
        chksum *= (double) (c - 64);
    }
    // check if every letter in the alphabet is there only once
    if (chksum != 403291461126605635584000000.0)
    {
        printf("Your key should include every letter in the alphabet exactly once.\n");
        return 1;
    }
    //get user input
    string input = get_string("plaintext:  ");
    int len = strlen(input);
    string cipher = input;
    // cipher per character
    for (int i = 0; i < len; i++)
    {
        char p = input[i];
        // only sub letters
        if (isalpha(p))
        {
            // preserve case
            if (isupper(p))
            {
                p -= 65;
                cipher[i] = toupper(key[(int)p]);
            }
            if (islower(p))
            {
                p -= 97;
                cipher[i] = tolower(key[(int)p]);
            }
        }
    }
    printf("ciphertext: %s\n", cipher);
}
