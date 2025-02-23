#include <stdio.h>
#include <math.h>

float freq(int x)//calculating frequency based on midi number
{
    // midi pitch to freq formula: f = 2^((p-69)/12)) * 440

    return pow(2, (x - 69) / 12.0) * 440;
}
int main()
{
    int C4 = 60;
    int A4 = 69; // Assign variables

    float C4f = freq(C4);
    float A4f = freq(A4);

    printf("The MIDI pitch %i (C4) is %f.\n", C4, C4f);
    printf("The MIDI pitch %i (A4) is %f.\n", A4, A4f); // Print variables

    return 0;
}