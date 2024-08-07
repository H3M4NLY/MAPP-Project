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

char lmao = 0;

unsigned char key, outChar;
unsigned char status = '0';
char Message1 [ ] = "Status:             ";
char Message2 [ ] = "Mode 1              ";	  
char Message3 [ ] = "Mode 2              ";	  
char Message4 [ ] = "Invalid. Loser.     ";  
Ticker pumpTicker;
Ticker moistureTicker;
Ticker readMoistureTicker;
Timer timer;

char bruh;
// Global variable to store the moisture percentage
float moisturePercentage;

// Function to simulate reading the moisture sensor
void read_moisture_sensor() {
    float moistureValue = moistureSensor.read();
    float moisturePercentage = moistureValue * 100.0;
    printf("Moisture percentage: %.2f\n", moisturePercentage);
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
void stop_servo1() {
    Motor1.period(0.02f);
    Motor1.write(0.0f);   // 0% duty cycle (Suspending servo movement)
    printf("Servo 1 stopped\n");
}

// Function to set the first servo motor forward
void set_servo1_forward() {
    stop_servo1();
    Motor1.period(0.02f);   // 20ms period (standard for servos)
    Motor1.write(0.10f);    // 10% duty cycle (forward position)
    printf("Servo 1 set to forward position\n");
    stop_servo1();
}

// Function to set the first servo motor backward
void set_servo1_backward() {
    stop_servo1();
    Motor1.period(0.02f);   // 20ms period (standard for servos)
    Motor1.write(0.05f);    // 5% duty cycle (backward position)
    printf("Servo 1 set to backward position\n");
}

void stop_servo2() {
    Motor2.period(0.02f);
    Motor2.write(0.0f);   // 0% duty cycle (Suspending servo movement)
    printf("Servo 2 stopped\n");
}


// Function to set the second servo motor forward
void set_servo2_forward() {
    stop_servo2();
    Motor2.period(0.02f);   // 20ms period (standard for servos)
    Motor2.write(0.10f);    // 10% duty cycle (forward position)
    printf("Servo 2 set to forward position\n");
    stop_servo2();
}

// Function to set the second servo motor backward
void set_servo2_backward() {
    Motor2.period(0.02f);   // 20ms period (standard for servos)
    Motor2.write(0.05f);    // 5% duty cycle (backward position)cn
    printf("Servo 2 set to backward position\n");

}

void check_moisture(){
    if (moisturePercentage >= 50 && lmao == 0){
                leds = 0;
                led2 = 1;
                 while(timer.read()<= 2){
                    set_servo1_forward();
                    set_servo2_forward();
                 }
                timer.stop();
                if (timer.read()>2){
                    stop_servo1();
                    stop_servo2();
                }
                timer.reset();
                lmao = 1;
             }
    else if (moisturePercentage <= 50 && lmao == 1)
            {
                leds = 0;
                led2 = 1;
                while(timer.read()<= 2){
                    set_servo1_backward();
                    set_servo2_backward();
                }
                timer.stop();
                if (timer.read()>2){
                    stop_servo1();
                    stop_servo2();
                }
                timer.reset();
                lmao = 0;
            }
}


// ---- Main Program ---------------------------------------------------------------
int main( )
{
    

	int i;
	lcd_init();	 

    moistureTicker.attach(&read_moisture_sensor, 5000ms);
    readMoistureTicker.attach(&check_moisture, 5000ms);

    
    
    
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
                while(timer.read() <= 2){
                    set_servo1_forward();
                    stop_servo1();
                }
                timer.stop();
                    if (timer.read() >= 2){
                        stop_servo1();

                }
                timer.stop();
                timer.reset(); 
                timer.start();  
                while(timer.read()<= 2){
                    stop_servo1();
                    set_servo1_backward();
                }
                timer.stop();
                if (timer.read() >=2){
                    stop_servo1();

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
                while(timer.read()<= 4){
                    set_servo1_backward();
                }
                timer.stop();
                if (timer.read()>4){
                    Motor1.suspend();
                }
                timer.reset();
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
        }
        if (bruh == 1){
            pumpTicker.attach(&toggle_water_pump, 5000ms);
        }

        if (bruh == 2){
            pumpTicker.attach(&toggle_water_pump, 10000ms);
        }
        
}
