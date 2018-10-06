#pragma once

#include <portaudio.h>
#include <sndfile.h>
#include <iostream>
#include <string>
/*#include <memory>*/

#define DEFAULT_CHANNEL_COUNT (2)
#define DEFAULT_SAMPLE_RATE (44100)
#define BUFFER_SIZE (256)
#define NO_INPUT (nullptr)

using namespace std;

enum PlayBackStateMachine
{
    start, stop, continue_to_play, pause, playback_ended
};

enum Program_Functionality
{
	func_playback, func_record
};

struct AudioFile
{
	SNDFILE *sndfile;
	SF_INFO sf_info;

	~AudioFile();
};

struct Playback
{
	AudioFile *audioFile = nullptr;
	int current_Pos;

	~Playback();
};

class FileHandler
{
	private:
		bool check_exist;//true: exist, false: not exists
		int file_output_channel, file_sample_rate;
	
	public:
		FileHandler();
		~FileHandler();
		int getFileOutputChannel();
		int getFileSampleRate();
		bool checkExist();
		AudioFile *loadAudioFile(string filename);
		AudioFile *createAudioFile(string filename);
};

//Initialize/Terminate/OpenStream pa through constructor/deconstructor of this class
class StreamHandler
{
	private:
		PaError error;
		PaStream *stream;
		int input_channel, output_channel,srate;//don't worry about input_channel for now
	//change these pointer names for both input and output later
		float *bufferCursor,*inputBuffer,*inputCursor,*outputBuffer,*outputCursor;
		unsigned int framesLeft, framesToRead;
		unsigned long samplesCount;
		Playback playback_data;
		PlayBackStateMachine playback_status;
		Program_Functionality program_functionality;
		
		int paInstanceCallback(
		const void *inputSample, 
		void *outputSample,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags);
		
		static int paCallback(
			const void *inputSample, 
			void *outputSample,
	        unsigned long framesPerBuffer,
	        const PaStreamCallbackTimeInfo* timeInfo,
	        PaStreamCallbackFlags statusFlags,
	        void *userData);

	public:
		float volume;
		//bool playBackEnded;
		StreamHandler();
		~StreamHandler();
		void setUpProgramFunctionality(Program_Functionality setUp_PF);
		void load_file(AudioFile *file_load);
		void setFileOutputChannel(int get_output_channel);
		void setFileSampleRate(int get_srate);
		void processEvent(PlayBackStateMachine audioEventType);
		void processEvent_Recording();
};