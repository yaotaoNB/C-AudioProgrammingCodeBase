#ifndef _WRITECSD_H
#define _WRITECSD_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
class CsoundFile{
public:
	//if you want to define (not declare) class member functions inside of class, don't forget to add {} after functionName()! 
	short validParam;//0 = parameter is valid, 1 means invalid
	//Constructors
	CsoundFile();
	
	CsoundFile(int,string);
	//destructor
	
	~CsoundFile();

	//passing the name by value (copy)
	void writeToFile();

private:
	ofstream file;
	string csdAmp;
	string instr;
	string score;
	/*static const*/ int voicing[2][4];
	static const string csdHeader;
	static const string csdMiddle;
	static const string csdFooter;
};

#endif
