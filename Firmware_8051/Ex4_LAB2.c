#include "reg51.h"

sbit SPK  = P3^7;	

//verify this table according to your CPU clock
code unsigned int note_table[]={
		0xF9,0x1F,	// 1 C4
		0xF9,0x82,	// 2 C#4/Db4 
		0xF9,0xDF,	// 3 D4
		0xFA,0x37,	// 4 D#4/Eb4 
		0xFA,0x8A, 	// 5 E4
		0xFA,0xD9, 	// 6 F4
		0xFB,0x23, 	// 7 F#4/Gb4 
		0xFB,0x68, 	// 8 G4
		0xFB,0xAA,	// 9 G#4/Ab4 
		0xFB,0xE9, 	// 10 A4
		0xFC,0x23,	// 11 A#4/Bb4 
		0xFC,0x5B, 	// 12 B4
		0xFC,0x8F, 	// 13 C5
		0xFC,0xC1,	// 14 C#5/Db5 
		0xFC,0xEF, 	// 15 D5
		0xFD,0x1B,	// 16 D#5/Eb5 
		0xFD,0x45, 	// 17 E5
		0xFD,0x6C, 	// 18 F5
};
unsigned char note_index;	

void init();		
void delay(unsigned int time); 

void main(void) 
{
	unsigned char i;
	init();
	while(1){
		note_index=0;
		for(i=0;i<18;i++){		
			TH0=note_table[note_index];
			TL0=note_table[note_index+1];			
			TR0=1;
			delay(50000);
			delay(50000);
			TR0=0;
			SPK=1;
			delay(30000);
			note_index+=2;			
		}
		delay(50000);
		delay(50000);
	}			
}


void init()
{
	TMOD=0x01;
	EA=1;
	ET0=1;
	TR0=0;
}

void timer0() interrupt 1
{
	TH0=note_table[note_index];
	TL0=note_table[note_index+1];
	SPK=~SPK;
}

void delay(unsigned int time)
{
	while(time--);
}