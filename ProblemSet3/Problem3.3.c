#include <stdio.h>
#include <math.h>

void print_chord_frequencies(int chord[], int size);
float midi_to_frequency(int);
void frequencies_of_chord(int* chord, int size, float* frequencies);

int main()
{
    int size;
    printf("Please enter the number of  notes in chord:\n");
    scanf("%d", &size);

    int chord[size];

    printf("Please enter the MIDI notes in the chord separated by spaces:\n");

    for (int i = 0; i < size; i++)//looping inserting numbers into array
    {
        scanf("%d", &chord[i]);
    }

    float frequencies[size];

    frequencies_of_chord(chord,size,frequencies);

    for (int i = 0; i < size; i++)
    {
        printf("The frequency of MIDI note %d is %.2f hz.\n",chord[i],frequencies[i]);
    }

    return 0;
}


float midi_to_frequency(int midi)
{
    float frequency;

    frequency = 440.0 * pow(2.0, (midi - 69) / 12.0); // converting with formula

    return frequency;
}

void frequencies_of_chord(int* chord, int size, float* frequencies)
{
    for (int i = 0; i < size; i++)
    {
        frequencies[i] = midi_to_frequency(chord[i]);  // storing calculated frequency
    }
}