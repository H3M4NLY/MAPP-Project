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
PwmOut Motor1(PA_6);
PwmOut Motor2(PA_7);
//Declare the GPIO pins for the Switch as DigitalIn type

BusOut leds(PB_0);  
BusOut led2(PB_1);

PortOut displayBarPort(PortC, DISPLAY_BAR_MASK);




unsigned char key, outChar;
unsigned char status = '0';
char Message1 [ ] = "Status:             ";
char Message2 [ ] = "Mode 1              ";	  
char Message3 [ ] = "Mode 2              ";	  
char Message4 [ ] = "Invalid. Loser.     ";  
Ticker pumpTicker;

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

void toggle_water_pump() {
    static bool pump_state = false;
    pump_state = !pump_state;
    control_water_pump(pump_state);
    printf("idk \n");
}

// Function to set the angle of the first servo motor
void set_servo1_time(float time) {
    // Assuming the servo motor operates between 0.5ms (0 degrees) and 2.5ms (180 degrees)
    Motor1.period(0.000000001);
    Motor1.write(0.3f);
    thread_sleep_for(time);
    printf("should work \n");
}

// Function to set the angle of the second servo motor
void set_servo2_time(float time) {
    float period = 1;
    Motor1.period(period);
    Motor1.write(time);
    printf("should work \n");
}

// ---- Main Program ---------------------------------------------------------------
int main( )
{

	int i;
	lcd_init();	
    char status;					// Initialise LCD module
    lcd_write_cmd(0x80); // Move cursor to line 1 position 1
    for (i = 0; i < 20; i++) //for 20 char LCD module
    {
        outChar = Message1[i];
        lcd_write_data(outChar); // write character data to LCD
    }  
    while(true)
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
                set_servo1_time(4000);

            
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
                pumpTicker.attach(&toggle_water_pump, 5000ms);
            }

            if (bruh == 2){
                pumpTicker.attach(&toggle_water_pump, 10000ms);
            }

            if (sensor1_value <= 0.6){
                leds = 1;
                led2 = 0;
                set_servo1_time(0.75);
                set_servo2_time(0.75);
             }
            else 
            {
                leds = 0;
                led2 = 1;
                set_servo1_time(0);
                set_servo2_time(0);
}
}
}   
