#include <stdio.h>
#include <math.h>

void print_chord_frequencies(int chord[], int size);
float midi_to_frequency(int);

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

    print_chord_frequencies(chord, size);//returning hz

    return 0;
}

void print_chord_frequencies(int chord[], int size)
{
    for (int i = 0; i < size; i++)//looping through the frequency conversion for each no.
    {
        printf("The frequency of MIDI note %d is %.2f Hz.\n", chord[i], midi_to_frequency(chord[i]));
    }
}

float midi_to_frequency(int midi)
{
    float frequency;

    frequency = 440.0 * pow(2.0, (midi - 69) / 12.0); // converting with formula

    return frequency;
}