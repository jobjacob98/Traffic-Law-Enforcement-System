/*
*
* Project Name: 	Traffic Law Enforcement System
* Author List: 		Ankitha Girish, Aiswarya Arun, Aishwarya LV, Job Jacob
* Filename: 		speed_of_vehicle.ino
* Functions: 		void setup(), void loop(), int data(int, int)
* Global Variables:	1) trig_pin_1 of type int
*                       2) trig_pin_2 of type int
*                       3) echo_pin_1 of type int 
*                       4) echo_pin_2 of type int
*                       5) distance_travelled of type float
*
*/

int trig_pin_1 = 3; // trigger pin of first ultrasonic sensor
int echo_pin_1 = 2; // echo pin of first ultrasonic sensor
int trig_pin_2 = 4; // trigger pin of second ultrasonic sensor
int echo_pin_2 = 5; // echo pin of second ultrasonic sensor

float distance_travelled = 40.0; // distance between the two ultrasonic sensors or the distance used to calculate the speed
float speed_of_vehicle = 0.0; // speed of the vehicle

/*
*
* Function Name: 	setup()
* Input: 		None
* Output: 		None
* Logic: 		This function is used to initialize the pin modes for the ultrasonic sensor and also to set the data rate as 9600bps 
*                       for serial data transmission. This function will only run once, after each powerup or reset of the Arduino board.
* Example Call:		setup(); --> from any function in the file
*
*/
void setup() {
  // configuring the specified pins
  pinMode(trig_pin_1, OUTPUT); 
  pinMode(echo_pin_1, INPUT);
  pinMode(trig_pin_2, OUTPUT);
  pinMode(echo_pin_2, INPUT);
  Serial.begin (9600); // opens serial port, sets data rate to 9600 bps  
}

/*
*
* Function Name: 	loop()
* Input: 		None
* Output: 		None
* Logic: 		This function runs on and on like a loop as long as the Arduino has power.
*                       This function calls the function data(int, int) to get the output from the first ultrasonic sensor and then checks if a vehicle/ 
*                       object is within 3cm of the ultrasonic sensor. If yes then it will call millis() and store its return value to time_1. 
*                       millis() is an inbuild function that returns the number of milliseconds since the Arduino board began running the current program.
*                       Then it again calls the function data(int, int) to get the output from the second ultrasonic sensor and then check if a vehicle/
*                       object is within 3cm of the ultrasonic sensor. If yes then it will call millis() and store its return value to time_2.
*                       We thus obtain the time taken for the car to cross both the ultrasonic sensors by calculating time_2-time_1. The distance between
*                       both the ultrasonic sensors is set as 40cm in our model. Thus we can find the speed of the vehicle by using the formula 
*                       speed=distance/time and converting the speed to km/hr.
* Example Call:		loop(); --> from any function in the file
*
*/
void loop() {
  float distance_1, distance_2, time_1, time_2, time_taken;
  distance_1 = data(trig_pin_1, echo_pin_1);
  // checking if there is an object just in front of the first ultrasonic sensor
  if(distance_1<3) {
    time_1 = millis();
  }
  distance_2 = data(trig_pin_2, echo_pin_2);
  // checking if there is an object just in front of the second ultrasonic sensor
  if(distance_2<3) {
    time_2 = millis();
    // calculating the time taken in seconds for the car to pass through both the ultrasonic sensors
    time_taken = (time_2-time_1)/1000;
  }
  // calculating the speed in km/hr of the vehicle by converting the distance from cm to km and time from sec to hr 
  speed_of_vehicle = (distance_travelled*0.00001)/(time_taken*0.000277778);
  // print to the serial monitor only if a valid speed is detected
  if(speed_of_vehicle > 0.0) {
    Serial.println(speed_of_vehicle);
    Serial.print("km/hr");
    speed_of_vehicle = 0.0;
  }
}

/*
*
* Function Name: 	data()
* Input: 		1) trig_pin of type int --> trigger pin of the ultrasonic sensor
*                       2) echo_pin of type int --> echo pin of the ultrasonic sensor
* Output: 		distance of type int --> the distance in cm of any object from the echo/trigger of ultrasonic sensor
* Logic: 		This function is used to obtain the output of an ultrasonic sensor. The output of an ultrasonic sensor is the time taken
*                       by the sound sent by the trigger to be received by the echo. From this time we can calculate the distance of an object in front
*                       of it. If the distance is less, then it means that a car is in front of the sensor.
* Example Call:		data(trig_pin_1, echo_pin_1); --> from loop function in the file
*
*/
int data(int trig_pin, int echo_pin) {
  float duration, distance;
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  // sending the trigger sound pulse
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH); // receiving the signal sent by the trigger and returning the time to the variable duration
  distance = (duration/2) / 29.1;
  return distance;
}

