#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int readinglevel(string input);

int main(void)
{
    string input = get_string("Text: ");
    int x = readinglevel(input);
    if (x < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (x >= 1 && x <= 15)
    {
        printf("Grade %i\n", x);
    }
    else if (x >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        return 1;
    }
}

int readinglevel(string input)
{
    int lvl = 0;
    int sentences = 0;
    int words = 0;
    int letters = 0;
    int length = strlen(input);
    // scan thru each char of the input
    for (int i = 0; i < length; i++)
    {
        // letter counter (yes i couldve used isupper() and islower() but i cannot be bothered rn)
        if ((input[i] >= 65 && input[i] <= 90) || (input[i] >= 97 && input[i] <= 122))
        {
            letters ++;
        }
        // word counter
        else if (input[i] == 32)
        {
            words ++;
        }
        // sentence counter
        else if (input[i] == 46 || input[i] == 63 || input[i] == 33)
        {
            words ++;
            sentences ++;
            // skip spaces following punctuation
            while (input[i + 1] == 32)
            {
                i++;
            }
        }
    }
    // calculate letters per 100 words
    float l = (float)letters / (float)words * 100.0;
    // calculate sentences per 100 words
    float s = (float)sentences / (float)words * 100.0;
    // coleman-liau index
    lvl = round(0.0588 * l - 0.296 * s - 15.8);
    return lvl;
}