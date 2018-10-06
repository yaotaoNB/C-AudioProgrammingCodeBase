To compile the program, first cd to src folder, then type:
g++ -std=c++14 -I../usr/local/include -L../usr/local/lib -lrtmidi -lportaudio ../modules/modular.cpp main.cpp -o final_project

This is a MIDI playable synthesizer program with 3 fx modules that can be added in the fx queue, modules are:

1. A simple OSC that can generate 4 different waveforms

2. An delay line based echo effect module.

3. A delay line based simple filter module.

5. An ADSR that modulate the amplitude without decay and release.

To play the program, first plug-in a midi keyboard, run the program and type a number on conputer keyboard to pick selection and command acroding to 
the instruction when running the program.

Library used: portaudio and rtmidi.