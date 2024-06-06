#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Prompt user for input
    int height;
    do
    {
        height = get_int("Pyramid height from 1 to 8: ");
    }
    while (height < 1 || height > 8);

    // Count rows
    for (int rowcount = 0; rowcount < height; rowcount++)
    {
        // Output spaces for left align
        for (int spacecount = 0; spacecount < (height - rowcount - 1); spacecount++)
        {
            printf(" ");
        }
        // Output pyramid block(s)
        for (int blockcount = 0; blockcount <= rowcount; blockcount++)
        {
            printf("#");
        }
        //Output break to next row
        printf("\n");
    }
}