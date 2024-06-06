// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

/* i chose this table size because its the closest prime to 1.25 * 143091 (large dict size) [which will leave the hash table 80% full (if every key is unique),
which was characterized as optimal by my research] */

const unsigned int N = 178859;

/* 196613; an alternate prime tested bc its > 143091 (large dict size) that is farthest from a power of 2 (my research showed me that
powers of 2 will result in more collisions due to symmetry with binary structure which is base 2). ultimately i think this was
based on using bitwise operations in the hash function which i elected not to do so its unnecessary to consider powers of 2*/

// Hash table
node *table[N];

// word count pointer to cheat the size function ty
int *words = NULL;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // marginal speed optimization to only call hash once
    unsigned int hashn = hash(word);

    // if hash address empty
    if (table[hashn] == NULL)
    {
        return false;
    }
    // if first element matches return true
    else if (strcasecmp(table[hashn]->word, word) == 0)
    {
        return true;
    }
    else
    {
        // linear search list at hash address
        for (node *tmp = table[hashn]->next; tmp != NULL; tmp = tmp->next)
        {
            if (strcasecmp(tmp->word, word) == 0)
            {
                return true;
            }
        }
        // if we reach end of the list then search fails to find and its not here!
        return false;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /* my hash function generates a key by multiplying ascii values of the word by a prime and summing them, then modulo N to prevent overflow.
    i had this general idea earlier in the course when calculating chksums (in the Credit assignment) which i now realize are very similar in
    concept and are basically a form of hash function (?). i added some ideas from research on hash functions in general and generic implementations.
    no copy/paste.

    i experimented with which prime to multiply by, and found that going up to 7 (my fav number) gave some small but appreciable improvement and got me
    practically identical times to the staff solution. going higher seemed to yield imperceptible and diminishing returns and choosing classic primes from
    popular algos such as 33 or 5381 (from djb2) seemed to give no improvement.

    i also experimented with initializing hash to these primes and saw no real effect, so left it to zero for the sake of original thought

    EDIT: after more rigourous testing i believe my hash may even be slightly faster than the staff solution, but i lose those hundreths of a second
    in my load function which i stubborly refuse to change. the my total times are practically identical to speller50, but if i were to cave and
    implement scanf as spoonfed by the video i would probably beat speller50. */

    unsigned int hash = 0;
    char c;

    while ((c = *word++)) /* found this clever loop implementation while researching order of operations for incrementing pointers in my size() cheat */
    {
        hash = hash * 7 + toupper(c);
    }
    hash %= N;
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dictionary that was specified
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        fclose(dict);
        return false;
    }

    // inits
    words = malloc(sizeof(int));
    *words = 0;
    int index = 0;
    char word[LENGTH + 1];

    /*************** this loop borrowed and adapted from speller.c distro *******************/
    /* later watched the walkthru and saw that they recommended scanf which is prob slightly faster,
    but honestly this felt more original than copying spoonfed code from the walkthru */

    // read dictionary one char at a time and add words to hash table
    char c;
    while (fread(&c, sizeof(char), 1, dict))
    {
        // build word
        if (c != '\n')
        {
            // Append character to word
            word[index] = c;
            index++;
        }
        // finished word
        else
        {
            // Terminate current word
            word[index] = '\0';

            // marginal speed optimization to only call hash once
            unsigned int hashn = hash(word);

            // prepend word to hash table
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }
            strcpy(n->word, word);
            n->next = table[hashn];
            table[hashn] = n;

            // Update counter
            (*words)++;

            // Prepare for next word
            index = 0;
        }
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    int size = *words;
    return size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    free(words);
    return true;
}
