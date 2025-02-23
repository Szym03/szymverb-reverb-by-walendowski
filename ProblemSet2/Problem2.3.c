#include <stdio.h>

int main()
{
    int x;

    scanf("%d", &x); // receiving input

    if (x < 0 || x > 127)
    {
        printf("Invalid MIDI pitch. It should be between 0 and 127.\n");
        return 1; // handling wrong input
    }
    else
    {
        printf("The octave for MIDI pitch %d is %d.\n", x, x / 12 - 1);
        return 0; // calculating correct input
    }
}