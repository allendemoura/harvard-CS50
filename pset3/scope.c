#include <cs50.h>
#include <stdio.h>

void test(int x[]);

int main(void)
{
    int x[2] = {0 , 0};
    printf("value of x in main(): %i\n", x[0]);
    test(x);
    printf("value of x in main() after test() was called: %i\n", x[0]);
}

void test(int x[])
{
    x[0] = 1;
    printf("value of x in test(): %i\n", x[0]);
}

// huh. so this taught me that C passes arrays as pointers. meaning normal scope
// rules dont apply to arrays. gremlin shit
