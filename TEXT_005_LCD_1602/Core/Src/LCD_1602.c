/*
 * LCD_1602.c
 *
 *  Created on: Nov 21, 2024
 *      Author: user
 *	Function : 1602 LCD device control;
 */

//RS = 0 command, RS = 1 data
extern I2C_HandleTypeDef *hi2c;


#define I2C_ADDR 0x4E	//0x27 << 1 same

void LCD_command(char cmd)	//command bit : abcd efgh
{
	char n1, n2, dd[4];
	n1 = cmd & 0xf0;		//mask use upper nibble, abcd 0000
	n2 = (cmd & 0x0f) << 4;		//mask use cmd << 4 same , lower nibble to upper, efgh 0000
	dd[0] = n1 | 0x0c;
	dd[1] = n1 | 0x08;
	dd[2] = n2 | 0x0c;
	dd[3] = n2 | 0x08;
	HAL_I2C_Master_Transmit(hi2c, I2C_ADDR, dd, 4, 10);
}
void LCD_init()
{

}
void LCD_data(char ch)
{

}
void LCD_init()
{
	LCD_command(0x01);	//screen clear  0000_0001
	LCD_command(0x02);	//cursor return 0000_001*
	LCD_command(0x06);
	LCD_command(0x0f);
}
