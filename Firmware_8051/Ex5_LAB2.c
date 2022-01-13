#include "reg51.h"
#include "string.h"

//Dinh nghia mot so chan de dieu khien den LCD
#define LCD_DATA P2
sbit EN=P0^2;
sbit RS=P0^0;
sbit RW=P0^1;
sbit SPK  = P3^7;	// speaker pin
char uart_data;
unsigned char note_index;
int i;

// Bai hat Happy New Year
unsigned int hpny[]={18,20,18,14,12,12,14,18,22,4,2,46,46,46,4,50,50,50,46,22,4,46,46,50,50,22,22,20,22};  //29 phan tu

//verify this table according to your CPU clock
code unsigned int note_table[]={
	0xEB,0x62,	//  F2
	0xFA,0x8A, 	//  E4
	0xF9,0x1F,	//  C4
	0xEF,0xA3,	//  A2
	0xD4,0x50,	//  E1
	0xF1,0x6C,	//  B2
	0xF2,0x3D,	//  C3
	0xF3,0xBE,	//  D3
	0xE4,0x7B,	//  C2
	0xF5,0x14,	//  E3
	0xF5,0xB1,	//  F3
	0xF6,0xD1,	//  G3
	0xFB,0x68, 	//  G4
	0xFA,0xD9, 	//  F4
	0xE7,0x7C,	//  D2
	0xEA,0x28,	//  E2
	0xC8,0xF4,	//  C1
	0xD6,0xC3,	//  F1
	0xED,0xA2,	//  G2
	0xDB,0x44,	//  G1
	0xE2,0xD8,	//  B1
	0xF9,0xDF,	//  D4
	0xCE,0xF8,	//  D1
	0xF8,0xB6,	//  B3
	0xDF,0x46,	//  A1
	0xF7,0xD1,	//  A3
};

unsigned char key[]={
	'F','2',	//a F2
	'E','4',	//b E4
	'C','4',	//c C4
	'A','2',	//d A2
	'E','1',	//e E1
	'B','2',	//f B2
	'C','3',	//g C3
	'D','3',	//h D3
	'C','2',	//i C2
	'E','3',	//j E3
	'F','3',	//k F3
	'G','3',	//l G3
	'G','4',	//m G4
	'F','4',	//n F4
	'D','2',	//o D2
	'E','2',	//p E2
	'C','1',	//q C1
	'F','1',	//r F1
	'G','2',	//s G2
	'G','1',	//t G1
	'B','1',	//u B1
	'D','4',	//v D4
	'D','1',	//w D1
	'B','3',	//x B3
	'A','1',	//y A1
	'A','3',	//z A3
};

//Khai bao prototype cho cac ham
void music_player_init();		
void delay(unsigned int time);
void music_node();
void Init_System();	
void LCD_init();
void LCD_Send_Command(unsigned char x);
void LCD_Write_One_Char(unsigned char c);
void LCD_Write_String(unsigned char *s);

void main()
{
	Init_System();
	LCD_init();
	music_player_init();
	delay(50000);
	LCD_Write_String(" Electric Piano");
	LCD_Send_Command(0xC0); //Chuyen con tro xuong dong thu 2
	SCON = 0x50; 			// uart in mode 1 (8 bit), REN=1
	TMOD = 0x21 ; 	// Timer 1 in mode 2, timer 0 in mode 1
  	TH1 = 0xFD; 			// 9600 Bds at 11.0592MHz 
  	TL1 = 0xFD; 			// 9600 Bds at 11.0592MHz 
  	ES = 1; 				// Enable serial interrupt
  	EA = 1; 				// Enable global interrupt
  	TR1 = 1; 				// Timer 1 run 
	
	// Phat nhac bai Happy New Year
	for(i=0;i<29;i++){
		note_index = hpny[i];
		TH0=note_table[note_index];
		TL0=note_table[note_index+1];			
		TR0=1;
		delay(30000);
		TR0=0;
		delay(500);
		SPK=1;
	}
	
	// Vong lap de chay chuong trinh
  	while(1){}
}
void Init_System()
{
	//Thiet lap LCD o che do doc
	RW=1;	
}

//Ham thuc hien gui mot lenh xuong LCD
void LCD_Send_Command(unsigned char x)
{
	LCD_DATA=x;
	RS=0; //Chon thanh ghi lenh
	RW=0; //De ghi du lieu
	EN=1;
	delay(5);
	EN=0;		  
}

void LCD_init()
{
	LCD_Send_Command(0x38); //Chon che do 8 bit, 2 hang cho LCD
	LCD_Send_Command(0x0C); //Bat hien thi, nhap nhay con tro	
	LCD_Send_Command(0x01); //Xoa man hinh	
	LCD_Send_Command(0x80); //Ve dau dong
	
}
//Ham de LCD hien thi mot ky tu
void LCD_Write_One_Char(unsigned char c)
{
	LCD_DATA=c; //Dua du lieu vao thanh ghi 
	RS=1; //Chon thanh ghi du lieu
	RW=0;
	EN=1;
	delay(5);
	EN=0;
}
//Ham de LCD hien thi mot xau
void LCD_Write_String(unsigned char *s)
{
	unsigned char length;
	length=strlen(s); //Lay do dai xau
	while(length!=0)
	{
		LCD_Write_One_Char(*s); //Ghi ra LCD gia tri duoc tro boi con tro
		s++; //Tang con tro
		length--;
	}
}

// Bat che do phat am thanh
void music_player_init()
{
	ET0=1;
	TR0=0;
}

// Phat am thanh note nhac da duoc chon
void music_node() {
	TH0=note_table[note_index];
	TL0=note_table[note_index+1];			
	TR0=1;
	SPK=1;
}


void delay(unsigned int time)
{
	while(time--);
}

// Nhan du lieu tu cong COM
void serial_IT(void) interrupt 4
{
  	if (RI == 1)
  	{ 
    		RI = 0; 			// prepare for next reception 
    		uart_data = SBUF; 	// Du lieu nhan duoc
		
		//cac phim tu a-z duoc nhan
		if(uart_data>96 && uart_data<123) {
			note_index = (uart_data-97) * 2;
			music_node();		// phat am thanh
			LCD_Write_One_Char(key[note_index]);		// Ghi ten not nhac len LCD
			LCD_Write_One_Char(key[note_index+1]);	// Ghi ten not nhac len LCD
			LCD_Send_Command(0xC0); //Chuyen con tro xuong dong thu 2
		}
		//cac phim con lai duoc nhan hoac cac phim a-z duoc nha ra
		else {
			TR0=0;	// ngat am thanh hien tai
		}
	}
  	else TI = 0; 			// if emission occur 
}

// Tao am thanh
void timer0() interrupt 1
{
	TH0=note_table[note_index];
	TL0=note_table[note_index+1];
	SPK=~SPK;
}
