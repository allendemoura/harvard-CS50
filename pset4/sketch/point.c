#include <stdio.h>

void printer(int *n);

int main(void)
{
    int n = 1;
    int *p = &n;
    printf("main: %i\n", n);
    printer(p);
    printer(p);
    printf("main after: %i\n", n);
}

void printer(int *n)
{
    *n = *n +1;
    printf("value of n in printer: %i\n", *n);
}