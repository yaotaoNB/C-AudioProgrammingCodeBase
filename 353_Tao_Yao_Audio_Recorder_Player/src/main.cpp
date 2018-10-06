#include "../include/audio_player.hpp"

int main(){
	string filename;
	bool done = false;
	int program_functionality;
	FileHandler filehandler;
	StreamHandler streamhandler = StreamHandler(); 
	AudioFile *loadfile;

	cout<<"Press 1 for recording, 2 for play a sound file."<<endl;
	cin>>program_functionality;
	switch (program_functionality)
	{
		case 1:
			streamhandler.setUpProgramFunctionality(func_record);
			cout<<"You picked recording."<<endl;
			cout<<"Type the name of the file you want to create including format name behind the file name, e.g. test.wav. (so far only support .wav format) the file will be created in ../audio_files folder. "<<endl;
			cin>>filename;
			//filename += ".wav";
			loadfile = filehandler.createAudioFile(filename);
		break;

		case 2:
			streamhandler.setUpProgramFunctionality(func_playback);
			cout<<"You picked playback."<<endl;
			cout<<"Please put your sound file in audio_files folder, and type its name here with its format after file name here. e.g. test.wav (so far only support .wav format)"<<endl;
			cin>>filename;
			//filename += ".wav";
			loadfile = filehandler.loadAudioFile(filename);
			if(!filehandler.checkExist())//if file do not exist
			{
				cout<<"file do not exist, or wrong file name. No space and format in the name is allowed."<<endl;
				delete loadfile;
				exit(1);
			}
		break;

		default:
			cout<<"Invalid option, choose either 1 or 2."<<endl;
			exit(1);
		break;
	}
	
		streamhandler.load_file(loadfile);
		/*this program uses default input_channel (2) for now*/
		streamhandler.setFileOutputChannel(filehandler.getFileOutputChannel());
		/*createFile() uses default srate (44100) for record_func for now*/
		streamhandler.setFileSampleRate(filehandler.getFileSampleRate());

		cout<<"Options: "<<endl;
		cout<<"K: Start."<<endl;
		cout<<"S: Stop."<<endl;
		cout<<"R: Continue to play when current state is pause."<<endl;
		cout<<"P: Pause."<<endl;
		cout<<"Q: Quit the program"<<endl;

		int status;
		while(!done)
		{
		/*getchar() will stop execution of following statements until it receives a letter, getchar() will be called twice, 1st time receive a letter,
		2nd time re-init back to 10(nothing) and wait for the next input*/
			status = getchar();
			switch (status)
			{
				case 'K':
				case 'k':
					streamhandler.processEvent(start);
					break;
				case 'S':
				case 's':
					streamhandler.processEvent(stop);
					break;
				case 'R':
				case 'r':
					streamhandler.processEvent(continue_to_play);
					break;
				case 'P':
				case 'p':
					streamhandler.processEvent(pause);
					break;
				case 'Q':
				case 'q':
					done = true;
					break;
	//getchar() default is 10 (line feed), will return to 10 and prepare for next get input from user
				case 10:
					break;
				default:
					cout<<"invalid command, choose a character from options."<<endl;
					break;
			}
		}

	return 0;
}
