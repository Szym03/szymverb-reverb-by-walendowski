#include <stdio.h>

const char* getIntervalName(int interval) {
    switch (interval) {
        case 0: return "unison";
        case 1: return "minor second";
        case 2: return "major second";
        case 3: return "minor third";
        case 4: return "major third";
        case 5: return "perfect fourth";
        case 6: return "tritone";
        case 7: return "perfect fifth";
        case 8: return "minor sixth";
        case 9: return "major sixth";
        case 10: return "minor seventh";
        case 11: return "major seventh";
        default: return "????"; //again, needed.
    }
}

int main() {
    for (int note1 = 0; note1 < 12; note1++) { //first note
        for (int note2 = 0; note2 < 12; note2++) {//seond note
            int interval = (note2 - note1 + 12) % 12; //  non-negative interval
            printf("The interval from note %d to note %d is a %s.\n", note1, note2, getIntervalName(interval));
        }
        printf("\n"); 
    }
    return 0;
}
