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
int senseSig = 4;
int bigLed1 = 3, bigLed2 = 5, bigLed3 = 6, bigLed4 = 9;
int bigLed5 = 10, bigLed6 = 11;
int buttonLed = 7;
int buttonLed2 = 2;
int brightness = 0;    // how bright the LED is
int brightness2 = 0;
int fadeAmount = 15, fadeAmount2 = 15;
bool button1Pressed = false, button2Pressed = false;
bool canceled = false, done = false;
int finishTime1 = 200;
int finishTime2 = 100;
int currentTime1, currentTime2;



void setup() {
  Serial.begin(9600);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(senseSig, INPUT);
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
                           state = RUN; 
                       else if (SensorSignal() == CLOSE) {
                           state = IDLE;
                       }
                       else 
                           state = DOOR_OPEN;
                       break;                      
    case RUN:          if(canceled)state = CANCEL;
                       else if(done) state = DONE;
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
    button1Pressed = false;
    button2Pressed = false;
    canceled = false;
    done = false;
    analogWrite(bigLed1, brightness);
    analogWrite(bigLed2, brightness);
    analogWrite(bigLed3, brightness);
    analogWrite(bigLed4, brightness);
    analogWrite(bigLed5, brightness2);
    analogWrite(bigLed6, brightness2);
    digitalWrite(buttonLed , LOW);
    digitalWrite(buttonLed2 , LOW);
    // Reset the timer 
    currentTime1 = 0;
    currentTime2 = 0;
    // Check the sensor in a loop is not neccessary?
  }
  else if (state == DOOR_OPEN)
  {
    digitalWrite(buttonLed , HIGH);
    digitalWrite(buttonLed2 , HIGH);
    if (analogRead(button1) >= 1000 ) 
    {
      Serial.print("Button1Pressed\n");
      button1Pressed = true;
    }
    if (analogRead(button2) >= 1000 ) {
      Serial.print("Button2Pressed\n");
      button2Pressed = true;
    }
    if(button1Pressed && button2Pressed) //You can make this part shorter
    { 
      brightness = 255;
      digitalWrite(bigLed1, HIGH);
      digitalWrite(bigLed2, HIGH);
      digitalWrite(bigLed3, HIGH);
      digitalWrite(bigLed4, HIGH);
      brightness2 = 255;
      digitalWrite(bigLed5, HIGH);
      digitalWrite(bigLed6, HIGH);
    }
    if(button1Pressed && !button2Pressed)
     {
      brightness = 255;
      digitalWrite(bigLed1, HIGH);
      digitalWrite(bigLed2, HIGH);
      digitalWrite(bigLed3, HIGH);
      digitalWrite(bigLed4, HIGH);
     }
     if(!button1Pressed && button2Pressed)
     {
      brightness2 = 255;
      digitalWrite(bigLed5, HIGH);
      digitalWrite(bigLed6, HIGH);
     }
  }
  else if (state == RUN){
    // consistently check the SensorSignal() to cancel the operations
    while(currentTime1 < finishTime1 || currentTime2 < finishTime2){
      Serial.print("The state is : RUN\n");
      if(SensorSignal() == OPEN)
      {
        canceled = true;
        break;
      }
      delay(50);
      currentTime1 = currentTime1 + 1;
      currentTime2 = currentTime2 + 1;
      if(button1Pressed && button2Pressed){
        Serial.print("Both Buttons Detected\n");
        if(currentTime1 <= finishTime1 / 4){
          analogWrite(bigLed1, brightness); 
          digitalWrite(bigLed2, HIGH);
          digitalWrite(bigLed3, HIGH);
          digitalWrite(bigLed4, HIGH);
        }
        else if(currentTime1 <= finishTime1 / 2){
          digitalWrite(bigLed1, LOW); 
          analogWrite(bigLed2, brightness);
          digitalWrite(bigLed3, HIGH);
          digitalWrite(bigLed4, HIGH);
        }
        else if(currentTime1 <= 3*finishTime1 / 4){
          digitalWrite(bigLed1, LOW); 
          digitalWrite(bigLed2, LOW);
          analogWrite(bigLed3, brightness);
          digitalWrite(bigLed4, HIGH);
        }
        else if(currentTime1 < finishTime1){
          digitalWrite(bigLed1, LOW); 
          digitalWrite(bigLed2, LOW);
          digitalWrite(bigLed3, LOW);
          analogWrite(bigLed4, brightness);
        }
        else if(currentTime1 >= finishTime1){
         digitalWrite(bigLed1, LOW); 
          digitalWrite(bigLed2, LOW);
          digitalWrite(bigLed3, LOW);
          digitalWrite(bigLed4, LOW);
        }
        if(currentTime2 <= finishTime2 / 2){
            analogWrite(bigLed5, brightness2);
            digitalWrite(bigLed6, HIGH);
          }
       else if(currentTime2 < finishTime2){
            digitalWrite(bigLed5, LOW);
            analogWrite(bigLed6, brightness2);
          }
       else if(currentTime2 >= finishTime2){
            digitalWrite(bigLed5, LOW);
            digitalWrite(bigLed6, LOW);
          }
      }
      else if(button2Pressed && !button1Pressed){
       Serial.print("Button2 Detected\n"); 
       if(currentTime2 <= finishTime2 / 2){
          analogWrite(bigLed5, brightness2);
          digitalWrite(bigLed6, HIGH);
        }
       else if(currentTime2 < finishTime2){
          digitalWrite(bigLed5, LOW);
          analogWrite(bigLed6, brightness2);
        }
       else if(currentTime2 >= finishTime2){
           digitalWrite(bigLed5, LOW);
          digitalWrite(bigLed6, LOW);
        }
       }
        //if only button one is pressed
        else if(button1Pressed && !button2Pressed){
        Serial.print("Button1 Detected\n");
        Serial.print(currentTime1);
        if(currentTime1 <= finishTime1 / 4){
          Serial.print("1\n");
          analogWrite(bigLed1, brightness); 
          digitalWrite(bigLed2, HIGH);
          digitalWrite(bigLed3, HIGH);
          digitalWrite(bigLed4, HIGH);
        }
        else if(currentTime1 <= finishTime1 / 2){
          Serial.print("2\n");
          digitalWrite(bigLed1, LOW); 
          analogWrite(bigLed2, brightness);
          digitalWrite(bigLed3, HIGH);
          digitalWrite(bigLed4, HIGH);
        }
        else if(currentTime1 <= 3*finishTime1 / 4){
          Serial.print("3\n");
          digitalWrite(bigLed1, LOW); 
          digitalWrite(bigLed2, LOW);
          analogWrite(bigLed3, brightness);
          digitalWrite(bigLed4, HIGH);
        }
        else if(currentTime1 < finishTime1){
          Serial.print("4\n");
          digitalWrite(bigLed1, LOW); 
          digitalWrite(bigLed2, LOW);
          digitalWrite(bigLed3, LOW);
          analogWrite(bigLed4, brightness);
        }
        else if(currentTime1 >= finishTime1){
          Serial.print("5\n");
          digitalWrite(bigLed1, LOW); 
          digitalWrite(bigLed2, LOW);
          digitalWrite(bigLed3, LOW);
          digitalWrite(bigLed4, LOW);
        }  
      }
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
    }
    if(!canceled)done = true;
    // Maybe quickly turn off the the mosfet signal here before changing the state to cancel
  }
  else if(state == CANCEL)
  {
    Serial.print("The state is : CANCEL\n");
    // Double-Check the Mosfet signal one more time to turn off the device
    // Display some sort of message using the light dance that the device is in resume state
  }
  else if (state == DONE){
    Serial.print("state = DONE\n");
    // Turn off the mosfet signal 
    // nothing else I can think of at the moment   
  }

  else if (state == LIGHT_DANCE_2){
    // Start the New light dance 
    brightness = 0;
    brightness2 = 0;
//    digitalWrite(buttonLed, HIGH);
//    digitalWrite(buttonLed2, HIGH);
    analogWrite(bigLed1, brightness);
    delay(50);
    analogWrite(bigLed2, brightness);
    delay(100);
    analogWrite(bigLed3, brightness);
    delay(100);
    analogWrite(bigLed4, brightness);
    delay(100);
    analogWrite(buttonLed, brightness);
    delay(100);
    analogWrite(bigLed5, brightness2);
    delay(100);
    analogWrite(bigLed6, brightness2);
    delay(100);
    analogWrite(buttonLed2, brightness2);
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
  }
  else
  {
    Serial.print("The state is : UNKNOWN\n"); 
  }
}

int SensorSignal(){
  if(digitalRead(senseSig) == LOW)
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
int checkCancel()
{
  if(analogRead(button1)) return true;
  else return false;
}
