/*
 * LCD_1602.c
 *
 *  Created on: Nov 21, 2024
 *      Author: user
 *	Function : 1602 LCD device control;
 */

//RS = 0 command, RS = 1 data
//include "C:\Users\user\STM32Cube\Repository\STM32Cube_FW_F4_V1.28.1\Drivers\STM32F4xx_HAL_Driver\Inc\stm32f4xx_hal_i2c.h"

#include "main.h"

#define I2C_ADDR 0x4E	//0x27 << 1 same
I2C_HandleTypeDef *hi2c = NULL;

int i2c_init(I2C_HandleTypeDef *p)
{
	hi2c = p;
}

int i2c_scan()
{
	if(hi2c == NULL) return;
	for(int addr = 0; addr < 128; addr++)
	{
		if(HAL_I2C_IsDeviceReady(hi2c, addr, 1, 10) == HAL_OK)
		{
			printf("  %02x ", addr);
		}
		else
		{
			printf("  ,  ");
		}
		if((addr+1) % 16 == 0) printf("\r\n");
	}
}

void LCD_command(char cmd)	//command bit : abcd efgh
{
	char n1, n2, n3, n4, dd[4];
	n1 = cmd & 0xf0;		   		 	//mask use upper nibble, abcd 0000
	n2 = (cmd & 0x0f) << 4;			 	//mask use cmd << 4 same , lower nibble to upper, efgh 0000
	n3 = (1<<3)| (1<<2) | 0 | 0; 		//RW|EN_1|NC|RS 0x0c
	n4 = (1<<3)|    0 	| 0 | 0; 		//RW|EN_0|NC|RS 0x08
	dd[0] = n1 | n3;
	dd[1] = n1 | n4;
	dd[2] = n2 | n3;
	dd[3] = n2 | n4;
	HAL_I2C_Master_Transmit(hi2c, I2C_ADDR, dd, 4, 10);
}

void LCD_data(char ch)
{
	char n1, n2, n3, n4, dd[4];
		n1 = ch & 0xf0;		   		 	//mask use upper nibble, abcd 0000
		n2 = (ch & 0x0f) << 4;			 	//mask use cmd << 4 same , lower nibble to upper, efgh 0000
		n3 = (1<<3)| (1<<2) | 0 | (1 << 0); 		//RW|EN_1|NC|RS 0x0d
		n4 = (1<<3)|    0 	| 0 | (1 << 0); 		//RW|EN_0|NC|RS 0x09
		dd[0] = n1 | n3;
		dd[1] = n1 | n4;
		dd[2] = n2 | n3;
		dd[3] = n2 | n4;
		HAL_I2C_Master_Transmit(hi2c, I2C_ADDR, dd, 4, 10);
}

void LCD_init()
{
	LCD_command(0x01);	//screen clear  0000_0001
	LCD_command(0x02);	//cursor return 0000_001*
	LCD_command(0x06);
	LCD_command(0x0f);
}

void LCD_print(char *str)
{
	while(*str) LCD_data(*str++);

}
void LCD_printEx(char *str, int in)
{
	if(in == 0) LCD_command(0x80);
	if(in == 1) LCD_command(0xc0);
	LCD_print(str);
}
int ln2 = 0;		// current line
char sbuf[20];		// 2nd line string;
void LCD_printEx2(char *str)
{
	if(ln2 == 0)
		{
			LCD_command(0x80);
			ln2 ++;
		}
	else if(ln2 == 1)
		{
			LCD_command(0x80);
			LCD_print(sbuf);

			LCD_command(0xc0);
			strcpy(sbuf, str);
		}
			LCD_print(str);
}
void LCD_test()
{
	LCD_data('H');
	LCD_data('E');
	LCD_data('L');
	LCD_data('L');
	LCD_data('O');
}

