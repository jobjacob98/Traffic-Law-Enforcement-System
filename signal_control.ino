/*
*
* Project Name: 	Traffic Law Enforcement System
* Author List: 		Ankitha Girish, Aiswarya Arun, Aishwarya LV, Job Jacob
* Filename: 		signal_control.ino
* Functions: 		void setup(), void loop()
* Global Variables:	1) red_1 of type int
*                       2) yellow_1 of type int
*                       3) green_1 of type int 
*                       4) red_2 of type int
*                       5) yellow_2 of type int
*                       6) green_2 of type int
*                       7) red_3 of type int
*                       8) yellow_3 of type int
*                       9) green_3 of type int
*                       10) red_4 of type int
*                       11) yellow_4 of type int
*                       12) green_4 of type int
*                       13) incoming_bit of type char
*                       14) prev_incoming_bit of type char
*
*/

#include<EEPROM.h> // to store the previous green signal lane onto the EEPROM of the Arduino board

// declaring pins for the first traffic signal:
int red_1 = 2;
int yellow_1 = 3;
int green_1 = 4;

// declaring pins for the second traffic signal:
int red_2 = 5;
int yellow_2 = 6;
int green_2 = 7;

// declaring pins for the third traffic signal:
int red_3 = 8;
int yellow_3 = 9;
int green_3 = 10;

// declaring pins for the fourth traffic signal:
int red_4 = A2;
int yellow_4 = A1;
int green_4 = A0;

char incoming_bit; // for incoming serial data
char prev_incoming_bit; // for controlling yellow light

/*
*
* Function Name: 	setup()
* Input: 		None
* Output: 		None
* Logic: 		This function is used to initialize the pin modes of the LEDs which represent the traffic signals in our model. It also
*                       sets the data rate as 9600bps for serial data transmission. This function will only run once, after each powerup or 
*                       reset of the Arduino board.
* Example Call:		setup(); --> from any function in the file
*
*/
void setup() {
    // configuring the specified pins to act as output of the LEDs:
    pinMode(red_1, OUTPUT);     
    pinMode(yellow_1, OUTPUT);   
    pinMode(green_1, OUTPUT); 
    pinMode(red_2, OUTPUT);     
    pinMode(yellow_2, OUTPUT);   
    pinMode(green_2, OUTPUT); 
    pinMode(red_3, OUTPUT);     
    pinMode(yellow_3, OUTPUT);   
    pinMode(green_3, OUTPUT); 
    pinMode(red_4, OUTPUT);     
    pinMode(yellow_4, OUTPUT);   
    pinMode(green_4, OUTPUT);   
    Serial.begin(9600); // opens serial port, sets data rate to 9600 bps  
}

/*
*
* Function Name: 	loop()
* Input: 		None
* Output: 		None
* Logic: 		This function runs on and on like a loop as long as the Arduino has power.
*                       First it checks if the Arduino is receiving any serial data. If yes then depending on the data, the corresponding 
*                       if statement (ie. for signal 1, signal 2, signal 3, signal 4) will run. If '1' the first signal will go green and the
*                       rest will go red, if '2' the second signal will go green and the rest will go red, if '3' the third signal will go 
*                       green and the rest will go red and if '4' the fourth signal will go green and the rest will go red. The header file 
*                       EEPROM.h is used to store the previous green signal serial data to the EEPROM so that we can blink the yellow (slow down) 
*                       LED before the signal turns from green to red.
* Example Call:		loop(); --> from any function in the file
*
*/
void loop() {    
    // reply only when you receive data:
    if(Serial.available() > 0) {  
        // read the incoming byte:
        incoming_bit = Serial.read();
        // to run the appropriate condition based on the compared density 
        if(incoming_bit == '1')
        {
            // for yellow signal
            prev_incoming_bit = EEPROM.read(1000);
            if(prev_incoming_bit == '2')
            {
                digitalWrite(yellow_2, HIGH);
                delay(2000); 
            }
            else if(prev_incoming_bit == '3')
            {
                digitalWrite(yellow_3, HIGH);
                delay(2000); 
            }
            else if(prev_incoming_bit == '4')
            {
                digitalWrite(yellow_4, HIGH);
                delay(2000); 
            }            
            EEPROM.write(1000, '1'); // writing the currrent active green signal to the memory address 1000 
            digitalWrite(red_1, LOW); 
            digitalWrite(yellow_1, LOW); 
            digitalWrite(green_1, HIGH); 
            digitalWrite(red_2, HIGH);        
            digitalWrite(yellow_2, LOW); 
            digitalWrite(green_2, LOW);
            digitalWrite(red_3, HIGH); 
            digitalWrite(yellow_3, LOW); 
            digitalWrite(green_3, LOW);
            digitalWrite(red_4, HIGH); 
            digitalWrite(yellow_4, LOW); 
            digitalWrite(green_4, LOW);                         
        }
        else if(incoming_bit == '2')
        {
            // for yellow signal
            prev_incoming_bit = EEPROM.read(1000);
            if(prev_incoming_bit == '1')
            {
                digitalWrite(yellow_1, HIGH);
                delay(2000); 
            }
            else if(prev_incoming_bit == '3')
            {
                digitalWrite(yellow_3, HIGH);
                delay(2000); 
            }
            else if(prev_incoming_bit == '4')
            {
                digitalWrite(yellow_4, HIGH);
                delay(2000); 
            }
            EEPROM.write(1000, '2'); // writing the currrent active green signal to the memory address 1000 
            digitalWrite(red_1, HIGH); 
            digitalWrite(yellow_1, LOW); 
            digitalWrite(green_1, LOW);
            digitalWrite(red_3, HIGH); 
            digitalWrite(yellow_3, LOW); 
            digitalWrite(green_3, LOW);
            digitalWrite(red_4, HIGH); 
            digitalWrite(yellow_4, LOW); 
            digitalWrite(green_4, LOW);        
            digitalWrite(red_2, LOW); 
            digitalWrite(yellow_2, LOW); 
            digitalWrite(green_2, HIGH);                       
        }
        else if(incoming_bit == '3')
        {
            // for yellow signal
            prev_incoming_bit = EEPROM.read(1000);
            if(prev_incoming_bit == '1')
            {
                digitalWrite(yellow_1, HIGH);
                delay(2000); 
            }
            else if(prev_incoming_bit == '2')
            {
                digitalWrite(yellow_2, HIGH);
                delay(2000); 
            }
            else if(prev_incoming_bit == '4')
            {
                digitalWrite(yellow_4, HIGH);
                delay(2000); 
            }
            EEPROM.write(1000, '3'); // writing the currrent active green signal to the memory address 1000
            digitalWrite(red_1, HIGH); 
            digitalWrite(yellow_1, LOW); 
            digitalWrite(green_1, LOW);
            digitalWrite(red_2, HIGH); 
            digitalWrite(yellow_2, LOW); 
            digitalWrite(green_2, LOW);
            digitalWrite(red_3, LOW); 
            digitalWrite(yellow_3, LOW); 
            digitalWrite(green_3, HIGH);            
            digitalWrite(red_4, HIGH); 
            digitalWrite(yellow_4, LOW); 
            digitalWrite(green_4, LOW);                                   
        }
        else if(incoming_bit == '4')
        {
            // for yellow signal
            prev_incoming_bit = EEPROM.read(1000);
            if(prev_incoming_bit == '1')
            {
                digitalWrite(yellow_1, HIGH);
                delay(2000); 
            }
            else if(prev_incoming_bit == '2')
            {
                digitalWrite(yellow_2, HIGH);
                delay(2000); 
            }
            else if(prev_incoming_bit == '3')
            {
                digitalWrite(yellow_3, HIGH);
                delay(2000); 
            }
            EEPROM.write(1000, '4'); // writing the currrent active green signal to the memory address 1000
            digitalWrite(red_1, HIGH); 
            digitalWrite(yellow_1, LOW); 
            digitalWrite(green_1, LOW);
            digitalWrite(red_2, HIGH); 
            digitalWrite(yellow_2, LOW); 
            digitalWrite(green_2, LOW);  
            digitalWrite(red_3, HIGH); 
            digitalWrite(yellow_3, LOW); 
            digitalWrite(green_3, LOW);  
            digitalWrite(red_4, LOW); 
            digitalWrite(yellow_4, LOW); 
            digitalWrite(green_4, HIGH);                              
        }
    }
}
