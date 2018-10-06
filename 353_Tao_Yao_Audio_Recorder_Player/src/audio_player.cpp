#include "../include/audio_player.hpp"

AudioFile::~AudioFile()
{
	cout<<"AudioFile deallocator is called!"<<endl;
	sf_close(sndfile);
}

Playback::~Playback()
{
	delete audioFile;
}

FileHandler::FileHandler() : check_exist(false), file_output_channel(0), file_sample_rate(0)
{
	
}

FileHandler::~FileHandler()
{
	
}

int FileHandler::getFileOutputChannel()
{
	return this->file_output_channel;
}

int FileHandler::getFileSampleRate()
{
	return this->file_sample_rate;
}

bool FileHandler::checkExist()
{
	return this->check_exist;
}

AudioFile* FileHandler::loadAudioFile(string filename)
{
	/*
		When opening a file for read, the format field should be set to zero before calling sf_open().
		When opening a file for write, the caller must fill in structure members samplerate, channels, and format.
		Check if samplerate, channel and format of the file is supported by libsndfile in the future!
	*/
	string fillFileName = "../audio_files/" + filename;
	cout<<"Full file name is: "<<fillFileName<<endl;
	SF_INFO sf_info;
	sf_info.format = 0;
	SNDFILE * audio_file = sf_open(fillFileName.c_str(), SFM_READ, &sf_info);
	
	this->file_output_channel = sf_info.channels;
	this->file_sample_rate = sf_info.samplerate;

	if(!audio_file)
	{
		cout<<"Falied loading audio_file."<<endl;
		check_exist = false;
	}else
	{
		check_exist = true;
	}
	AudioFile *temp_file = new AudioFile 
										{	audio_file,
											sf_info
										};
	return temp_file;
}

AudioFile* FileHandler::createAudioFile(string filename)
{
	string fillFileName = "../audio_files/" + filename;//user type format name
	cout<<"The file you created: "<<fillFileName<<endl;
	SF_INFO sf_info;
	//use default sample rate and channel for now
	sf_info.samplerate = DEFAULT_SAMPLE_RATE;
	sf_info.channels = DEFAULT_CHANNEL_COUNT;//DEFAULT_CHANNEL_COUNT
	//sf_info.frames = DEFAULT_SAMPLE_RATE * 5;//try don't specify it later
	sf_info.format = (SF_FORMAT_WAV | SF_FORMAT_FLOAT);
	SNDFILE * audio_file = sf_open(fillFileName.c_str(), SFM_WRITE, &sf_info);

	this->file_output_channel = sf_info.channels;
	this->file_sample_rate = sf_info.samplerate;

	if(!audio_file)
	{
		cout<<"Falied loading audio_file."<<endl;
		check_exist = false;
	}else
	{
		check_exist = true;
	}

	AudioFile *temp_file = new AudioFile 
										{	audio_file,
											sf_info
										};
	return temp_file;
}

//instance callback function to implement audio playback
int StreamHandler::paInstanceCallback(
		const void *inputSample, 
		void *outputSample,//this output sample by sample (not dealing entire array!)
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags)
{
	if(this->program_functionality == func_playback)
	{
		//initialize all samples in the buffer to 0
		memset((float *)outputSample, 0, samplesCount * sizeof(float));
		
			bufferCursor = outputBuffer;
			framesLeft = framesPerBuffer;

			while (framesLeft > 0)
			{
				//update the frame position 
				sf_seek(playback_data.audioFile->sndfile, playback_data.current_Pos, SEEK_SET);
				//if buffer size is bigger than the frames left
				if(framesLeft > (playback_data.audioFile->sf_info.frames - playback_data.current_Pos))
				{
					framesToRead = playback_data.audioFile->sf_info.frames - playback_data.current_Pos;
				/* this might need to be changed since when there're multi channels */
					this->playback_status = playback_ended;
					framesLeft = framesToRead;
				}else
				{
					//Buffer size (in frames)
					framesToRead = framesLeft;
					playback_data.current_Pos += framesToRead;
				}
			//3rd argument of sf_readf_float is count in frames, this function will auto multiply frames with number of channels
				sf_readf_float(playback_data.audioFile->sndfile, bufferCursor, framesToRead);
			/*this should probably changed to bufferCursor += num_channels * framesToRead; but this statement shouldn't be necessary 
				since while loop will only be called once (so do this while loop) */
				bufferCursor += framesToRead; 
				framesLeft -= framesToRead;
			}
			
			outputCursor = (float*)outputSample;
			for(int i = 0; i < samplesCount; ++i)
			{
				*outputCursor = volume * outputBuffer[i];
				++outputCursor;
			}
		
        if(this->playback_status == playback_ended)
        {
        	cout<<"Reach the end of file, playback ended!"<<endl;
        	return paComplete;
        }else
        {
        	return paContinue;
        }
	}
	else if(this->program_functionality == func_record)
	{
	//memset((float *)inputSample, 0, samplesCount * sizeof(float));
	memset((float *)outputSample, 0, samplesCount * sizeof(float));
	framesToRead = framesPerBuffer;
	// !!! both input/output uses outputBuffer, change the name later !!!
	bufferCursor = inputBuffer;
	//for loop for both input and output, (!!! we only need one buffer[] so change inputBuffer and outputBuffer to one var buffer[] later !!!)
	inputCursor = (float*)inputSample;
	outputCursor = (float*)outputSample;
	
	for(int i = 0; i < samplesCount; ++i)
	{
		bufferCursor[i] = (*inputCursor) * volume;
		*outputCursor = bufferCursor[i];
		++inputCursor;
		++outputCursor;
	}
	//set bufferCursor back to the first element
	//bufferCursor = bufferCursor - samplesCount;
	sf_writef_float(playback_data.audioFile->sndfile, bufferCursor, framesToRead);//??? should the 3rd argument be frames or samples? (should it multiply with channels?)

	return paContinue;
	}	
	//!!! check out how many return options available later !!!
	return paContinue;
}

//Set choosing between playback/recording callback (enum)
int StreamHandler::paCallback( 
		const void *inputSample, 
		void *outputSample,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData )
{
    return ((StreamHandler*)userData)->paInstanceCallback(
        	inputSample, 
        	outputSample,//become this->paInstanceCallback
            framesPerBuffer,
            timeInfo,
            statusFlags);
}

//playback_load initialize current_pos to 0
void StreamHandler::processEvent(PlayBackStateMachine audioEventType)
{
	PaStreamParameters inputParameters,outputParameters;
	
  	if(this->program_functionality == func_playback)
  	{
	  	if(this->playback_status == playback_ended)
	  	{
	  		Pa_StopStream(stream);
	  		Pa_CloseStream(stream);
	  	    playback_data.current_Pos = 0;
	  		this->playback_status = stop;
	  	}
		//input parameter = null
	}
	else if(this->program_functionality == func_record)
	{
		inputParameters.device = Pa_GetDefaultInputDevice();
		if(inputParameters.device == paNoDevice)
		{
			cout<<"Error: No default input device."<<endl;
			goto error;
		}
		//use default input device parameters for now, try different parameters later
		/*???Check these parameters in pa API later, latency hostApiSpecificStreamInfo, flag and so on ???*/
		inputParameters.channelCount = this->input_channel;
		inputParameters.sampleFormat = paFloat32;
		inputParameters.suggestedLatency = 0.01;
		inputParameters.hostApiSpecificStreamInfo = 0;
	}
	  		
	  		switch (audioEventType)
	  		{
	  			case start:
	  			outputParameters.device = Pa_GetDefaultOutputDevice();
				if(outputParameters.device == paNoDevice)
			  	{
			  		cout<<"Error: No default output device."<<endl;
			  		goto error;
			  	}
				if(output_channel == 0)
				{
					outputParameters.channelCount = DEFAULT_CHANNEL_COUNT;
					cout<<"Use default output channel."<<endl;
				}else
				{
					outputParameters.channelCount = this->output_channel;
				}
				outputParameters.sampleFormat = paFloat32;
				outputParameters.suggestedLatency = 0.01;
				outputParameters.hostApiSpecificStreamInfo = 0;

  				if(Pa_IsStreamStopped(stream) && this->playback_status == stop)
  				{
				    if(this->srate == 0)
				    {
				    	this->srate = DEFAULT_SAMPLE_RATE;
				    }
					if(this->program_functionality == func_playback)
					{
						Pa_OpenStream(&stream,
					                  NO_INPUT,
					                  &outputParameters,
					                  this->srate,
					                  BUFFER_SIZE,/*paFramesPerBufferUnspecified*/
					                  paNoFlag,
					                  &paCallback,//don't modify the behavior of the streaming process
					                  this);
					}
					else if(this->program_functionality == func_record)
					{
						Pa_OpenStream(&stream,
					                  &inputParameters,
					                  &outputParameters,
					                  this->srate,
					                  BUFFER_SIZE,/*paFramesPerBufferUnspecified*/
					                  paNoFlag,
					                  &paCallback,//don't modify the behavior of the streaming process
					                  this);
					}
			
  					error = Pa_StartStream(stream);
  					if(error) goto error;
					this->playback_status = start;
  				}
  				else
  				{
  					cout<<"Error: Start state can only be triggered when current state is stop."<<endl;
  				}
  				break;
	  
	  			case stop:
	  				if(Pa_IsStreamActive(stream) && (this->playback_status == start || this->playback_status == continue_to_play))
	  				{
	  					Pa_StopStream(stream);
	  					Pa_CloseStream(stream);
	  					playback_data.current_Pos = 0;
	  					this->playback_status = stop;
	  				}else
	  				{
	  					cout<<"Error: Stop state can only be triggered when current state is start."<<endl;
	  				}
	  				break;

	  			case continue_to_play:
	  				if(Pa_IsStreamStopped(stream) && this->playback_status == pause)
	  				{
	  					error = Pa_StartStream(stream);
	  					if(error) goto error;
						this->playback_status = continue_to_play;
	  				}else
	  				{
	  					cout<<"Error: continue_to_play state can only be triggered when current state is pause."<<endl;
	  				}
	  				break;

	  			case pause:
	  				if(Pa_IsStreamActive(stream) && this->playback_status != stop)
	  				{
	  					Pa_StopStream(stream);
	  					this->playback_status = pause;
	  				}else
	  				{
	  					cout<<"Error: Pause state can only be triggered when current state is start or continue_to_play."<<endl;
	  				}
	  				break;
	  			
	  			default:
	  				cout<<"void StreamHandler::processEvent(default) is called!"<<endl;
	  				break;
	  		}
	return;	
  
  	error:
  		string error_message = Pa_GetErrorText(error);
		cout<<"Unable to open stream, Portaudio error code: "<<error_message<<endl;
  		Pa_Terminate();
  		return;
}

void StreamHandler::processEvent_Recording()
{

}

void StreamHandler::setUpProgramFunctionality(Program_Functionality setUp_PF)
{
	this->program_functionality = setUp_PF;
	if(this->program_functionality == func_record)
	{
		inputBuffer = new float[samplesCount];
	}
}

void StreamHandler::setFileOutputChannel(int get_output_channel)
{
	this->output_channel = get_output_channel;
	samplesCount = BUFFER_SIZE * output_channel;
	if(this->program_functionality == func_record)
	{
		delete[] inputBuffer;
		inputBuffer = new float[samplesCount];
	}
	delete[] outputBuffer;
	outputBuffer = new float[samplesCount];
	cout<<"output channel of the file is: "<<output_channel<<endl;
}

void StreamHandler::setFileSampleRate(int get_srate)
{
	this->srate = get_srate;
	cout<<"sample rate of the file is: "<<srate<<endl;
}

void StreamHandler::load_file(AudioFile *file_load)
{
	playback_data.audioFile = file_load;	
}

StreamHandler::StreamHandler() : 
	volume(1), framesLeft(0), framesToRead(0), bufferCursor(nullptr), inputCursor(nullptr), outputCursor(nullptr), 
	input_channel(DEFAULT_CHANNEL_COUNT), output_channel(DEFAULT_CHANNEL_COUNT), srate(0), playback_status(stop), 
	inputBuffer(nullptr)
{
	program_functionality = func_playback;//default as playback
	samplesCount = BUFFER_SIZE * output_channel;
	outputBuffer = new float[samplesCount];
	playback_data.current_Pos = 0;
	error = Pa_Initialize();
	
    if(error){
    	string error_message = Pa_GetErrorText(error);
    	Pa_Terminate();
    	std::cout<<"Unable to open stream for output. Portaudio error code: "<<error_message<<std::endl;
    }
}

StreamHandler::~StreamHandler()
{
	if(this->playback_status != stop)
	{
		Pa_StopStream(stream);
		Pa_CloseStream(stream);
	}
        Pa_Terminate();
        if(this->program_functionality == func_record) delete[] inputBuffer;
		delete[] outputBuffer;
}

		