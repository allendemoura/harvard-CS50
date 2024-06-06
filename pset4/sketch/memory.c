#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *x = malloc(3 * sizeof(int));
    *x = 1234567890;
    x[1] = 1;
    printf("%i\n", *x);
    printf("%i\n", x[1]);
    printf("%i\n", x[0]);
    printf("%i\n", x[2]);
}