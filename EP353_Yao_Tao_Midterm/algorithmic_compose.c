#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VOLUME (8500)

const char *command = "csound -odac ./algorithmic_compose.csd";
char csdheader[] = "<CsoundSynthesizer>\n<CsOptions>\n</CsOptions>\n<CsInstruments>\nsr = 44100\nkr = 4410\nksmps = 10\nnchnls = 2\n";
char csdmid[] = "\n\n</CsInstruments>\n<CsScore>\n\n";
char ftable[] = "\nf1 0 16384 10 1   ;Sine\nf2 0 16384 10 1 0.5 0.3 0.25 0.2 0.167 0.14 0.125 .111   ;Sawtooth\nf3 0 16384 10 1 0 0.3 0  0.2 0   0.14 0   .111   ;Square\n\n";
char csdfooter[] = "\n\n</CsScore>\n</CsoundSynthesizer>\n";

//Notes and Keys: enum map, so that we can directly use index to get an element value
static const char *notes_check[] = { "C", "C#", "Db", "D", "D#", "Eb", "E", "F", "F#", "Gb", "G", "G#", "Ab", "A", "A#", "Bb", "B" };
typedef enum { C = 0, Cs = 1, Db = 1, D = 2, Ds = 3, Eb = 3, E = 4, F = 5, Fs = 6, Gb = 6, G = 7, Gs = 8, Ab = 8, A = 9, As = 10, Bb = 10, B = 11 } Notes;
static const Notes notes_map[] = { C, Cs, Db, D, Ds, Eb, E, F, Fs, Gb, G, Gs, Ab, A, As, Bb, B };
static const float notes[] = { 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440, 466.16, 493.88};

//controls rhythm of all instruments, pop = 8 straight notes, jazz = swing
char *style[] = {"pop","jazz"};
char *mood[] = {"Major", "major", "Minor", "minor"};

//1st index stand for the current chord, 2nd index stand for degree of next chord, value is percentage to next chord
int next_chord_percentage[7][7] = 
{
	{ 0, 10, 5, 25, 25, 30, 5 },
	{ 0, 0, 20, 20, 40, 20, 0 },
	{ 0, 20, 0, 20, 0, 40, 20 },
	{ 35, 0, 10, 0, 35, 10, 10 },
	{ 60, 10, 10, 10, 0, 10, 0 },
	{ 20, 25, 25, 10, 10, 0, 10 },
	{ 40, 20, 20, 0, 20, 0, 0 }
};

void write_to_file(char *style, int note_index_import, char *mood_sel, int bar_num);

int main(int argc, char *argv[])
{
	srand(time(NULL));
	int i, array_length, note_index, total_check;
	//check argument number
	if(argc != 5)
	{
		printf("Error: invalid number of command line arguments, expect 3 arguments behind the program name, 1st argument determins rhythm, 2nd determins key, 3rd determins mood (wheather it's major or minor key), 4th argument determins how many bars to generate (multiple of 2) e.g. ./algorithmic_compose pop C major 8\n");
		return 1;
	}
	//check first argument is valid
	//strcmp() return 0 if 2 string are equal
	i = 0;
	while(strcmp(argv[1], style[i]) != 0)
	{
		i++;
		if(i == 2)//when reach the end of array
		{
			printf("Error: invalid style/first argument, style argument should be the 1st argument after program name, choose between pop or jazz (all lower case), e.g. ./algorithmic_compose pop C major 8\n");
			return 1;
		}
	}
	//check 2nd argument is valid
	i = 0;
	array_length = sizeof(notes_check) / sizeof(notes_check[0]);
	while(strcmp(argv[2], notes_check[i]) != 0)
	{
		i++;
		if(i == array_length)
		{
			printf("Error: invalid key/second argument, choose one key from 12 keys (key must be capitalized), e.g. ./algorithmic_compose pop C# major 8\n");
			return 1;
		}
	}
	note_index = i;
	//check 3rd argument
	i = 0;
	array_length = sizeof(mood) / sizeof(mood[0]);
	while(strcmp(argv[3], mood[i]) != 0)
	{
		i++;
		if(i == array_length)
		{
			printf("Error: invalid mood/3rd argument, please choose between major and minor. e.g. ./algorithmic_compose pop C# major 8\n");
			return 1;
		}
	}
	//check 4th argument
	if((atoi(argv[4]) % 2) != 0)
	{
		printf("Error: invalid bar number/4th argument, bar number has to be multiple of 2. e.g. ./algorithmic_compose pop C# major 8\n");
	}

	//check if all number add together is 100
	for(int i = 0; i < 7; i++)
	{
		total_check = 0;
		for(int k = 0; k < 7; k++)
		{
			total_check = total_check + next_chord_percentage[i][k];
		}
		if(total_check > 100)
		{
			printf("Error: total percentage of member [%d] exceed 100. ",i);
			exit(1);
		}
	}

	int bars_num = atoi(argv[4]);
	write_to_file(argv[1], note_index, argv[3], bars_num);
	system(command);
	printf("program finished.\n");
	return 0;
}

int mood_transfer(char *mood_sel)
{
	if(strcmp(mood_sel, "Major") == 0 || strcmp(mood_sel, "major") == 0)
		return 0;
	if(strcmp(mood_sel, "Minor") == 0 || strcmp(mood_sel, "minor") == 0)
		return 1;
	return 0;//major by default
}

//deal with 0 - 11
void build_scale(int *scale, int root, int mood_sel)
{
	int i, k;
	k = root;
	for(i = 0; i < 7; i++)
	{
		scale[i] = k;
		switch (mood_sel)
		{
			case 0://major key
				if(i == 2) k++;
				else k = k + 2;
			break;
				
			case 1://minor key
				if(i == 1 || i == 4) k++;
				else k = k + 2;
			break;
		}
		if(k > 11) k = k % 12;
	}
}

int random_num(int *total_already_ptr, int percentage, int dice_check)
{
	int total = *total_already_ptr + percentage;
	if(total > 100)
	{
		exit(1);
		printf("Error: total percentage exceed 100.");
	}
	if(*total_already_ptr < dice_check && dice_check <= total) return 1;//true
	else return 0;//false
	
}

//deal with 0 - 7
int get_next_chord(int *_current_chord)
{
	int dice = (rand() % 100) + 1;
	int total_already = 0;
	int current_percentage, chord_return;
	for(int i = 0; i < 7; i++)
	{
		current_percentage = next_chord_percentage[*_current_chord][i];
		if(random_num(&total_already, current_percentage, dice) == 0)
		{
			total_already = total_already + current_percentage;
		}else
		{
			chord_return = i;
			break;
		}
	}
	printf("The current chord is: %d \n\n", chord_return);
	return chord_return;
}

//mood 0 = Major, mood 1 = minor, degree deal with 0 - 7, scale is the real index number 0 - 11
void build_chord_tone(float *_chord_tone, int *_scale, int degree)
{
	for(int i = 0; i < 4; i++)
	{
		_chord_tone[i] = notes[_scale[degree]];
		degree = degree + 2;
		if(degree > 6) degree = degree % 7;//when 7 (root), turn around
	}
}

//command line argument ./... style key major/minor bars
void write_to_file(char *style, int note_index_import, char *mood_sel, int bar_num)
{
	char instrument_one[] = "instr 1\n asig	  oscil  5300, p4 , 1\n outs asig, asig\n endin\n";
	char instrument_two[] = "instr 2\n asig	  oscil  5300, p4 , 2\n outs asig, asig\n endin\n";
	char instrument_three[] = "instr 3\n asig	  oscil  6000, p4 , 3\n outs asig, asig\n endin\n";
	FILE *file_ptr;
	file_ptr = fopen("algorithmic_compose.csd","w");
	int mood_choice = mood_transfer(mood_sel);
	int scale[7], current_chord_degree = rand() % 6;
	float total_duration, root_note, chord_duration[2], note_duration[2], chord_tone[4];
	total_duration = 0;
	char *instr_bass, *instr_chord, *instr_melody;
	
	if(strcmp(style, "pop") == 0)//pop will only need 1 note duration
	{
		chord_duration[0] = 1;
		chord_duration[1] = 1;
	}else if(strcmp(style, "jazz") == 0)
	{
		chord_duration[0] = 0.667;
		chord_duration[1] = 0.333;
	}
	
	int key_index = notes_map[note_index_import];
	build_scale(scale, key_index, mood_choice);
	//header
	fprintf(file_ptr, "%s\n", csdheader);
	//instrument, chord for now
	fprintf(file_ptr, "\n%s\n", instrument_one);
	fprintf(file_ptr, "\n%s\n", instrument_two);
	fprintf(file_ptr, "\n%s\n", instrument_three);
	//mid
	fprintf(file_ptr, "%s\n", csdmid);
	//ftable 
	fprintf(file_ptr, "%s\n", ftable);
	//note list: instr 2
	int cur_chord_dur_index = 0, pre_chord_dur_index = 0, melody_index;
	float mel_freq, mel_total_dur;
	char root_note_char[50], total_duration_char[50], mel_total_duration_char[50], mel_note_dur[50], mel_frequency_char[50], chord_duration_char[50], frequency_char[50];
	//get current chord
	printf("current_chord_degree: %d \n", current_chord_degree);
	for(int i = 0; i < (bar_num * 4); i++)
	{
		sprintf(total_duration_char, "%f", total_duration);
		if(cur_chord_dur_index == pre_chord_dur_index)
		{
			if(pre_chord_dur_index == 0) cur_chord_dur_index = 1;
			else if (pre_chord_dur_index == 1) cur_chord_dur_index = 0;
		}
		pre_chord_dur_index = cur_chord_dur_index;
		sprintf(chord_duration_char, "%f", chord_duration[cur_chord_dur_index]);
		//build chord tone
		build_chord_tone(chord_tone, scale, current_chord_degree);
		for(int k = 0; k < 4; k++)
		{
			sprintf(frequency_char, "%f", chord_tone[k]);
			//instr 1 (bass)
			if(k == 0)
			{
				root_note = chord_tone[k] / 4;
				sprintf(root_note_char, "%f", root_note);
				fprintf(file_ptr, "i1 %s %s %s\n", total_duration_char,chord_duration_char,root_note_char);
			} 
			//print instr 2 (chords)
			fprintf(file_ptr, "i2 %s %s %s\n", total_duration_char,chord_duration_char,frequency_char);
		}
		//print instr 3 (melody)
		mel_total_dur = total_duration;
		sprintf(mel_note_dur, "%f", (chord_duration[cur_chord_dur_index]/2));
		for(int i = 0; i < 2; i++)
		{
			sprintf(mel_total_duration_char, "%f", mel_total_dur);
			melody_index = rand() % 4;//0 - 3
			mel_freq = (chord_tone[melody_index] * 2);
			sprintf(mel_frequency_char, "%f", mel_freq);
			fprintf(file_ptr, "i3 %s %s %s\n", mel_total_duration_char, mel_note_dur, mel_frequency_char);
			mel_total_dur = mel_total_dur + (chord_duration[cur_chord_dur_index]/2);
		}

		total_duration = total_duration + chord_duration[cur_chord_dur_index];
		//get next chord degree
		current_chord_degree = get_next_chord(&current_chord_degree);
	}
	//footer
	fprintf(file_ptr, "\n%s\n", csdfooter);
	fclose(file_ptr);
}
