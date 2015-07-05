//during call to displayString, set the line[1 or 2] to the string being passed
//then use a search method to figure out which char's need to change and change them

#include "Arduino.h"
#include "moLCDlib.h"

//#define enbl 13
//#define srl 11
//#define clk 8
//#define LCDpow 9


String line1,line2;

moLCD::moLCD(int e, int s, int c, int p){
	this->enbl = e;
	this->srl = s;
	this->clk = c;
	this->LCDpow = p;
	
	pinMode(this->enbl, OUTPUT);
	pinMode(this->srl, OUTPUT);
	pinMode(this->clk, OUTPUT);
	pinMode(this->LCDpow, OUTPUT);
	  
	  
	digitalWrite(this->enbl,LOW);
	digitalWrite(this->srl,LOW);
	digitalWrite(this->clk,LOW);
	digitalWrite(this->LCDpow,LOW);
	_delay_ms(1000);

	digitalWrite(this->LCDpow,HIGH);
	_delay_ms(1000);
	
	this->clearSR();
	  
	//set to 4 bit mode
	this->sendMessage(0b001000);
	//set to 8x2 display && 5x8 char size
	this->sendMessage(0b001000);
	this->sendMessage(0b100000);
	  
	//Turn on display and blink it!
	this->sendMessage(0b000000);
	this->sendMessage(0b111100);
	  
	//Set display to move forward
	this->sendMessage(0b000000);
	this->sendMessage(0b011000);  
/*
	char test[8]={
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000};

	char test1[8]={
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000};


	char test2[8]={
		0b11100,
		0b11100,
		0b11100,
		0b11100,
		0b11100,
		0b11100,
		0b11100,
		0b11100};

	char test3[8]={
		0b11110,
		0b11110,
		0b11110,
		0b11110,
		0b11110,
		0b11110,
		0b11110,
		0b11110};
	  
	  
	char test4[8]={
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111};
	  
	drawChar(0,test);
	drawChar(1,test1);
	drawChar(2,test2);
	drawChar(3,test3);
	drawChar(4,test4);
*/
	//type name
	char amot[16]="String 1";
	this->displayString(1,amot);

	//turn off cursor
	sendMessage(0);
	sendMessage(0b110000);

}

void moLCD::clock(){
  delayMicroseconds(10);
  digitalWrite(this->clk,HIGH);
  delayMicroseconds(10);
  digitalWrite(this->clk,LOW);
  delayMicroseconds(10);

  
}

void moLCD::enable(){
  delayMicroseconds(10);
  digitalWrite(this->enbl,HIGH);
  delayMicroseconds(10);
  digitalWrite(this->enbl,LOW);
  delayMicroseconds(10);
}


void moLCD::sendMessage(unsigned char msg){
  for(int i=0;i<6;i++){
    if(msg&0b100000)
      digitalWrite(this->srl,HIGH);
    else
      digitalWrite(this->srl,LOW);
    
    this->clock();
    msg=msg<<1;
  }
  this->enable();
}

void moLCD::clearSR(){
  digitalWrite(this->srl,LOW);
  for(int i=0;i<6;i++){
    this->clock();
  }
}

void moLCD::setDDRAM(int aDDy){
  this->sendMessage(((aDDy>>2)&0b111100)+0b100000);
  this->sendMessage((aDDy<<2)&0b111100);
}

    
void moLCD::displayString(int lineNum,char *msg)
{
	
  if(lineNum==1){
    int j=0;
    //go to beginning of line and fill with spaces. Then return to beginning of line.
    setDDRAM(0);
    for(int i=0;i<16;i++){
      this->sendMessage(0b001001);
      this->sendMessage(0b000001);
    }
    setDDRAM(0);
    while(msg[j]!='\0'){
      this->sendMessage(((msg[j]>>2)&0b111100)|1);
      this->sendMessage(((msg[j]<<2)&0b111100)|1);
      j++;
    }
  }
  else{
    int j=0;
    this->setDDRAM(64);
    for(int i=0;i<16;i++){
      this->sendMessage(0b001001);
      this->sendMessage(0b000001);
    }
    this->setDDRAM(64);
    while(msg[j]!='\0'){
      this->sendMessage(((msg[j]>>2)&0b111100)|1);
      this->sendMessage(((msg[j]<<2)&0b111100)|1);
      j++;
    }
  }
}

void moLCD::drawChar(int charIndex, char *drawing){

  //format the character index by choosing the correct row and
  //adding a bit in position D6 to signal CG RAM address info
  char formattedIndex = charIndex * 8 + (1<<6);
  
  //send start address for CG RAM characters
  this->sendMessage((formattedIndex>>2) & 0b111100);
  this->sendMessage((formattedIndex<<2) & 0b111100);

  //start drawing character
  for(int i=0;i<8;i++){
    this->sendMessage(((drawing[i]>>2) & 0b111100) + 1);
    this->sendMessage(((drawing[i]<<2) & 0b111100) + 1);
  }

	//go back to DDram position
	this->setDDRAM(0);
}

