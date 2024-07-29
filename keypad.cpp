// LCDKeypad.cpp
// Program to test LCD.
// The LCD display with two lines, 20 characters each.
// PORT A1: PA_15 : PA_8
// There are three control lines (PA_14:PA_12) and four data lines (PA_11 : PA_8).
// PA_14 - RS=0 Data represents Command, RS=1 Data represents Character
// PA_13 - RW=0 Writing into the LCD module
// PA_12 - E =1 Data is latched into LCD module during low to hight transition  
#include <cstdio>
#undef __ARM_FP

#include "mbed.h"
#include "lcd.h"	
#include "keypad.h"	
#define DISPLAY_BAR_MASK 0x000000FF 	//PORT C0: PC_7 - PC_0
#define DISPLAY_BAR_RESET 0x00000000 

AnalogIn moistureSensor1(PA_0);
AnalogIn moistureSensor2(PA_1);

DigitalOut waterPump(PA_5);
PwmOut servoMotor1(PA_6);
PwmOut servoMotor2(PA_7);
//Declare the GPIO pins for the Switch as DigitalIn type

BusOut leds(PB_14);  
BusOut led2(PB_15);

PortOut displayBarPort(PortC, DISPLAY_BAR_MASK);




unsigned char key, outChar;
/*unsigned char status[] = {1, 2, 3};*/
unsigned char status = '0';
char Message1 [ ] = "Status:             ";
char Message2 [ ] = "YOU PRESSED ONE     ";	  // Defining a 20 char string
char Message3 [ ] = "FUCKING TWO         ";
char Message4 [ ] = "AINT AVAILABLE MOFO!";

char bruh;
// Function to read moisture sensor values
void read_moisture_sensors(float &sensor1_value, float &sensor2_value) {
    sensor1_value = moistureSensor1.read(); // Read value from sensor 1
    sensor2_value = moistureSensor2.read(); // Read value from sensor 2
}

void control_water_pump(bool turn_on) {
    waterPump = turn_on ? 1 : 0; // Turn the pump on or off
    printf("Pump should work. \n");
}

// Function to set the angle of the first servo motor
void set_servo1_angle(float angle) {
    // Assuming the servo motor operates between 0.5ms (0 degrees) and 2.5ms (180 degrees)
    float pulse_width = 0.001 + (angle / 180.0) * 0.002; // Calculate pulse width
    servoMotor1.pulsewidth(pulse_width); // Set the pulse width
}

// Function to set the angle of the second servo motor
void set_servo2_angle(float angle) {
    // Assuming the servo motor operates between 0.5ms (0 degrees) and 2.5ms (180 degrees)
    float pulse_width = 0.001 + (angle / 180.0) * 0.002; // Calculate pulse width
    servoMotor2.pulsewidth(pulse_width); // Set the pulse width
}

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
            float sensor1_value, sensor2_value;
            read_moisture_sensors(sensor1_value, sensor2_value); // Read moisture sensor values

            // Print sensor values to the console
            printf("Moisture Sensor 1: %.2f\n", sensor1_value);
            printf("Moisture Sensor 2: %.2f\n", sensor2_value);

            // Add a delay to avoid flooding the console with messages
            thread_sleep_for(50);

            if (status == '1')
            {
                for (i = 0; i < 20; i++)		//for 20 char LCD module
                {
                    outChar = Message2[i];
                    lcd_write_data(outChar); 	// write character data to LCD
                    thread_sleep_for(50);
                }
                bruh = 1;
                control_water_pump(true);
            
            }
            else if (status == '2')
            {
                for (i = 0; i < 20; i++)		//for 20 number
                { 
                    outChar = Message3[i];
                    lcd_write_data(outChar); 		//display on LCD
                    thread_sleep_for(50);
                    
                }
                bruh = 2;
                control_water_pump(false);
                
                    
            }
            else 
            {
                for (i = 0; i < 20; i++)		//for 20 number
                { 
                    outChar = Message4[i];
                    lcd_write_data(outChar);
                    thread_sleep_for(50);
                }  
                bruh = 3;
            }
            if (bruh == 1){
                leds = 1;
                led2 = 1;
            }

            if (bruh == 2){
                leds = 0;
                led2 = 0;
            }
        }
}
        
