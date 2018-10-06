							/****************************************************************************************
							*																						*
							*						This is a simple wavetable based OSC 							*
							*																						*
							*	we simply make table size and sample rate equal for pitch modification convenience.	*
							*																						*
							*																						*
							*																						*
							*																made by Tao Yao			*
							*																	09/28/2017			*
							*																						*
							*****************************************************************************************/

#ifndef WAVEGEN_H
#define WAVEGEN_H

#include <portaudio.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
using namespace std;

#define NUM_SECONDS   (2)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)
#define TABLE_SIZE   (44100)

class ScopedPaHandler{

public:
	ScopedPaHandler();
	~ScopedPaHandler();

	PaError result() const;

private:
	PaError _result;

};

//Base class for different wave gen
class OSC_Basic{

public:
	OSC_Basic();
	~OSC_Basic();

//use the device (passed as argument) and stream to play a note, return false if failed open
	bool open(PaDeviceIndex);
//start to play the note
	bool start();
//stop playing the note
	bool stop();
//after opening the device, close the stream
	bool close();

	//since table size = sample rate, this is samples to skip per loop
	int frequency;
protected:
	//override this function from inherited class for different processing
	virtual int paInstanceCallback(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags);

	//return paInstanceCallback() so that we can call the callBack of an instance instead of class
	static int paCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData );

	void paStreamFinishedMethod();

	static void paStreamFinished(void* userData);

	PaStream *stream;
	char message[20];
	float amp;
	int left_phase;
    int right_phase;
};

//sine OSC
class SineGen:public OSC_Basic{

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

public:
	SineGen();
	~SineGen();

private:
	int paInstanceCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags) override;

	float sine[TABLE_SIZE];
	//the sample to be skiped / index of wavetable
};

//saw OSC
 class SawGen:public OSC_Basic{

public:
	SawGen();
	~SawGen();

private:
	int paInstanceCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags) override;

	float saw[TABLE_SIZE];

};

//square OSC
 class SquareGen:public OSC_Basic{

 public:
 	SquareGen();

 private:
 	int paInstanceCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags) override;

 	float square[TABLE_SIZE];

 };

//triangle OSC
 class TriangleGen:public OSC_Basic{

 public:
 	TriangleGen();

 private:
 	int paInstanceCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags) override;

 	float triangle[TABLE_SIZE];

 };
		
#endif