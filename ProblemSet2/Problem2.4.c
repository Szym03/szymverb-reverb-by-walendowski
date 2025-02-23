#include <stdio.h>

#include <stdio.h>

char *named(int pitch)
{ // function to return name of the midipitch
    int note = pitch % 12;

    switch (note)
    {
    case 0:
        return "C";
    case 1:
        return "C#";
    case 2:
        return "D";
    case 3:
        return "D#";
    case 4:
        return "E";
    case 5:
        return "F";
    case 6:
        return "F#";
    case 7:
        return "G";
    case 8:
        return "G#";
    case 9:
        return "A";
    case 10:
        return "A#";
    case 11:
        return "B";
    default:
        return "ERROR! try again :("; // shoul not ever happen, but wouldn't compile without this.
    }
}

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
        printf("The MIDI pitch %d is %s%d.\n", x, named(x), x / 12 - 1);
        return 0;
    }
}