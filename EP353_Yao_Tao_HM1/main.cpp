#include "writeCsd.h"

int main(int argc,char* argv[]){
		int *amp;
		string strVoicing,continune;
		//command line to output the .csd file
		string commandStr = "csound -odac ./sine.csd";
		//convert it to an array of chars since system(const char*), this char* can NOT be changed 
		const char *command = commandStr.c_str();

		program:
		cout<<"Please tell me what amplitude do you want? from, range from 0 to 15"<<endl;
		amp = new int;
		cin>>*amp;
		if(*amp >15 || *amp < 0 || cin.fail()){
			delete amp;
			cin.clear();//cin itself doesn't remove the new line char (\n) in the stream after user hit enter, use getline(cin, var) is another solution
			//this will prevent the infinit loop if typing a bad value (e.g. another data type such as a string of chars)
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout<<"please type a valid value, from 0 to 15!"<<" The value you typed was: "<<*amp<<endl;
			goto program;
		}
		*amp = (*amp)*1000;
		cout<<"Please tell me what voicing you want to play? type the either C_major or G_dominant"<<endl;
		cin>>strVoicing;
		CsoundFile *csoundFile = new CsoundFile(*amp,strVoicing);
		if(csoundFile->validParam == 1){
			delete csoundFile;
			cout<<"invalid voicing.\n";
			goto program;
		}
		csoundFile->writeToFile();
		delete csoundFile;
		delete amp;
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