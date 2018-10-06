#pragma once
#include <portaudio.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstring>
//#include <iomanip>
#include <list>
//#include <memory>
#include <algorithm>
using namespace std;

#define SAMPLE_RATE   (44100)
#define TABLE_SIZE   (44100)

template<typename Number_Type>
Number_Type Re_Range_Value(Number_Type thisvalue,Number_Type oldmin,Number_Type oldmax,Number_Type newmin,Number_Type newmax);

//???check why here must const?
//octative by how many time devided, note by result of %12
const float frequency_base[] = {8.176, 8.662, 9.177, 9.723, 10.301, 10.913, 11.562, 12.250, 12.978, 13.750, 14.568, 15.4338};//C0 - B0
int get_frequency(const int& midi_pitch);

//For initializing portaudio, handle initialize and terminate pa, when the instance of this class go out of scope, destructor will call pa_terminate
class ScopedPaHandler{
public:
	ScopedPaHandler();
	~ScopedPaHandler();

	PaError result() const;

private:
	PaError _result;

};

enum OSC_Table_Type {
	Sine, Triangle, Saw, Square, Custom_Load
};

enum FX_Module_List {
	Delay, LPF, ADSR
};

class FX {
public:
	string module_name;
	FX(){
	}
	virtual ~FX() {
	}
	virtual void operator()(float& pa_out) = 0;//this functor will within the callback frame_count for loop
	virtual void Param_Modify(float param) = 0;//modify 1 parameter of the child class itself
};

//Delay, the maxium delay time is 500 ms coz delay array size is SR/2
class FX_Delay : public FX {
private:
	bool *osc_status;
	//circular buffer and attenuation and the value we'll put to the delay line
	float *cir_buf, attenuation, fx_out;
	//read_pos for filling buffer, read_index for read buffer and write to output, delay_counter for count delay time to start
	int read_pos, read_index, cir_buf_size, delay_counter, samples_to_wait;
public:
	FX_Delay(bool* osc_status_ps, float delayTime = 500., float att_pass = 0.25, int buf_size_pass = SAMPLE_RATE/2);
	~FX_Delay();
	void Param_Modify(float param) override;//modify delay time
	void operator()(float& pa_out) override;
};

//ADSR withut Decay and Release (because stream stop is triggered by note off), but it doesn't need timer
class FX_ADSR : public FX {
public:
	FX_ADSR(bool* osc_status_ps, float atk_tm_ps = 1000., float sustain_amp_ps = 1.);
	~FX_ADSR() override;
	void operator()(float& pa_out) override;
	void Param_Modify(float param) override;//modify delay time
private:
//amp_incre is amp increment per sample from 0 to sustain_amp
	float store_amp, amp_incre, atk_tm, sustain_amp;
	int sample_tm;
	bool *osc_status;
};

//Filter
class FX_IIR_LPF : public FX {
public:
	FX_IIR_LPF(bool* osc_status_ps, float coe_ps = 0.5, float cir_buf_size_ps = 5);
	~FX_IIR_LPF();
	void Param_Modify(float param) override;
	void operator()(float& pa_out) override;

private:
	//coefficient
	float *cir_buf, coe;
	int index, cir_buf_size;
	bool *osc_status;
};

//1. WaveTable OSC, contain an empty table that can read a wavefrom from outside and 4 basic wavefrom function to fill the table
class OSC_Basic {

public:
	//constructor will open a stream, to change a device, create a new object
	OSC_Basic(PaDeviceIndex index = Pa_GetDefaultOutputDevice(), int chan_num = 2, PaSampleFormat _sampleFormat = paFloat32, void* hostAPIstreaminfo = NULL);
	//destructor will close a stream
	~OSC_Basic();

	void fill_table(OSC_Table_Type osc_table_type = Sine, float* pass_table = nullptr, int table_size = TABLE_SIZE);
//when note on, start stream, when note off, stop stream
	bool start_stream(const float& pass_amp, const int& midi_note);
	bool stop_stream();
	void Add_Module(FX_Module_List module);

	void Show_module();
	void rm_module();

	void paStreamFinishedMethod();
	static void paStreamFinished(void* userData);

	bool OSC_Status;

private:
	PaStream* stream;
	float amp;
//1. phase are index of L and R channel output, wave_table[], 2. since table size = sample rate, this is samples to skip per loop 3. number of channels
	int frequency, phase, channel_num;
	float wave_table[SAMPLE_RATE];

	int output_channel;
	PaSampleFormat output_sample_format;
	PaStreamParameters outputParameters;
	PaError err;
	char message[20];
	//a list of fx
	std::list<FX*> fx_list;

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
};
