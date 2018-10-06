#include "modular.h"

template<typename Number_Type>
Number_Type Re_Range_Value(Number_Type thisvalue,Number_Type oldmin,Number_Type oldmax,Number_Type newmin,Number_Type newmax){
		thisvalue = (((thisvalue - oldmin) * (newmax - newmin)) / (oldmax - oldmin)) + newmin;
		return thisvalue;
	}
/*	this step is necessary when using template in other cpp file
	template float Re_Range_Value<float>(float,float,float,float,float);	*/
int get_frequency(const int& midi_pitch){
	int octative = 0; 
	int note_index = midi_pitch % 12;
	int octative_count = midi_pitch;
	while(octative_count >= 0){
		octative_count -= 12;
		if(octative_count >= 0){
			++octative;
		}
	}
	float frequency_return = frequency_base[note_index] * pow(2,octative);
	//cout<<"MIDI note: "<<midi_pitch<<", real frequency: "<<frequency_return<<endl;
	return (int)frequency_return;
}

ScopedPaHandler::ScopedPaHandler() : _result(Pa_Initialize()) {
    }

ScopedPaHandler::~ScopedPaHandler() {
        if (_result == paNoError) {
            Pa_Terminate();
        }
    }

PaError ScopedPaHandler::result() const { return _result; }


OSC_Basic::OSC_Basic(PaDeviceIndex index, int chan_num, PaSampleFormat _sampleFormat, void* hostAPIstreaminfo)
					: phase(0), channel_num(chan_num), stream(0), OSC_Status(false)
	{
   		outputParameters.device = index;
        if (outputParameters.device == paNoDevice) {
            std::cout<<"No available device."<<std::endl;
            return;
        }
        const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(index);
        if (pInfo != 0) {
            //printf("Output device name: '%s'\r", pInfo->name);
        }
        outputParameters.channelCount = channel_num;       
        outputParameters.sampleFormat = _sampleFormat; 
        outputParameters.hostApiSpecificStreamInfo = hostAPIstreaminfo;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;//check out what's this argument

        err = Pa_OpenStream(
            &stream,
            NULL, /* no input */
            &outputParameters,
            SAMPLE_RATE,
            paFramesPerBufferUnspecified,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            &OSC_Basic::paCallback,
            this);

        if (err != paNoError) {
        	std::cout<<"An error occured during opening stream."<<std::endl;
        	cout<<Pa_GetErrorText(err)<<endl;
            return;
        }
//Pa_SetStreamFinishedCallback will be called when the stream becomes inactive, set up the callback function that will be called in Pa_CloseStream( stream )
        err = Pa_SetStreamFinishedCallback( stream, &OSC_Basic::paStreamFinished );

        if (err != paNoError) {
        	std::cout<<"An error occured during set stream finished callback."<<std::endl;
        	cout<<Pa_GetErrorText(err)<<endl;
            Pa_CloseStream( stream );
            stream = 0;
            return;
        }
	}

//destructor will be called when instance go out of scope
OSC_Basic::~OSC_Basic() {
		if (stream != NULL) PaError err = Pa_CloseStream( stream );
        stream = NULL;
    	if (err != paNoError) {
    		std::cout<<"An error occurs during closing stream in the destructor of OSC_Basic"<<std::endl;
    		cout<<Pa_GetErrorText(err)<<endl;
    	}
    	if(!fx_list.empty()) {
	    		for(auto it = fx_list.begin(); it != fx_list.end(); ++it){
	    		if(*it != nullptr){
	    			delete *it;
	    		}
    		}
    	}
    	
	}

void OSC_Basic::Add_Module(FX_Module_List module) {
	switch (module) {
		case Delay: {
			fx_list.push_back(new FX_Delay(&(this->OSC_Status)));
			break;
		}
		case LPF: {
			fx_list.push_back(new FX_IIR_LPF(&(this->OSC_Status)));
			break;
		}
		case ADSR: {
			fx_list.push_back(new FX_ADSR(&(this->OSC_Status)));
			break;
		}
		default: 
			std::cout<< "Non valid module name, adding module failed. "<<std::endl;
			break;
	}
}

void OSC_Basic::Show_module() {
	int i = 1;
	if(!fx_list.empty()){
		for(auto it = fx_list.begin(); it != fx_list.end(); ++it) {
			cout << i << ". " << (*it)->module_name<<endl;
			++i;
		}
	} else {
		cout<<"The FX queue is empty."<<endl;
	}
}

void OSC_Basic::rm_module() {
	if(!fx_list.empty()){
		fx_list.pop_back();
	} else {
		cout<<"The FX queue is empty."<<endl;
	}
}

//load table first, and streaming second
void OSC_Basic::fill_table(OSC_Table_Type osc_table_type, float* pass_table, int table_size) {
	float increase, wave_amp = 1.0, val = 0.;
	//when using switch and declare variables, to make it invisible in other cases we need a scope for each cases
	switch (osc_table_type){
			case Sine: {
				for( int i=0; i<table_size; i++ ) {
		            wave_table[i] = (float) sin( ((double)i/(double)table_size) * M_PI * 2. );
		        }
			break;
			}

			case Triangle: {
				int quater = table_size/4;
		 		float increase = (wave_amp/(float)quater);
		 		for(int i = 0; i < table_size; ++i){
		 			wave_table[i] = val;
		 			if(i<=quater){
		 				val +=increase;
		 			}else if (i > quater && i<=quater*2){
		 				val -=increase;
		 			}else if (i > quater*2 && i<=quater*3){
		 				val -=increase;
		 			}else {
		 				val +=increase;
		 			}
		 		}
			break;
			}

			case Saw: {
				int half = table_size/2;
			    increase = wave_amp/(float)half;

			    for(int i = 0; i < table_size; ++i){
			      wave_table[i] = val;
			      if(i<(table_size/2)){
			        val += increase;
			      }else if(i == (table_size/2)){
			        val -= (wave_amp*2);
			      }else if(i>(table_size/2)){
			        val +=increase;
			      }
			    }
			break;
			}

			case Square: {
				for(int i = 0; i < table_size; ++i){
				if(i<(table_size/2)){
					wave_table[i] = wave_amp;
				}else{
					wave_table[i] = -wave_amp;
					}
				}
			break;
			}

			case Custom_Load:
				//load custom wave table file
				cout<<"Custom_Load is called."<<endl;
			break;

			default:
				cout<<"default is called."<<endl;
			break;
		}
	}

bool OSC_Basic::start_stream(const float& midi_amp, const int& midi_note) {
	//phase = 0;
	amp = Re_Range_Value<float>(midi_amp,0.,127.,0.,1.);
    frequency = get_frequency(midi_note);
    //cout<<"start_stream, amp: "<<amp<<", freq: "<<frequency<<endl;
//Start a stream
   	//if (stream == 0)	return false;
    PaError err = Pa_StartStream( stream );
    return (err == paNoError);
}

//it's truely pass midi note number
bool OSC_Basic::stop_stream(){
	if (stream == 0)	return false;
    PaError err = Pa_StopStream( stream );
    //if (err != paNoError) std::cout<<"An error occurs during stop stream during note off"<<std::endl;
    //cout<<"Now the total number of the stream is: "<<streams.size()<<endl;
    return (err == paNoError);
}

int OSC_Basic::paInstanceCallback(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags)
	{
		float *out = (float*)outputBuffer;
        unsigned long i;
        (void) inputBuffer;/* Prevent unused variable warnings. */
        (void) timeInfo; 
        (void) statusFlags;
        float pa_output;
        for( i=0; i<framesPerBuffer; i++ ) {
        	pa_output = wave_table[phase];
        	for(int j = 0; j < channel_num; ++j) {
        		//int jb = 0;
        		for(auto it = fx_list.begin(); it != fx_list.end(); ++it){
        			(**it)(pa_output);
        			//std::cout<<"jb: "<< jb <<std::endl;
        		}
        	if(this->OSC_Status == false) this->OSC_Status = true;
        		*out++ = pa_output * amp;
        	}
        phase += frequency;
        if( phase >= TABLE_SIZE ) phase -= TABLE_SIZE;
        }
        return paContinue;
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
 void OSC_Basic::paStreamFinishedMethod() {
 	this->OSC_Status = false;
    }

 void OSC_Basic::paStreamFinished(void* userData) {
        return ((OSC_Basic*)userData)->paStreamFinishedMethod();
    }

 FX_Delay::FX_Delay(bool* osc_status_ps, float delayTime, float att_pass, int buf_size_pass) : osc_status(osc_status_ps),
 	cir_buf_size(buf_size_pass), attenuation(att_pass), read_pos(0), delay_counter(0), read_index(0) {
 		module_name = "Delay";
 		cir_buf = new float[cir_buf_size];
 		if(delayTime > 500) {
 			cout<<"Delay time can't be larger than the result of cir_buf_size / SAMPLE_RATE division." <<endl;
 			delayTime = 500;
 		}
 		samples_to_wait = (int) ((SAMPLE_RATE / 1000) * delayTime);
 		//cout<<"samples_to_wait: "<<samples_to_wait<<endl;
 }

 FX_Delay::~FX_Delay() {
 	//std::cout<<"~FX_Delay is called."<<std::endl;
 	delete[] cir_buf;
 }

 void FX_Delay::Param_Modify(float param) {
 	if(param > 500) {
 			cout<<"Delay time can't be larger than the result of cir_buf_size / SAMPLE_RATE division." <<endl;
 			param = 500;
 		}
 		samples_to_wait = (int) ((SAMPLE_RATE / 1000) * param);
 		delay_counter = read_index = 0;
 	}

 void FX_Delay::operator()(float& pa_out) {
 		//std::cout<<"FX_Delay::operator() is called."<<std::endl;
	 	if(*osc_status == false) {
			std::fill(cir_buf, cir_buf + cir_buf_size, 0);
			//*osc_status = true;
		} 
 		fx_out = cir_buf[read_pos];//store in temp var
 		cir_buf[++read_pos] = pa_out + (fx_out * attenuation);//fill delay_line
		if(read_pos == cir_buf_size) read_pos = 0;//check range for circular
		if(delay_counter < samples_to_wait) {
			++delay_counter;
		} else {
			pa_out += cir_buf[read_index];
			++read_index;
			if(read_index >= cir_buf_size) read_index = 0;
		}
	}

FX_ADSR::FX_ADSR(bool* osc_status_ps, float atk_tm_ps, float sustain_amp_ps) : 
	osc_status(osc_status_ps), store_amp(0.), sustain_amp(sustain_amp_ps) {
	module_name = "ADSR";
	sample_tm = (int)(SAMPLE_RATE / (1000. / atk_tm_ps));
	amp_incre = sustain_amp / sample_tm;
}

FX_ADSR::~FX_ADSR() {
	//cout<<"~FX_ADSR() is called."<<endl;
}

void FX_ADSR::operator()(float& pa_out) {
	if(*osc_status == false) {
		store_amp = 0;
	} 
	if(store_amp <= sustain_amp){
		store_amp += amp_incre;
		pa_out *= store_amp;
	}
}

void FX_ADSR::Param_Modify(float param) {

}

FX_IIR_LPF::FX_IIR_LPF(bool* osc_status_ps, float coe_ps, float cir_buf_size_ps) 
	:	osc_status(osc_status_ps), coe(coe_ps), cir_buf_size(cir_buf_size_ps), index(0)

{
	module_name = "LPF";
	cir_buf = new float[cir_buf_size];
	std::fill(cir_buf, cir_buf + cir_buf_size, 0);
}

FX_IIR_LPF::~FX_IIR_LPF() {
	//cout<<"~FX_IIR_LPF is called."<<endl;
	delete[] cir_buf;
}

void FX_IIR_LPF::Param_Modify(float param) {

}

void FX_IIR_LPF::operator()(float& pa_out) {
	//pa_out = coe*(cir_buf[index]) + coe*(cir_buf[index-3])
	if(*osc_status == false) {
			std::fill(cir_buf, cir_buf + cir_buf_size, 0);
		}
	cir_buf[index] = pa_out;
	//go back 3 sample to make filter steeper
	int wr_index_1 = (index - 2) % cir_buf_size;
	//int wr_index_2 = (index - 2) % cir_buf_size;
	pa_out = (coe * pa_out) + (coe * cir_buf[wr_index_1]) /*+ (coe * cir_buf[wr_index_2])*/;
	++index;
	if(index >= cir_buf_size) index = 0;
}


