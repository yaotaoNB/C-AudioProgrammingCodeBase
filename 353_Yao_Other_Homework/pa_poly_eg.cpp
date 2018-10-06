//g++ -std=c++14 -L /usr/local/lib -lportaudio test.cpp -o test

//try if unique_ptr can be allocate with custom new float on the right

#include <iostream>
#include <cmath>
#include <portaudio.h>

using namespace std;

#define TABLE_SIZE (44100)
#define SAMPLE_RATE (44100)

struct OSC {
	float sine[TABLE_SIZE];
	int frequency;
	int phase = 0;
};

struct Sum {
	int phase = 0;	
	int channel_num = 2;
	float sum[TABLE_SIZE];
};

static int paCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData ) {
        
        float *out = (float*)outputBuffer;
        unsigned long i;

        Sum* final = (Sum*) userData;
        (void) inputBuffer;/* Prevent unused variable warnings. */
        (void) timeInfo; 
        (void) statusFlags;
        //cout<<"frames per buffer is: "<<framesPerBuffer<<endl;
        for( i=0; i<framesPerBuffer; i++ ) {
          for(int j = 0; j < final->channel_num; ++j) {
            *out++ = final->sum[final->phase] * 0.5; 
          }
          final->phase += 1;
          if( final->phase >= TABLE_SIZE ) final->phase -= TABLE_SIZE;
        }
        return paContinue;
}

int main(){
	OSC osc, osc2, osc3;
	osc.frequency = 261;
	osc2.frequency = 329 * 2;
	osc3.frequency = 392;
	Sum sum;
	for( int i=0; i<TABLE_SIZE; i++ )
        {
            osc.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
            osc2.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
            osc3.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
        }

    for(int i=0; i<TABLE_SIZE; i++){
		sum.sum[i] = (osc.sine[osc.phase] + osc2.sine[osc2.phase] + osc3.sine[osc3.phase]) / 3;
		osc.phase += osc.frequency;
		osc2.phase += osc2.frequency;
		osc3.phase += osc3.frequency;
    	if(osc.phase >= TABLE_SIZE) osc.phase -= TABLE_SIZE;
    	if(osc2.phase >= TABLE_SIZE) osc2.phase -= TABLE_SIZE;
    	if(osc3.phase >= TABLE_SIZE) osc3.phase -= TABLE_SIZE;
    }

  Pa_Initialize();
  PaStream* stream;
  PaStreamParameters outputParameters;
  outputParameters.device =  Pa_GetDefaultOutputDevice();
  outputParameters.channelCount = 2;       
  outputParameters.sampleFormat = paFloat32; 
  outputParameters.hostApiSpecificStreamInfo = NULL;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
  Pa_OpenStream(
            &stream,
            NULL, /* no input */
            &outputParameters,
            SAMPLE_RATE,
            paFramesPerBufferUnspecified,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            &paCallback,
            &sum);

  Pa_StartStream( stream );
  Pa_Sleep  (3000);
  Pa_StopStream( stream );
  Pa_CloseStream( stream );
  Pa_Terminate();
}
