#include "helpers.h"
#include <math.h>
#include <stdlib.h>

int imin(int n, int m);
int imax(int n, int m);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }

    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //store og values before computing filter so that it doesnt compound on itself
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = fmin(round(.393 * red + .769 * green + .189 * blue), 255);
            image[i][j].rgbtGreen = fmin(round(.349 * red + .686 * green + .168 * blue), 255);
            image[i][j].rgbtBlue = fmin(round(.272 * red + .534 * green + .131 * blue), 255);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            int swap = width - j - 1;
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][swap];
            image[i][swap] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create copy and populate by looping thru the 2d array of pixels
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    // create blur by referencing copy for avgs and writing to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avgcount = 0;
            float ravg = 0.0;
            float gavg = 0.0;
            float bavg = 0.0;
            for (int k = imax(i - 1, 0); k < imin(i + 2, height); k++)
            {
                for (int l = imax(j - 1, 0); l < imin(j + 2, width); l++)
                {
                    ravg += copy[k][l].rgbtRed;
                    gavg += copy[k][l].rgbtGreen;
                    bavg += copy[k][l].rgbtBlue;
                    avgcount++;
                }
            }
            image[i][j].rgbtRed = round(ravg / avgcount);
            image[i][j].rgbtGreen = round(gavg / avgcount);
            image[i][j].rgbtBlue = round(bavg / avgcount);
        }
    }
    return;
}

// integer min and max functions because C somehow does not have them in any headers i knew of and i was worried fmin/max was throwing my calc off in blur()
int imin(int n, int m)
{
    int min;

    if (n < m)
    {
        min = n;
    }
    else
    {
        min = m;
    }
    return min;
}
int imax(int n, int m)
{
    int max;

    if (n > m)
    {
        max = n;
    }
    else
    {
        max = m;
    }
    return max;
}