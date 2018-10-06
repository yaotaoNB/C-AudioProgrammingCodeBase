#include "waveGen.h"

/*Pitch (Hz) = SR / (TABLE_SIZE / samples skip per loop)
samples skip per loop = TABLE_SIZE / (SR / Pitch)
when SR = TABLE_SIZE
samples skip per loop = pitch
*/
ScopedPaHandler::ScopedPaHandler()
        : _result(Pa_Initialize())
    {
    }

ScopedPaHandler::~ScopedPaHandler()
    {
        if (_result == paNoError)
        {
            Pa_Terminate();
        }
    }

PaError ScopedPaHandler::result() const { return _result; }

/*child class can be pointed to by its base class pointer since child class is also its base class obj, but base class obj
can't be pointed to by its child class ptr since base class instance is not its child class obj, and you HAVE TO initialize base class members
in the base class instead of child class*/
OSC_Basic::OSC_Basic():stream(0), left_phase(0), right_phase(0), amp(0.8), frequency(440)//initialize frequency with 440 Hz
	{

	};

//destructor will be called when instance go out of scope
OSC_Basic::~OSC_Basic(){

	};

bool OSC_Basic::open(PaDeviceIndex index){
        PaStreamParameters outputParameters;

        outputParameters.device = index;
        if (outputParameters.device == paNoDevice) {
            return false;
        }

        const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(index);
        if (pInfo != 0)
        {
            printf("Output device name: '%s'\r", pInfo->name);
        }

        outputParameters.channelCount = 2;       /* stereo output */
        outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
        outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = NULL;

        PaError err = Pa_OpenStream(
            &stream,
            NULL, /* no input */
            &outputParameters,
            SAMPLE_RATE,
            paFramesPerBufferUnspecified,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            &OSC_Basic::paCallback,
            this            /* Using 'this' for userData so we can cast to Sine* in paCallback method; whatever userData argument in Pa_OpenStream will being passed and 
            processed by callBack function, the last userData argument in our callBack function (NOT Open_Stream) is actually come from Open_Stream, so that we can call 
            our instance callBack function (this->paCallbackMethod)*/
            );

        if (err != paNoError)
        {
            /* Failed to open stream to device !!! */
            return false;
        }
//Pa_SetStreamFinishedCallback will be called when the stream becomes inactive, set up the callback function that will be called in Pa_CloseStream( stream )
        err = Pa_SetStreamFinishedCallback( stream, &OSC_Basic::paStreamFinished );

        if (err != paNoError)
        {
            Pa_CloseStream( stream );
            stream = 0;

            return false;
        }

        return true;
    }

   bool OSC_Basic::close(){

   	if (stream == 0)
            return false;

        PaError err = Pa_CloseStream( stream );
        stream = 0;

        return (err == paNoError);

   }

     bool OSC_Basic::start()
    {
        if (stream == 0)
            return false;

        PaError err = Pa_StartStream( stream );

        return (err == paNoError);
    }

    bool OSC_Basic::stop()
    {
        if (stream == 0)
            return false;

        PaError err = Pa_StopStream( stream );

        return (err == paNoError);
    }

int OSC_Basic::paInstanceCallback(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags)
	{
		//override
        return 0;
	}

//The real callBack function, *userData is passed to this function from the function Pa_OpenStream()
int OSC_Basic::paCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData )
    {
        return ((OSC_Basic*)userData)->paInstanceCallback(inputBuffer, outputBuffer,//become this->paInstanceCallback()
            framesPerBuffer,
            timeInfo,
            statusFlags);
    }
//our custom StreamFinishedCallBack function
 void OSC_Basic::paStreamFinishedMethod()
    {
        printf( "Stream Completed: %s\n", message );
    }

 void OSC_Basic::paStreamFinished(void* userData)
    {
        return ((OSC_Basic*)userData)->paStreamFinishedMethod();
    }

//initialize base class members in the initialize list by calling base class constructor
 SineGen::SineGen() : OSC_Basic()
 	{
 		for( int i=0; i<TABLE_SIZE; i++ )
        {
            sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
        }

        printf( message, "No Message" );
 	}

 SineGen::~SineGen()
 	{

 	};

 int SineGen::paInstanceCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags)
 	{
 		float *out = (float*)outputBuffer;
        unsigned long i;

        (void) inputBuffer;/* Prevent unused variable warnings. */
        (void) timeInfo; 
        (void) statusFlags;

        for( i=0; i<framesPerBuffer; i++ )
        {
            *out++ = sine[left_phase];  /* left */
            *out++ = sine[right_phase];  /* right */
            left_phase += frequency;
            if( left_phase >= TABLE_SIZE ) left_phase -= TABLE_SIZE;
            right_phase += frequency; /* higher pitch so we can distinguish left and right. */
            if( right_phase >= TABLE_SIZE ) right_phase -= TABLE_SIZE;
        }
        return paContinue;
 	}

 SawGen::SawGen() : OSC_Basic()
	 {

        int half = TABLE_SIZE/2;
        float increase = amp/(float)half;
        float val = 0;

        for(int i = 0; i < TABLE_SIZE; ++i){
          saw[i] = val;
          if(i<(TABLE_SIZE/2)){
            val += increase;
          }else if(i == (TABLE_SIZE/2)){
            val -= (amp*2);
          }else if(i>(TABLE_SIZE/2)){
            val +=increase;
          }
        }
	 }

 SawGen::~SawGen()
	 {

	 }

 int SawGen::paInstanceCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags)
	 {
	 	float *out = (float*)outputBuffer;
		unsigned int i;
		(void) inputBuffer;/* Prevent unused variable warnings. */
        (void) timeInfo; 
        (void) statusFlags;

		for( i=0; i<framesPerBuffer; i++ )
		{
            *out++ = saw[left_phase];  /* left */
            *out++ = saw[right_phase];  /* right */
            left_phase += frequency;
            if( left_phase >= TABLE_SIZE ) left_phase -= TABLE_SIZE;
            right_phase += frequency; /* higher pitch so we can distinguish left and right. */
            if( right_phase >= TABLE_SIZE ) right_phase -= TABLE_SIZE;
		}
		return paContinue;
	 }

 SquareGen::SquareGen() : OSC_Basic()//initialize the amplitde of square wave
 {

 		for(int i = 0; i < TABLE_SIZE; ++i){

 			if(i<(TABLE_SIZE/2)){
 				square[i] = amp;
 			}else{
 				square[i] = -amp;
 			}
 		}
 }

 int SquareGen::paInstanceCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags)
 	 {
 	 	float *out = (float*)outputBuffer;
		unsigned int i;
		(void) inputBuffer;/* Prevent unused variable warnings. */
        (void) timeInfo; 
        (void) statusFlags;

		for( i=0; i<framesPerBuffer; i++ )
        {
            *out++ = square[left_phase];  /* left */
            *out++ = square[right_phase];  /* right */
            left_phase += frequency;
            if( left_phase >= TABLE_SIZE ) left_phase -= TABLE_SIZE;
            right_phase += frequency; /* higher pitch so we can distinguish left and right. */
            if( right_phase >= TABLE_SIZE ) right_phase -= TABLE_SIZE;
        }
        return paContinue;

 	 }

 TriangleGen::TriangleGen() : OSC_Basic()
 {
 		int quater = TABLE_SIZE/4;
 		float increase = amp/(float)quater;

 		for(int i = 0; i < TABLE_SIZE; ++i){

 			triangle[i] = increase;
 			if(i<=quater){
 				increase +=increase;
 			}else if (quater<i<=quater*2){
 				increase -=increase;
 			}else if (quater*2<i<quater*3){
 				increase -=increase;
 			}else{
 				increase +=increase;
 			}
 		}

 }

 int TriangleGen::paInstanceCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags)
 	 {
 	 	float *out = (float*)outputBuffer;
		unsigned int i;
		(void) inputBuffer;/* Prevent unused variable warnings. */
        (void) timeInfo; 
        (void) statusFlags;

		for( i=0; i<framesPerBuffer; i++ )
        {
            *out++ = triangle[left_phase];  /* left */
            *out++ = triangle[right_phase];  /* right */
            left_phase += frequency;
            if( left_phase >= TABLE_SIZE ) left_phase -= TABLE_SIZE;
            right_phase += frequency; /* higher pitch so we can distinguish left and right. */
            if( right_phase >= TABLE_SIZE ) right_phase -= TABLE_SIZE;
        }
        return paContinue;
 	 	
 	 }

