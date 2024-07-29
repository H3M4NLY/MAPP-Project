// LCDKeypad.cpp
// Program to test LCD.
// The LCD display with two lines, 20 characters each.
// PORT A1: PA_15 : PA_8
// There are three control lines (PA_14:PA_12) and four data lines (PA_11 : PA_8).
// PA_14 - RS=0 Data represents Command, RS=1 Data represents Character
// PA_13 - RW=0 Writing into the LCD module
// PA_12 - E =1 Data is latched into LCD module during low to hight transition  
#undef __ARM_FP

#include "mbed.h"
#include "lcd.h"	
#include "keypad.h"	

unsigned char key, outChar;
/*unsigned char status[] = {1, 2, 3};*/
unsigned char status = '0';
char Message1 [ ] = "Status:             ";
char Message2 [ ] = "YOU PRESSED ONE     ";	  // Defining a 20 char string
char Message3 [ ] = "FUCKING TWO         ";
char Message4 [ ] = "AINT AVAILABLE MOFO!";

// ---- Main Program ---------------------------------------------------------------
int main( )
{
	printf("This is MAPP LCDKeypad running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

	int i;
	lcd_init();	
    char status;					// Initialise LCD module
    lcd_write_cmd(0x80); // Move cursor to line 1 position 1
    for (i = 0; i < 20; i++) //for 20 char LCD module
    {
        outChar = Message1[i];
        lcd_write_data(outChar); // write character data to LCD
    }  


	while(1)
	{
        status = getkey();
        //display the PIN keyed in on LCD
        //lcd_write_data(status);
        //lcd_write_cmd(0x01);
        lcd_write_cmd(0xC0); // Move cursor to line 2 position 1

        if (status == '1')
        {
            for (i = 0; i < 20; i++)		//for 20 char LCD module
            {
                outChar = Message2[i];
                lcd_write_data(outChar); 	// write character data to LCD
            }
        }
        else if (status == '2')
        {
            for (i = 0; i < 20; i++)		//for 20 number
            { 
                outChar = Message3[i];
                lcd_write_data(outChar); 		//display on LCD
                
            }
           		
        }
        else 
        {
            for (i = 0; i < 20; i++)		//for 20 number
            { 
                outChar = Message4[i];
                lcd_write_data(outChar);

           }  
         }
	}
}




