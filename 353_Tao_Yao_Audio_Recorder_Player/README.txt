Audio Playback and Recorder(including makefile):

This is a simple program that allows you to record and playback audio using your default audio i/o device. It only support .wav format for now. Executable is available in ./src folder called audio_recorder_player.
When run it in the shell, first choose wheather you want to record or playback, and then type the name of audio file you want to create (for recording) or play (for playback) include its format (e.g. test.wav). 

If you choose to create the audio file for recording, it will be created in the ../audio_files folder, and if you want to playback an audio file, it must be put in the ../audio_files folder and the audio file must exist first.

Finally there are 5 different states command that you can control:
K: Start record/playback from the beginning.
S: Stop record/playback and prepare for the next task or play the current audio file back to beginning.
R: Continue to record/playback from the current position after you pause record/playback.
P: Pause record/playback and it will stop at the current position.
Q: Shut down all tasks and completely quit the program.

For recording, the default channel now is 2, for playback, the pa output channel is determined by your soundfile.

Note: You need to install both portaudio and libsndfile in order to run this program.

to compile program, cd to src folder and type make, and ./audio_recorder_player for running. to clean all files type make clean.
