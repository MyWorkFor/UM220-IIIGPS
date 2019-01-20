/*
--------------------------------------------------------------------
--------------------------------------------------------------------
* lcd1602.c
* RS  RW   EN
* 16*2 display screen
* about 1 ms
--------------------------------------------------------------------
--------------------------------------------------------------------
*/
#include "lcd1602.h"
#include "reg52.h"
#include "delay.h"

//write cmd
void write_com(unsigned char com) 
{
	lcden=0;
	rs=0;
	wr=0;

	P0=com;
	delay(1);
	lcden=1;
	delay(5);
	lcden=0;

}

//write data
void write_data(unsigned char dat) 
{
	lcden=0;
	rs=1;
	wr=0;

	P0=dat;
  delay(1);

	lcden=1;
	delay(5);
	lcden=0;
}

/*
 * write the string
 * x:row
 * y;column
 * *s,the point of char
 * Eg:LCD_Write_String(0,0,"Hello")
 * Display Hello to Lcd1602 at  row 1st column 1st
 * x=0 or x=1,y too
*/
 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
 {     
 if (y == 0) 
 	{     
	 write_com(0x80 + x);    
 	}
 else 
 	{      
 	write_com(0xC0 + x);    
 	}        
 while (*s) 
 	{     
write_data( *s);     
 s ++;     
 	}
 }

//write alpah
 void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
 {     
 if (y == 0) 
 	{     
 	write_com(0x80 + x);     
 	}    
 else 
 	{     
 	write_com(0xC0 + x);     
 	}        
 write_data( Data);  
 }

//initial lcd1602
void lcd_init(void)
{							
	write_com(0x38);  //enable lcd 
	write_com(0x0c);  //not use cursor
	write_com(0x01);  //clear screen
	write_com(0x06);  //write DDRAM,and point +1
	write_com(0x0f);  //flash the cursor
}

