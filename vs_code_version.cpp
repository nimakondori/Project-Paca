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
#define CLOSE 2   // Is ir really necessary

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
//int switchLED = 6;
//int switchPin = 3;
//int ledPin = 7;
//int hallSensor = A2;
//int switchPin_Dig = 3; 
//int isStarted = 0;
//int lightIntensity = 0;
//volatile bool onState = false;
//long minsElapsed = 0;
//long secElapsed = 0;


void setup() {
  Serial.begin(9600);
//  pinMode(switchLED, OUTPUT);
//  pinMode(switchPin, INPUT);
//  pinMode(switchPin_Dig, INPUT);
//  pinMode(ledPin, OUTPUT);
//  pinMode(hallSensor, INPUT);
//  attachInterrupt(digitalPinToInterrupt(switchPin_Dig), toggle, HIGH);
  Timer1.initialize(100000); 
}
void loop() {
    switch(state){
    case START:        state = IDLE;
                       break;
    case IDLE:         if(SensorSignal() == OPEN) state = DOOR_OPEN;
                       break;
    case DOOR_OPEN:    if (SensorSignal() == CLOSE && buttonPress == 00)
                           state = IDLE; 
                       else if (SensorSignal() == CLOSE) {
                           state = RUN;
                       }
                       else 
                           state = DOOR_OPEN;
                       break;                      
    case RUN:          main();
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
                      
    default:         state = IDLE;
  }
  if (state == START)
  {
    // Do nothing        
  }
  else if (state == IDLE)
  {
    // Turn Lights OFF
    // Reset the timer 
  }
  else if (state == DOOR_OPEN)
  {
    // Check which buttons are pressed
    // Start the light dance accordingly
    // Update the light dance as the buttons get pressed
    // Update the buttonPress here
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
  return 1;
}
// bool checktime()
// {
//   if(minsElapsed >= 10)
//     return true;
//   else 
//     return false;
// }
