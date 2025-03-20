#include <stdio.h>
#include <math.h>

float midi_to_frequency(int);

int main()
{
    int midi;

    printf("Please enter MIDI note number:\n");//Requesting and getting midi no.
    scanf("%i", &midi);
    printf("The frequency of the MIDI note %i is %.2f hz.\n", midi, midi_to_frequency(midi));
    return 0;//returning the frequency with 2 digits after point and returning 0
}

float midi_to_frequency(int midi)
{
    float frequency;

    frequency = 440.0 * pow(2.0, (midi - 69) / 12.0);//converting with formula

    return frequency;
}