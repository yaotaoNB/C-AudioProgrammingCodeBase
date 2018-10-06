	
	#include "chordGen.h"

	int main(){
		//command line to run Csound with the csd file created by Csound
		string continune;
		string commandStr = "csound -odac ./csd/melodyGen.csd";
		const char *command = commandStr.c_str();

	program:
		//Allocate a new instance of our class in the heap, most of Csound properties will be generated with the constructor method
		//The class is inherit from the class of Homework1
		ChordGen *chordPtr = new ChordGen();
		chordPtr->writeToFile();
		system(command);
		delete chordPtr;
		
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