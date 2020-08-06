# include <U8glib.h>
#include <TimerOne.h>
// This code is working great lolololol
//**************************************************
// Change this constructor to match your display!!!
//U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9, 8);
//**************************************************

//#define NO_PRESS 0
//#define SHORT_PRESS 1 
//#define LONG_PRESS 2
#define OPEN 1
#define CLOSE 0   // Is ir really necessary

typedef enum enum_deviceStatus { 
  START = 10,
  IDLE = 11,
  DOOR_OPEN = 12,
  RUN = 13,
  CANCEL = 14,
  DONE = 15,
  LIGHT_DANCE_2 = 17
}deviceStatus;

deviceStatus state = START; 

double timePressed = 0, timeReleased = 0;
int button1 = A7;
int button2 = A6;
int bigLed1 = 3, bigLed2 = 5, bigLed3 = 6, bigLed4 = 9;
int bigLed5 = 10, bigLed6 = 11;
int buttonLed = 7;
int buttonLed2 = 2;
int brightness = 0;    // how bright the LED is
int brightness2 = 0;
int fadeAmount = 15, fadeAmount2 = 15;
bool button1Pressed = false, button2Pressed = false;


void setup() {
  Serial.begin(9600);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(bigLed1, OUTPUT);
  pinMode(bigLed2, OUTPUT);
  pinMode(bigLed3, OUTPUT);
  pinMode(bigLed4, OUTPUT);
  pinMode(bigLed5, OUTPUT);
  pinMode(bigLed6, OUTPUT);
  pinMode(buttonLed, OUTPUT);
  pinMode(buttonLed2, OUTPUT);
//  Timer1.initialize(100000); 
}
void loop() {
    switch(state){
    case START:        state = IDLE;
                       break;
    case IDLE:         if(SensorSignal() == OPEN) state = DOOR_OPEN;
                       break;
    case DOOR_OPEN:    if (SensorSignal() == CLOSE && (button1Pressed || button2Pressed))
                           state = IDLE; 
                       else if (SensorSignal() == CLOSE) {
                           state = RUN;
                       }
                       else 
                           state = DOOR_OPEN;
                       break;                      
    case RUN:          run_main();
                       break;
    case CANCEL:       if(SensorSignal() == CLOSE)      
                            state = RUN;
                        else if (checkCancel())
                            state = IDLE;
                       break;
    case DONE:         if (SensorSignal() == OPEN)  
                            state = LIGHT_DANCE_2; 
                       break;
    case LIGHT_DANCE_2:if (SensorSignal() == CLOSE)
                            state = IDLE;
                        break;
                      
    default:           state = IDLE;
  }
  if (state == START)
  {
    // Do nothing  
    Serial.print ("State = START\n");      
  }
  else if (state == IDLE)
  {
    // Turn Lights OFF
    Serial.print ("State = IDLE\n");
    brightness = 0;
    brightness2 = 0;
    analogWrite(bigLed1, brightness);
    analogWrite(bigLed2, brightness);
    analogWrite(bigLed3, brightness);
    analogWrite(bigLed4, brightness);
    analogWrite(bigLed5, brightness2);
    analogWrite(bigLed6, brightness2);
    digitalWrite(buttonLed , LOW);
    digitalWrite(buttonLed2 , LOW);
    // Reset the timer 
    //    delay(100);
    // Check the sensor in a loop is not neccessary?
  }
  else if (state == DOOR_OPEN)
  {
    analogWrite(bigLed1, brightness);
    analogWrite(bigLed2, brightness);
    analogWrite(bigLed3, brightness);
    analogWrite(bigLed4, brightness);
    analogWrite(bigLed5, brightness2);
    analogWrite(bigLed6, brightness2);
    Serial.print("Brightness1 = ");
    Serial.print(brightness);
    Serial.print("\nBrightness2 = ");
    Serial.print(brightness2);
    Serial.print("\n");
    delay(50);
     // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0) {
      fadeAmount = 15;
    }
    else if (brightness >= 250) {
      fadeAmount = -15;
    }
    if (brightness2 <= 0) {
      fadeAmount2 = 15;
    }
    else if (brightness2 >= 250) {
      fadeAmount2 = -15;
    }
    brightness = brightness + fadeAmount;
    brightness2 = brightness2 + fadeAmount2;
//     Check which buttons are pressed and start the light dance accordingly
    if (analogRead(button1) >= 1000 ) button1Pressed = true;
    if (analogRead(button2) >= 1000 ) button2Pressed = true;
    if (button1Pressed && button2Pressed)
    {
      brightness = 255;
      brightness2 = 255;
      digitalWrite(buttonLed , HIGH);
      digitalWrite(buttonLed2 , HIGH);
    }
    // Update the light dance as the buttons get pressed
     else if(button1Pressed) //You can make this part shorter
     { 
      brightness = 255;
      digitalWrite(buttonLed , HIGH);
     }
     else if(button2Pressed)
     {
      brightness2 = 255;
      digitalWrite(buttonLed2 , HIGH);
     }
  }
  else if (state == RUN){
    // Turn of the lights 
    // Send the mosfet signal according to the buttons pressed
    // Start the timer 
    // consistently check the SensorSignal() to cancel the operations
    // Maybe quickly turn off the the mosfet signal here before changing the state to cancel
  }
  else if(state == CANCEL)
  {
    // Double-Check the Mosfet signal one more time to turn off the device
    // Display some sort of message using the light dance that the device is in resume state
  }
  else if (state == DONE){
    // Turn off the mosfet signal 
    // nothing else I can think of at the moment   
  }

  else if (state == LIGHT_DANCE_2){
    // Start the New light dance 
  }
  else
  {
    Serial.print("The state is : UNKNOWN\n"); 
  }
}

int SensorSignal(){
  if(state == IDLE || state == DOOR_OPEN)
    return OPEN;
  else return CLOSE;
}
// bool checktime()
// {
//   if(minsElapsed >= 10)
//     return true;
//   else 
//     return false;
// }
int run_main()
{
}
int checkCancel()
{
}