#include "chordGen.h"

ChordGen::ChordGen(){
	int *amp;
	selWave:
	cout<<"Plese select a tone for your melody. 1. sine wave 2. Sawtooth 3. Square 4. Pulse"<<endl;
	tableSel = new int;
	cin>>*tableSel;
	switch(*tableSel)
	{
		case 1:
			cout<<"You selected sine wave. "<<endl;
			break;
		case 2:
			cout<<"You selected Sawtooth. "<<endl;
			break;
		case 3:
			cout<<"You selected Square. "<<endl;
			break;
		case 4:
			cout<<"You selected Pulse. "<<endl;
			break;
		default:
			delete tableSel;
			cinClear();
			cout<<"Invalid value, please select between 1, 2, 3, 4!"<<endl;
			goto selWave;
		}
	ampSelect:
	cinClear();
	cout<<"What's the amplitude you want? type a value from 0 to 100."<<endl;
	amp = new int;
	cin>>*amp;
	if(*amp >100 || *amp < 0 || cin.fail()){
			cout<<"please type a valid value, from 0 to 100!"<<" The value you typed was: "<<*amp<<endl;
			delete amp;
			goto ampSelect;
	}else{
		cout << "The amp you choosed is: "<<*amp<<endl;
		*amp = *(amp)*120;
		csdAmp = to_string(*amp);
		delete amp;
	}

	instr = "\ninstr 1\n asig	  oscil  " + this->csdAmp + ", p4" + " , "
			+ "\t" + to_string(*tableSel) +"\n outs  asig,asig\n" + "\nendin\n";

BPM_SELECT:
	cout<<"What BPM do you want? type a number in the range between 30 to 300."<<endl;
	cin>>bpm;
	cinClear();
	if(bpm > 300 || bpm < 30){
		cout<<"Please type a BPM in the range between 30 to 300."<<endl;
		goto BPM_SELECT;
	}

	//fileSel:
	//string *fileName = new string;
	string fileName;
	cout<<"Please type the name of your melody file (with no .txt extension): "<<endl;
	cin>>fileName;
	fileName = "./melody/" + fileName + ".txt";
	cout<<"you typed: "<<fileName<<endl;
	cinClear();
	inFile.open(fileName);

	//delete fileName;

	if(inFile.fail()){
		cout<<"Error reading file, Go back and check out you typed the correct file name (no extension) and contents in the melody file."<<endl;
		inFile.close();
		exit(1);
		//goto fileSel;
	}

	char c,prec;
	while(inFile.get(c)){
		if(!isNum(c)){
			cout<<"All members in the melody file must be numbers! Go back and check out contents in your melody file."<<endl;
			exit(1);
			/*inFile.close();
			total = 0;
			goto fileSel;*/
		}
		if(c == '\t' || c == '\n' || c == ' '){
			if(prec == '\t' || prec == '\n' || prec == ' '){
				--total;
			}
			total++;
		}
		prec = c;
	}
	total = ++total;
	//check if total can be devided by 2
	if(total%2 != 0){
		cout<<"values in the melody file must be a duration and pitch pair! Go back and check out contents in your melody file (There can't be space,tab or new line in the beginning and end of the file)"<<endl;
		exit(1);
		//goto fileSel;
	}

	//go back to the beginning of file after eof caused by inFile.get()
	inFile.clear();
	inFile.seekg(inFile.beg);

	values = new float[total];//float[5] will return a reference address with size of 5 floats

	int i = 0,intTest; 
	float floatTest;
	while(!inFile.eof()){
			inFile>>values[i];
			i++;
		}

	dur = new float[total/2];
	pitch = new float[total/2];

	int duri = 0,pitchi = 0;
	
	for(int i = 0;i < total;i++){
		if(i%2 == 0)//it's a odd number, can be devided by 2 (duration)
		{
			if(values[i]<0 || values[i] > 4){
				cout<<"The notes duration of your melody file must between greater than 0 and smaller than 4! Go back and check out contents in your melody file."<<endl;
				exit(1);
				//releaseValues();
				//goto selWave;
			}
			dur[duri]=(60/bpm)*values[i];
			duri++;
		}
		else if(i%2 == 1)//pitch
		{		

switch ((int)values[i]){
 case 1: pitch[pitchi] = 130.81; break; case 2: pitch[pitchi] = 138.59; break; case 3: pitch[pitchi] = 146.83; break; case 4: pitch[pitchi] = 155.56; break; case 5: pitch[pitchi] = 164.81; break; 
 case 6: pitch[pitchi] = 174.61; break; case 7: pitch[pitchi] = 185.00; break; case 8: pitch[pitchi] = 196; break; case 9: pitch[pitchi] = 207.65; break; case 10: pitch[pitchi] = 220.00; break; 
 case 11: pitch[pitchi] = 233.08; break; case 12: pitch[pitchi] = 246.94; break; 

 case 13: pitch[pitchi] = 261.63; break; case 14: pitch[pitchi] = 277.18; break; case 15: pitch[pitchi] = 293.66; break; case 16: pitch[pitchi] = 311.13; break; case 17: pitch[pitchi] = 329.63; break; 
 case 18: pitch[pitchi] = 349.23; break; case 19: pitch[pitchi] = 369.99; break; case 20: pitch[pitchi] = 392; break; case 21: pitch[pitchi] = 415.30; break; case 22: pitch[pitchi] = 440.00; break; 
 case 23: pitch[pitchi] = 466.16; break; case 24: pitch[pitchi] = 493.88; break; 

 case 25: pitch[pitchi] = 523.25; break; case 26: pitch[pitchi] = 554.37; break; case 27: pitch[pitchi] = 587.33; break; case 28: pitch[pitchi] = 622.25; break; case 29: pitch[pitchi] = 659.26; break; 
 case 30: pitch[pitchi] = 698.46; break; case 31: pitch[pitchi] = 739.99; break; case 32: pitch[pitchi] = 783.99; break; case 33: pitch[pitchi] = 830.61; break; case 34: pitch[pitchi] = 880.00; break; 
 case 35: pitch[pitchi] = 932.33; break; case 36: pitch[pitchi] = 987.77; break; 
 	default:
 		cout<<"The pitch of your melody file must between 1 to 36 as intergers! Go back and check out contents in your melody file."<<endl;
				//releaseValues();
				exit(1);
				//goto selWave;
		}
			pitchi++;
		}
		else
			cout<<"There's a problem"<<endl;
	}

	total = total/2;
	for(int i = 0; i < total; ++i){
		score = score + "\ni1 " + to_string(startTime) + "\t" + to_string(dur[i]) + "\t" + to_string(pitch[i]) + "\n";
		startTime = startTime + dur[i];
	}

	cout<<"Total number is: "<<total<<endl;

}

ChordGen::~ChordGen(){
	//do I need to deallocate every single pointer? Or only this? What should I write in descructor pointer?
}

void ChordGen::releaseValues(){
	//if either 3 of them point at sth
		if(values) delete values;
		if(dur) delete dur;
		if(pitch) delete pitch;
}

void ChordGen::cinClear(){
	cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool ChordGen::isNum(char c){
	if(c == '0' || c == '1'|| c == '2' || c == '3' || c == '4'|| c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == ' ' || c == '\n' || c == '\t'|| c == '.'){
		return true;
	}else{
		return false;
	}
}

void ChordGen::writeToFile(){
		string fileName = "./csd/melodyGen.csd";
		file.open(fileName);
		file<<csdHeader+instr+csdMiddle+ftable+score+csdFooter;
		file.close();
}

const string ChordGen::ftable = "\nf1 0 16384 10 1                                          ; Sine\nf2 0 16384 10 1 0.5 0.3 0.25 0.2 0.167 0.14 0.125 .111   ; Sawtooth\nf3 0 16384 10 1 0   0.3 0    0.2 0     0.14 0     .111   ; Square\nf4 0 16384 10 1 1   1   1    0.7 0.5   0.3  0.1          ; Pulse\n";
