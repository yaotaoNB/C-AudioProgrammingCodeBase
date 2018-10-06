This program will random generate random chord progression depending on the user choices, and create a csd file. All parameters are controled through command line arguments, there are 3 instruments in csd file, sine as bass note, sawtooth as chord, square as arpegio.

To compile:
gcc algorithmic_compose.c -o algorithmic_compose

To run the program:
e.g. ./algorithmic_compose pop A# major 4

command argument instructions:
(1)./algorithmic_compose: name of the program

(2)choose and type either jazz or pop, will generate different rhythm.

(3)The key, choose amoung 12 keys:
"C", "C#", "Db", "D", "D#", "Eb", "E", "F", "F#", "Gb", "G", "G#", "Ab", "A", "A#", "Bb", "B"

(4)Type major or minor determin wheather it's major or minor key.

(5)bar number: type a number for the last argument, it has to be multiple of 2!