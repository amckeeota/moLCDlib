#ifndef moLCDlib_h
#define moLCDlib_h

#include "Arduino.h"


class moLCD
{
	public:
		moLCD(int e,int s, int c, int p);
		void displayString(int lineNum,char *msg);
		void drawChar(int charIndex, char *drawing);
	private:
		void clock();
		void enable();
		void sendMessage(unsigned char msg);
		void clearSR();
		void setDDRAM(int aDDy);
		int enbl,srl,clk,LCDpow;
};



#endif