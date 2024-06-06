#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool only_digits(string s);
char rotate(char c, int key);

int main(int argc, string argv[])
{
    // check arguments
    if (argc != 2 || only_digits(argv[1]) == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // convert key string to int
    int key = atoi(argv[1]);
    // get input and determine length
    string input = get_string("plaintext:  ");
    int len = strlen(input);
    string cipher = input;
    // rotate one char at a time per key
    for (int i = 0; i < len; i++)
    {
        cipher[i] = rotate(input[i], key);
    }
    printf("ciphertext: %s\n", cipher);
    return 0;
}

// checker function that key is digits and not some trash
bool only_digits(string s)
{
    int len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        if (isdigit(s[i])) {}
        else
        {
            return false;
        }
    }
    return true;
}

// cipher function that works one char at a time
char rotate(char c, int key)
{
    if (islower(c))
    {
        c = ((c - 97 + key) % 26) + 97;
    }
    if (isupper(c))
    {
        c = ((c - 65 + key) % 26) + 65;
    }
    return c;
}