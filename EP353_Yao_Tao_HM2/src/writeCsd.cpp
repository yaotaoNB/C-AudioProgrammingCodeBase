/*
	Q1: will garbage be cleaned if I forgot to deallocate some garbage in the heap after I quit the program?
	Q2: Talk about undefined behaviour (pointer deallocation and reuse to print the value in that memory)
*/
#include "writeCsd.h"

	CsoundFile::CsoundFile(){};

	CsoundFile::CsoundFile(int amp,string checkVoicing)
	: voicing {{262,330,392,494},{392,494,587,698}} //g++ -std=c++14 writeCsd.cpp main.cpp -o writecsd, coz initializer list is not allowed in C++98
	{
		//you can't assign multiple values to a raw array in C++ (use std::vector or std::array), or use C++11 initialization list
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

	CsoundFile::~CsoundFile(){};

	void CsoundFile::writeToFile(){
		string fileName = "sine.csd";
		this->file.open(fileName);
		this->file<<csdHeader+instr+csdMiddle+this->score+csdFooter;
		this->file.close();
	}

	/*
		As for declaring non-static class data member, do it in constructor, coz there is no object being introduced. 
		If you have a data member such as int x;, no int object is created until you actually create an object of the type of the class.
		Unless it's a static int.
	*/
	//const int CsoundFile::voicing[2][4] = {{262,330,392,494},{392,494,587,698}};
	const string CsoundFile::csdHeader = "<CsoundSynthesizer>\n<CsOptions>\n</CsOptions>\n<CsInstruments>\nsr = 44100\nkr = 4410\nksmps = 10\nnchnls = 2\n";
	const string CsoundFile::csdMiddle = "\n\n</CsInstruments>\n<CsScore>\n\n";
	const string CsoundFile::csdFooter = "\n\n</CsScore>\n</CsoundSynthesizer>\n";
