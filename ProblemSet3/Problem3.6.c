#include <stdio.h>

typedef struct
{ // defining structure with typedef for easier initialization
    int pitch;
    int velocity;
    int channel;
} MIDINote;

void transpose_notes(MIDINote *notes, int size, int steps);
void print_notes(MIDINote notes[], int size);

int main()
{

    int size;

    printf("Enter the number of MIDI notes: ");
    scanf("%d", &size);

    MIDINote notes[size]; // creating an array for the notes

    for (int i = 0; i < size; i++)
    {
        // looping to get user input for desired number of notes
        printf("Enter pitch, velocity, and channel for note %d (separated by spaces):\n", i + 1);
        scanf("%d %d %d", &notes[i].pitch, &notes[i].velocity, &notes[i].channel);
    }

    int steps;

    printf("Please enter the number of steps to transpose the pitch.\n");
    scanf("%d", &steps);

    transpose_notes(notes, size, steps);//calling the transposing and then the print function

    print_notes(notes, size);

    return 0;
}

void transpose_notes(MIDINote *notes, int size, int steps)
{
    // transposing all pitches
    for (int i = 0; i < size; i++)
    {
        notes[i].pitch += steps;
    }
}

void print_notes(MIDINote notes[], int size)
{
    // printing with for loop
    for (int i = 0; i < size; i++)
    {
        printf("The MIDI note has pitch %d, velocity %d, and channel %d.\n", notes[i].pitch, notes[i].velocity, notes[i].channel);
    }
}