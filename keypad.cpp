#include <cstdio>
#undef __ARM_FP

#include "mbed.h"
#include "lcd.h"	
#include "keypad.h"	
#define DISPLAY_BAR_MASK 0x000000FF 	//PORT C0: PC_7 - PC_0
#define DISPLAY_BAR_RESET 0x00000000 

AnalogIn moistureSensor(PA_0);


DigitalOut waterPump(PA_5);
PwmOut Motor1(PA_6);
PwmOut Motor2(PA_7);
//Declare the GPIO pins for the Switch as DigitalIn type

BusOut leds(PB_0);  
BusOut led2(PB_1);

PortOut displayBarPort(PortC, DISPLAY_BAR_MASK);


char no = 0;

unsigned char key, outChar;
unsigned char status = '0';
char Message1 [ ] = "Status:             ";
char Message2 [ ] = "Mode 1              ";	  
char Message3 [ ] = "Mode 2              ";	  
char Message4 [ ] = "Invalid. Loser.     ";  
Ticker pumpTicker;

char bruh;
// Function to read moisture sensor values

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

// Function to set the first servo motor forward
void set_servo1_forward() {
    Motor1.period(0.01f);   // 20ms period (standard for servos)
    Motor1.write(0.40f);    // 10% duty cycle (forward position)
    printf("Servo 1 set to forward position\n");
}

// Function to set the first servo motor backward
void set_servo1_backward() {
    Motor1.period(0.01f);   // 20ms period (standard for servos)
    Motor1.write(0.20f);    // 5% duty cycle (backward position)
    printf("Servo 1 set to backward position\n");
}

// Function to set the second servo motor forward
void set_servo2_forward() {
    Motor2.period(0.02f);   // 20ms period (standard for servos)
    Motor2.write(0.10f);    // 10% duty cycle (forward position)
    printf("Servo 2 set to forward position\n");
}

// Function to set the second servo motor backward
void set_servo2_backward() {
    Motor2.period(0.02f);   // 20ms period (standard for servos)
    Motor2.write(0.05f);    // 5% duty cycle (backward position)
    printf("Servo 2 set to backward position\n");
}

// ---- Main Program ---------------------------------------------------------------
int main( )
{
    

	int i;
	lcd_init();	
    float moistureValue = 0.0;  
    
    char status;					// Initialise LCD module
    lcd_write_cmd(0x80); // Move cursor to line 1 position 1
    for (i = 0; i < 20; i++) //for 20 char LCD module
    {
        outChar = Message1[i];
        lcd_write_data(outChar); // write character data to LCD
    }  
    while(true)
        {
            Timer timer;
            status = getkey();
            //display the PIN keyed in on LCD
            //lcd_write_data(status);
            //lcd_write_cmd(0x01);
            lcd_write_cmd(0xC0); // Move cursor to line 2 position 1

            moistureValue = moistureSensor.read();
            printf("Moisture: %.2f%%\n", moistureValue);
            float moisturePercentage = moistureValue * 100.0;
            printf("Moisture: %.2f%%\n", moisturePercentage);


            

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
                timer.start();
                while(timer.read()<= 4)
                    set_servo1_forward();

                
                if (timer.read() == 4){
                        timer.stop();
 
                }
                timer.reset();
    

            
            }
            else if (status == '2')
            {
                for (i = 0; i < 20; i++)		//for 20 number
                { 
                    outChar = Message3[i];
                    lcd_write_data(outChar); 		//display on LCD
                    thread_sleep_for(50);
                    
                }
                 timer.start();
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

            if (no == 1){
                leds = 0;
                led2 = 1;
                 while(timer.read()<= 2){
                    set_servo1_forward();
                    set_servo2_forward();
                 }
                timer.stop();
                if (timer.read()>2){
                    Motor1.suspend();
                    Motor2.suspend();
                }
                timer.reset();
             }
            else if (no == 2)
            {
                leds = 0;
                led2 = 1;
                while(timer.read()<= 2){
                    set_servo1_backward();
                    set_servo2_backward();
                }
                timer.stop();
                if (timer.read()>2){
                    Motor1.suspend();
                    Motor2.suspend();
                }
                timer.reset();
            }
        }
}
