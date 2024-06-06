#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover file.raw\n");
        return 1;
    }
    // Open forensic image file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    // initializations
    int jcount = 0;
    char *filename = malloc(8);
    BYTE buffer[512];

    sprintf(filename, "%03i.jpg", jcount);
    FILE *img = fopen(filename, "w");

    while (fread(buffer, 1, 512, input) == 512)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (jcount == 0)
            {
                fwrite(buffer, 1, 512, img);
                jcount++;
            }
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", jcount);
                img = fopen(filename, "w");
                fwrite(buffer, 1, 512, img);
                jcount++;
            }
        }
        else if (jcount > 0)
        {
            fwrite(buffer, 1, 512, img);
        }
    }
    fclose(input);
    fclose(img);
    free(filename);
}