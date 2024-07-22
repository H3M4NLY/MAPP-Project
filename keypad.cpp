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
                thread_sleep_for(50);
            }
            bruh = 1;
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
