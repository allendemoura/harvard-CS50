#include <cs50.h>
#include <stdio.h>

int chksumcalc(long);

int main(void)
{
    // Prompt user for input
    long ccnum = get_long("Number: ");


    // Evaluate chksum
    int chksum = chksumcalc(ccnum);
    if (chksum == 0)
    {
        // evaluate for 13 digits for visa
        if (ccnum < 10000000000000)
        {
            if (ccnum / 1000000000000 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        // evaluate for 15 digits for amex
        else if (ccnum < 1000000000000000)
        {
            if (ccnum / 10000000000000 == 34 || ccnum / 10000000000000 == 37)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        // evaluate for 16 digits
        else if (ccnum < 10000000000000000)
        {
            // evaluate first digits 51-55 for mastercard
            if (ccnum / 100000000000000 >= 51 && ccnum / 100000000000000 <= 55)
            {
                printf("MASTERCARD\n");
            }
            // evaluate first digit 4 for visa
            else if (ccnum / 1000000000000000 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

// chksum calculator function
int chksumcalc(long ccnum)
{
    int chksum = 0;
    int seconddigitsum = 0;
    for (int digitcount = 0; digitcount < 8; digitcount++)
    {
        // add last digit to chksum
        chksum = ccnum % 10 + chksum;
        // second digit operations
        seconddigitsum = ccnum / 10 % 10 * 2;
        if (seconddigitsum > 9)
        {
            // extrapolate digits and add together
            seconddigitsum = (seconddigitsum % 10) + (seconddigitsum / 10 % 10);
            // add extrapolated digits' sum to chksum
            chksum = seconddigitsum + chksum;
        }
        else
        {
            // otherwise add second to last digit *2 to chksum
            chksum = seconddigitsum + chksum;
        }
        //drop last 2 digits to move on
        ccnum = ccnum / 100;
    }
    // obtain last digit of chksum
    chksum = chksum % 10;

    return chksum;
}

