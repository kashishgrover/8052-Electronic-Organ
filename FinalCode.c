#include<at89x52.h>

#define COL P2
#define ROW P1

__sbit __at (0xB0) SPEAKER; // P3.0

__sbit __at (0xB5) RS; // P3.5
__sbit __at (0xB6) RW; // P3.6
__sbit __at (0xB7) EN; // P3.7

__sbit __at (0x80) D0;	// P0_0
__sbit __at (0x81) D1; 	// P0_1
__sbit __at (0x82) D2;	// P0_2
__sbit __at (0x83) D3;	// P0_3
__sbit __at (0x84) D4;	// P0_4
__sbit __at (0x85) D5; 	// P0_5
__sbit __at (0x86) D6;	// P0_6
__sbit __at (0x87) D7;	// P0_7


int count,tone;
unsigned char colloc;
int col,row,flag=0;

int keypad[4][4] = {{76,72,68,64},
		    {61,57,54,51},
		    {48,45,43,41},
		    {38,36,34,32}};

const char note[5][5][7] = {{"C4 ","C#4","D4 ","D4 ","D#4"},
	    	            {"E4 ","F4 ","F#4","F#4","G4 "},
		            {"G#4","A4 ","A#4","A#4","B4 "},
		            {"C5 ","C#5","D5 ","D5 ","D#5"}};

void Lcd_Delay(int);
void Lcd8_Port(char);
void Lcd8_Cmd(char);
void Lcd8_Clear();
void Lcd8_Set_Cursor(char a, char b);
void Lcd8_Init();
void Lcd8_Write_Char(char a);
void Lcd8_Write_String(char *a);
void MSDelay(int value);

//Timer1 initialization routine
void init_timer()
{
	ET1=1;
	TMOD=0x20;
	TH1=210;
	TR1=1;
	EA=1;
}

void timer1() __interrupt 3
{
	count++;
	if(count==tone)
	{
		count=0;
		SPEAKER^=1;
	}
	if(count==150)
		count=0;
}


void main()
{
 count=0;
 SPEAKER=1;
 init_timer();
 Lcd8_Init();
 Lcd8_Clear();
 Lcd8_Set_Cursor(1,0);
 Lcd8_Write_String(" KASHISH GROVER ");
 Lcd_Delay(5000);
 Lcd8_Set_Cursor(2,0);
 Lcd8_Write_String(" 130911410 IT-A ");
 Lcd_Delay(5000);
 Lcd8_Clear();
 Lcd8_Set_Cursor(1,4);
 Lcd8_Write_String("Note: ");
 
 COL=0xFF;

 while(1)
 {
   while(1)
   {
   	if(flag==1)
   	{
		Lcd8_Set_Cursor(1,10);
		Lcd8_Write_String(note[row][col]);
		Lcd_Delay(200);
   	}
	
	ROW=0XFE;
	colloc=COL;
	colloc&=0x0F;
	if(colloc!=0x0F)
	{
		TR1=1;
		row=0;
		break;
	}
	ROW=0XFD;
	colloc=COL;
	colloc&=0x0F;
	if(colloc!=0x0F)
	{
		TR1=1;
		row=1;
		break;
	}

	ROW=0XFB;
	colloc=COL;
	colloc&=0x0F;
	if(colloc!=0x0F)
	{
		TR1=1;
		row=2;
		break;
	}
	ROW=0XF7;
	colloc=COL;
	colloc&=0x0F;
	if(colloc!=0x0F)
	{
		TR1=1;
		row=3;
		break;
	}
    }

	if(colloc==0x0E)
		{
			col=0;
			tone = keypad[row][0];
			flag=1;
		}
	else if(colloc==0x0D)
		{
			col=1;
			tone = keypad[row][1];
			flag=1;
		}
	else if(colloc==0x0B)
		{
			col=3;
			tone = keypad[row][2];
			flag=1;
		}
	else if(colloc==0x07)
		{
			col=4;
			tone = keypad[row][3];
			flag=1;
		}
	else			//No key is pressed
	{
		SPEAKER=0;
		count=0;
		TR1=0;
	}

 }
}

void Lcd_Delay(int a)
{
    int j;
    int i;
    for(i=0;i<a;i++)
    {
        for(j=0;j<10;j++)
        {
        }
    }
}

//LCD 8 Bit Interfacing Functions
void Lcd8_Port(char a)
{
	if(a & 1)
		D0 = 1;
	else
		D0 = 0;

	if(a & 2)
		D1 = 1;
	else
		D1 = 0;

	if(a & 4)
		D2 = 1;
	else
		D2 = 0;

	if(a & 8)
		D3 = 1;
	else
		D3 = 0;

	if(a & 16)
		D4 = 1;
	else
		D4 = 0;

	if(a & 32)
		D5 = 1;
	else
		D5 = 0;

	if(a & 64)
		D6 = 1;
	else
		D6 = 0;

	if(a & 128)
		D7 = 1;
	else
		D7 = 0;
}
void Lcd8_Cmd(char a)
{ 
  RS = 0;             // => RS = 0
  Lcd8_Port(a);             //Data transfer
  EN  = 1;             // => E = 1
  Lcd_Delay(5);
  EN  = 0;             // => E = 0
}

void Lcd8_Clear()
{
	  Lcd8_Cmd(1);
}

void Lcd8_Set_Cursor(char a, char b)
{
	if(a == 1)
	  	Lcd8_Cmd(0x80 + b);
	else if(a == 2)
		Lcd8_Cmd(0xC0 + b);
}

void Lcd8_Init()
{
  Lcd8_Port(0x00);
  RS = 0;
  Lcd_Delay(200);
  ///////////// Reset process from datasheet /////////
  Lcd8_Cmd(0x30);
	Lcd_Delay(50);
  Lcd8_Cmd(0x30);
	Lcd_Delay(110);
  Lcd8_Cmd(0x30);
  /////////////////////////////////////////////////////
  Lcd8_Cmd(0x38);    //function set
  	Lcd_Delay(50);
  Lcd8_Cmd(0x0C);    //display on,cursor off,blink off
  	Lcd_Delay(50);
  Lcd8_Cmd(0x01);    //clear display
  	Lcd_Delay(1000);
  Lcd8_Cmd(0x06);    //entry mode, set increment
  	Lcd_Delay(50);
}

void Lcd8_Write_Char(char a)
{
   RS = 1;             // => RS = 1
   Lcd8_Port(a);             //Data transfer
   EN  = 1;             // => E = 1
   Lcd_Delay(5);
   EN  = 0;             // => E = 04
}

void Lcd8_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	 Lcd8_Write_Char(a[i]);
}


void MSDelay(int value)
{
    int x;
    int y;
    for(x=0;x<1275;x++)
    {
        for(y=0;y<value;y++)
        {
        }
    }
}

