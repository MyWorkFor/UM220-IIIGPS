#ifndef _LCD1602_H_
#define _LCD1602_H_

#include "reg52.h"

/*
 * IO definition
 * --------------------
 * |LCD1602   |  89C52 |
 * ---------------------
 * | RS       |   P26  |
 * ---------------------
 * | RW       |   P25  |
 * ---------------------
 * | EN       |   P27  |
 * ---------------------
*/
sbit rs	= P2^6;		
sbit wr = P2^5;
sbit lcden = P2^7;

//write command to lcd1206
void write_com(unsigned char com);  

//write data to lcd1206  
void write_data(unsigned char dat);   

//display the string
 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) ;  

//display the alphabetic
 void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) ;

//initial lcd
 void lcd_init(void);

#endif