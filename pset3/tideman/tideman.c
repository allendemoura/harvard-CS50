#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
// sets size to total number of possible matchups between all candidates using round robin formula
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool cycle(int winner, int loser);
void print_winner(void);
bool check_winner(int cand);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // loop thru candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // check name against candidates name
        if (strcmp(name, candidates[i]) == 0)
        {
            // update ranks array with index of candidate
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // loop over each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // loop over each subsequent ranking and record one vote of preference over them
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] ++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    // loop over each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // sub loop
        for (int j = 0; j < candidate_count; j++)
        {
            // check matchups
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count ++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{

    // selection sort
    for (int i = 0; i < pair_count; i++)
    {
        int max = preferences[pairs[i].winner][pairs[i].loser];
        // pointer variable for swapping idk how to explain this
        int mindex = i;
        // find highest value in unsorted portion
        for (int j = i + 1; j < pair_count; j++)
        {
            if (max < preferences[pairs[j].winner][pairs[j].loser])
            {
                max = preferences[pairs[j].winner][pairs[j].loser];
                mindex = j;
            }
        }
        // then swap first value of unsorted portion with highest value
        if (max > preferences[pairs[i].winner][pairs[i].loser])
        {
            pair temp = pairs[i];
            pairs[i] = pairs[mindex];
            pairs[mindex] = temp;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // loop thru pairs
    for (int i = 0; i < pair_count; i++)
    {
        // lock the pair if only cycle comes back false. skip the pair if cycle returns true which indicates a loop or cycle on the tideman graph
        if (!cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// conceptually, this function takes the source of the proposed arrow, checks if the destination of the arrow has any arrows pointing out already locked in.
// if it does, it follows that arrow until it dead ends (returns false) or ends up back at the source, indicating a cycle would be formed (returns true).
// its a mindfuck.
bool cycle(int winner, int loser)
{
    // if we've looped around to the beginning, cycle is detected and returns true in cascade to all the called instances of cycle on the stack (base case)
    if (winner == loser)
    {
        return true;
    }
    // loop thru candidates (recursive case)
    for (int i = 0; i < candidate_count; i++)
    {
        // check if the loser has already won against anyone in the locked array
        if (locked[loser][i])
        {
            // if they did, run it again
            if (cycle(winner, i))
            {
                return true;
            }
        }
    }
    // if loop finishes everything then there's no cycle
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (check_winner(i))
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}

bool check_winner(int cand)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][cand])
        {
            return false;
        }
    }
    return true;
}

// ALT VERSION THAT SHOULD WORK BUT CHECK50 IS FUCKED OFF AND PASSES IT SOMETIMES BUT NOT OTHERS
// void print_winner(void)
// {

//     for (int i = 0; i < candidate_count; i++)
//     {

//         for (int j = 0; j < candidate_count; j++)
//         {
//             if (locked[j][i])
//             {
//                 i++;
//                 j = -1;
//             }
//         }
//         printf("%s\n", candidates[i]);
//     }
//     return;
// } 