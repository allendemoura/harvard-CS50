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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // define kernels
    int gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    // create copy and populate by looping thru the 2d array of pixels
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    // define edges via sobel algo
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int ka = 0;

            float rsum = 0.0;
            float gsum = 0.0;
            float bsum = 0.0;

            float rgx = 0.0;
            float ggx = 0.0;
            float bgx = 0.0;

            float rgy = 0.0;
            float ggy = 0.0;
            float bgy = 0.0;

            for (int k = i - 1; k < i + 2; k++)
            {
                int kb = 0;
                if (k < 0 || k > height - 1)
                {
                    ka++;
                    continue;
                }
                for (int l = j - 1; l < j + 2; l++)
                {
                    if (l < 0 || l > width - 1)
                    {
                        kb++;
                        continue;
                    }

                    rgx += copy[k][l].rgbtRed * gx[ka][kb];
                    ggx += copy[k][l].rgbtGreen * gx[ka][kb];
                    bgx += copy[k][l].rgbtBlue * gx[ka][kb];

                    rgy += copy[k][l].rgbtRed * gy[ka][kb];
                    ggy += copy[k][l].rgbtGreen * gy[ka][kb];
                    bgy += copy[k][l].rgbtBlue * gy[ka][kb];

                    kb++;
                }
                ka++;
            }
            rsum = sqrt(pow(rgx, 2) + pow(rgy, 2));
            gsum = sqrt(pow(ggx, 2) + pow(ggy, 2));
            bsum = sqrt(pow(bgx, 2) + pow(bgy, 2));
            image[i][j].rgbtRed = round(fmin(rsum, 255));
            image[i][j].rgbtGreen = round(fmin(gsum, 255));
            image[i][j].rgbtBlue = round(fmin(bsum, 255));
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