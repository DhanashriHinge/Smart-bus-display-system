#include<reg51.h>
void keypad(void);
#define data P1								//data line of lcd

sbit led=P0^7;     
sbit rs=P0^1;
sbit rw=P0^2;
sbit en=P0^3;

void lcd_initi(void);
void lcd_data(unsigned char);
void lcd_cmd(unsigned char);
void delay(unsigned int);
void lcd_string(unsigned char *);
void trans(unsigned char );
unsigned char receive(void);
void string1(void);

sbit r0=P2^0;
sbit r1=P2^1;
sbit r2=P2^2;
sbit r3=P2^3;
sbit c0=P2^4;
sbit c1=P2^5;
sbit c2=P2^6;
sbit c3=P2^7;

void int0_isr(void)  interrupt 0
{
	string1();
	led=1;					//led on
	//delay(10);
}
void main ()
{
		unsigned int i;
		unsigned char c;
     led=0;   //led is off ...sinking mode
	   lcd_initi();
	// lcd_data("A");
	   EA=1;
		EX0=1; 		//enable external interrupt 0
		IT0=1;   	//falling edge  trigeering (falling edge triggering execute only once time,while low level triggering execute contineously) 
		TMOD=0x21;
		SCON=0x50;
		TH1=0xfd;		//9600 baud rate
		TR1=1; 				//start timmer
		  while(1)
    {
			i=0;
			while(i<=20)
			{
			//	led=0;    //led off
				//lcd_cmd(0x01);
				//lcd_cmd(0x80);
			c=receive();
			lcd_data(c);
			trans(c);		
			i++ ;
			}
			TR1=1;    		//stop timer
			
			//lcd_cmd(0x01);   //clear lcd
		}
}
		void lcd_initi(void)
{
	//	lcd_cmd(0x30);
		// delay(2);
		// lcd_cmd(0x30);
		// delay(1);
		// lcd_cmd(0x30);
		// delay(1);
   lcd_cmd(0x38); 	//lcd initialization
	lcd_cmd(0x80);  //1 st line 1 st position
	lcd_cmd(0x0c);   //display on cursor off)
	lcd_cmd(0x06);   //auto-increment in cursor
	lcd_cmd(0x01);		//clear display
}
void string1(void)
	{
		lcd_cmd(0x01);
		lcd_cmd(0x80);
lcd_string("bus_cancelled");
	//delay(2);
	}
void lcd_cmd(unsigned char a)
{
P1=a;
rs=0;
rw=0;
en=1;
delay(2);
en=0;
}
void lcd_data(unsigned char a)
{
P1=a;
rs=1;
rw=0;
en=1;
delay(2);
en=0;
}
//void delay(unsigned int a)
//{
	//unsigned int i,j;
	//for(i=0;i<a;i++)
	//{
	//	for(j=0;j<498;j++);
	//}
//}
	void lcd_string(unsigned char *P)
{
	unsigned int i;
	for(i=0;P[i]!='\0';i++)
	{
		lcd_data(P[i]);
	}
}
void delay(unsigned int a)
	{
		unsigned int i;
		for(i=0;i<a;i++)
		{
			//TMOD=0x21;
			TH0=0xfc;  //1 msec delay
			TL0=0x66;
			TR0=1;     //start timer
			while(TF0==0);
			TF0=0;
			TR0=0;		//stop timer
		}
	}
	void trans(unsigned char c)
	{
		SBUF=c;
		while(TI==0);
		TI=0;
	}
unsigned char receive(void)
	{
		unsigned char D;
		while(RI==0)
		{
		//	led=0;    //led off
			keypad();
		}
		RI=0;
	 D=SBUF;
		return(D);
	}
	void keypad(void)
	{
		r2=0;  		
		r1=r0=r3=1;			//  1  is pressed
	 if(c0==0)    
	  {
			lcd_cmd(0x01);
			lcd_cmd(0x80);
			lcd_string(" A1 to B1 ");
			lcd_cmd(0xc3);
			lcd_string( "11.30 AM" );
		}
//		lcd_cmd(0x01);   //clear lcd
		r2=0;  						
	 r0=r1=r3=1;    //2 is pressed
	 if(c1==0)    
	  {
			lcd_cmd(0x01);
			lcd_cmd(0x80);
			lcd_string(" A2 to B2 ");
			lcd_cmd(0xc3);
			lcd_string( "12.30 AM" );	
		}
		//lcd_cmd(0x01);   //clear lcd
		r2=0;  		
	 r0=r1=r3=1; 				//3 is pressed
	 if(c2==0)    
	  {
			lcd_cmd(0x01);
			lcd_cmd(0x80);
			lcd_string(" A3 to B3 ");
			lcd_cmd(0xc3);
			lcd_string( "1.30 AM" );
		}
	}