#include <stdio.h>

typedef struct
{ // defining structure with typedef for easier initialization
    int pitch;
    int velocity;
    int channel;
} MIDINote;

void print_note(MIDINote note);

int main()
{

    MIDINote note;

    // getting input for the note and handling wrong numbers by repeating the question
    do
    {
        printf("Enter the MIDI pitch (0-127): ");
        scanf("%d", &note.pitch);
    } while (note.pitch < 0 || note.pitch > 127);

    do
    {
        printf("Enter the MIDI velocity (0-127): ");
        scanf("%d", &note.velocity);
    } while (note.velocity < 0 || note.velocity > 127);

    do
    {
        printf("Enter the MIDI channel (1-16): ");
        scanf("%d", &note.channel);
    } while (note.channel < 1 || note.channel > 16);

    print_note(note);
}

void print_note(MIDINote note)
{
    // printing the appropriate data
    printf("The MIDI note has pitch %i, velocity %i, and channel %i.\n", note.pitch, note.velocity, note.channel);
}