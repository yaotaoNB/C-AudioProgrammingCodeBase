This program can read a text file containing multiple pairs of numbers (1st duration, 2nd pitch) seperate by spaces or new line, and generate a csd file depends on the memody we feed. 

The text file must be put in the melody folder in order for this program to read! No other character is allowed except interger or decimal. See the example in the melody folder called game_of_throne.txt, to feed this example in our program, ONLY type its file name withour .txt format when the program asks the name of file you want to feed, and the original BPM of this example is around 100 (program will ask for its BPM later).

Range of duration must between 0 to 4 (can be decimal), pitch have a range of 3 octaves (C3 - B5), use interger from 1 to 36 stands for each note (no other number is allowed), the following chart represent a relationship between note value and its actual pitch:

1:C3 (130.81) 2.C#3 (138.59) 3.D3 (146.83) 4.D#3 (155.56) 5.E3 (164.81) 6.F3 (174.61)7.F#3 (185.00) 8.G3 (196) 9.G#3 (207.65) 10.A3 (220.00) 11.A#3 (233.08) 12.B3 (264.94)

13:C4 (261.63) 14.C#4 (277.18) 15.D4 (293.66) 16.D#4 (311.13) 17.E4 (329.63) 18.F4 (349.23) 19.F#4 (369.99) 20.G4 (392) 21.G#4 (415.30) 22.A4 (440.00) 23.A#4 (466.16) 24.B4 (493.88)			

25:C5 (523.25) 26.C#5 (554.37) 27.D5 (587.33) 28.D#5 (622.25) 29.E5 (659.26) 30.F5 (698.46) 31.F#5 (739.99) 32.G5 (783.99) 33.G#5 (830.61) 34.A5 (880.00) 35.A#5 (932.33) 36.B5 (987.77)

All source codes are in src folder, and include folder has all header files. type make clean can clean all *.o, executable and .csd files.

I used the class from previous assignment as the base class for the class I'm using in this homework.

the name of executable is called melodygen, I typed wrong name for my cpp and header file called chordGen since I was thinking do a chord generator project, but don't be confused by that.