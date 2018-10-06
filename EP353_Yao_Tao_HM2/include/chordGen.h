#ifndef _CHORDGEN_H
#define _CHORDGEN_H

#include "writeCsd.h"

class ChordGen:public CsoundFile {
	public:
		ChordGen();

		~ChordGen();

		//override keyword should ONLY exist as declaration in the header file!
		void writeToFile() override;

		void cinClear();

		void releaseValues();

		bool isNum(char);

	private:
		ifstream inFile;
		float *values,*dur,*pitch,startTime=0,bpm;
		int total = 0,*tableSel;
		static const string ftable;
};

#endif