/*
	You'll be able to choose let this simple command-line based application to write between 2 different .csd file through command-line argument.
	Q1: will garbage be cleaned if I forgot to deallocate some garbage in the heap after I quit the program?
	Q2: Talk about undefined behaviour (pointer deallocation and reuse to print the value in that memory)
*/
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>//C libraries for C++ is acceptable

using namespace std;

class CsoundFile{
public:
	//if you want to define (not declare) class member functions inside of class, don't forget to add {} after functionName()! 
	short validParam;//0 = parameter is valid, 1 means invalid
	//Constructors
	CsoundFile(){};
	CsoundFile(int amp,string checkVoicing){
		this->csdAmp = to_string(amp);
		this->instr = "\ninstr 1\n asig	  oscil  " + this->csdAmp + ", p4"
		+ "\n outs  asig,asig\n" + "\nendin\n";
		if(checkVoicing == "C_major" || checkVoicing == "C_Major"){
		this->score = "i1 0 2 " + to_string(voicing[0][0]) + "\n" + "i1 0 2 " + to_string(voicing[0][1]) + "\n" + "i1 0 2 " + to_string(voicing[0][2]) + "\n" + "i1 0 2 " + to_string(voicing[0][3]) + "\n";
		this->validParam = 0;
		}else if(checkVoicing == "G_dominant" || checkVoicing == "G_Dominant"){
		this->score = "i1 0 2 " + to_string(voicing[1][0]) + "\n" + "i1 0 2 " + to_string(voicing[1][1]) + "\n" + "i1 0 2 " + to_string(voicing[1][2]) + "\n" + "i1 0 2 " + to_string(voicing[1][3]) + "\n";
		this->validParam = 0;
		}else{
		std::cout<<"Please type a valid voicing name, either \"C_major\" or \"G_dominant\".\n"<<endl;
		this->validParam = 1;
		}
	}
	//destructor
	~CsoundFile(){};

	//passing the name by value (copy)
	void writeToFile(){
		string fileName = "sine.csd";
		this->file.open(fileName);
		this->file<<csdHeader+instr+csdMiddle+this->score+csdFooter;
		this->file.close();
	}

private:
	//initializer for static const member inside class definition is only allowed with integral and enum types!!!
	ofstream file;
	string csdAmp;
	string csdHeader = "<CsoundSynthesizer>\n<CsOptions>\n</CsOptions>\n<CsInstruments>\nsr = 44100\nkr = 4410\nksmps = 10\nnchnls = 2\n";
	string instr;
	int voicing[2][4] = {{262,330,392,494},{392,494,587,698}};
	string score;
	string csdMiddle = "\n\n</CsInstruments>\n<CsScore>\n\n";
	string csdFooter = "\n\n</CsScore>\n</CsoundSynthesizer>\n";
};

	int main(int argc,char* argv[]){
		int amp;
		string strVoicing,continune;

		program:
		cout<<"Please tell me what amplitude do you want? from, range from 0 to 15"<<endl;
		cin>>amp;
		if(amp >15 || amp < 0){
			cout<<"please type a valid value, from 0 to 15!"<<endl;
			goto program;
		}
		amp = amp*1000;
		cout<<"Please tell me what voicing you want to play? type the either C_major or G_dominant"<<endl;
		cin>>strVoicing;
		CsoundFile *csoundFile = new CsoundFile(amp,strVoicing);
		if(csoundFile->validParam == 1){
			delete csoundFile;
			cout<<"invalid voicing.\n";
			goto program;
		}
		csoundFile->writeToFile();
		delete csoundFile;
		//command line to output the .csd file
		string commandStr = "csound -odac ./sine.csd";
		//convert it to an array of chars since system(const char*)
		const char *command = commandStr.c_str();
		system(command);
		
	continuing:
		cout<<"Wanna continune to play? type Yes or No."<<endl;
		cin>>continune;
		if(continune == "Yes" || continune == "yes"){
			goto program;
		}else if(continune == "No" || continune == "no"){
			cout<<"Bye!\n"<<endl;
		}else{
			cout<<"Please type either Yes or No."<<endl;
			goto continuing;
		}
		return 0;	
	}