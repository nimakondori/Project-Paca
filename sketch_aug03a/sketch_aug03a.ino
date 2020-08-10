  // #include <TimerOne.h>
  // // This code is working great lolololol
  // //**************************************************
  // // Change this constructor to match your display!!!
  // //U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9, 8);
  // //**************************************************

  //#define NO_PRESS 0
  //#define SHORT_PRESS 1 
  //#define LONG_PRESS 2
  #define OPEN 1
  #define CLOSE 0   // Is ir really necessary
  #define TOP_MAX_TIME 100
  #define BOTTOM_MAX_TIME 200
  #define MOSFET_ON 1024
  #define MOSFET_OFF 0
  

  typedef enum enum_deviceStatus { 
    START = 10,
    IDLE = 11,
    DOOR_OPEN = 12,
    RUN = 13,
    CANCEL = 14,
    DONE = 15,
    LIGHT_DANCE_2 = 16,
    COMPLETED = 17
  }deviceStatus;

  deviceStatus state = START; 

  double timePressed = 0, timeReleased = 0;
  int button1 = 7;
  int button2 = 8;
  int MOSFET = A5;
  int senseSig = 2;
  int bigLed1 = 3, bigLed2 = 5, bigLed3 = 6, bigLed4 = 9;
  int bigLed5 = 10, bigLed6 = 11;
  int buttonLed = 4;
  int buttonLed2 = 12;
  int brightness = 0;    // how bright the LED is
  int brightness2 = 0;
  int fadeAmount = 15, fadeAmount2 = 15;
  bool button1Pressed = false, button2Pressed = false;
  // bool cancelButton1Pressed = false, cancelButton2Pressed = false;
  bool canceled = false, done = false;
  bool FirstTimePress = false, FirstTimePress2 = false;
  bool userCanceled1 = !FirstTimePress, userCanceled2 = !FirstTimePress;
  bool topDone = false, bottomDone = false;
  int finishTime1 = BOTTOM_MAX_TIME;
  int finishTime2 = TOP_MAX_TIME;
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
    pinMode (MOSFET, OUTPUT);
//    pinMode(MOSFET2, OUTPUT);
  //  Timer1.initialize(100000); 
  }
  void loop() {
      switch(state){
      case START:          state = IDLE;
                           break;
      case IDLE:           if(SensorSignal() == OPEN) state = DOOR_OPEN;
                           break;
      case DOOR_OPEN:      if (SensorSignal() == CLOSE && (button1Pressed || button2Pressed))
                              state = RUN; 
                           else if (SensorSignal() == CLOSE) {
                              state = IDLE;
                           }
                           else 
                              state = DOOR_OPEN;
                           break;                      
      case RUN:            if(canceled)state = CANCEL;
                           else if(done) state = DONE;
                           break;
      case CANCEL:         if (userCanceled1 && userCanceled2) state = COMPLETED;
                           else if(SensorSignal() == CLOSE) state = RUN;
                              
                        //  else if (checkCancel())
                        //       state = IDLE;
                        break;
      case DONE:            if (userCanceled1 && userCanceled2) state = COMPLETED;
                            else if (topDone || bottomDone) state = LIGHT_DANCE_2; 
                            else state = RUN;
                            break;
      case LIGHT_DANCE_2: if (SensorSignal() == OPEN)
                            state = IDLE;
                          break;
      case COMPLETED:     state = IDLE;
                          break;
                         
      default:            state = IDLE;
    }
    if (state == START)
    {
      // Do nothing  
      Serial.print ("State = START\n");      
    }
    else if (state == IDLE)
    {
//      analogWrite(MOSFET2, 1024);
      // Turn Lights OFF
      Serial.print ("State = IDLE\n");
      brightness = 0;
      brightness2 = 0;
      button1Pressed = false;
      button2Pressed = false;
      canceled = false;
      done = false;
      FirstTimePress = false;
      FirstTimePress2 = false;
      userCanceled1 = false; 
      userCanceled2 = false; 
      topDone = false;
      bottomDone = false; 
      analogWrite(MOSFET, MOSFET_OFF);
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
      if (digitalRead(button1) == HIGH ) 
      {
        Serial.print("Button1Pressed\n");
        button1Pressed = !button1Pressed;
        FirstTimePress = !FirstTimePress;
        delay(10);
        while(digitalRead(button1));
      }
      if (digitalRead(button2) == HIGH ) {
        Serial.print("Button2Pressed\n");
        button2Pressed = !button2Pressed;
        FirstTimePress2 = !FirstTimePress2;
        delay(10);
        while(digitalRead(button2));
      }
      if(button1Pressed && button2Pressed && FirstTimePress && FirstTimePress2) //You can make this part shorter
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
      else if (button1Pressed && button2Pressed && !FirstTimePress && FirstTimePress2)
      {
        brightness = 0;
        digitalWrite(bigLed1, LOW);
        digitalWrite(bigLed2, LOW);
        digitalWrite(bigLed3, LOW);
        digitalWrite(bigLed4, LOW);
        brightness2 = 255;
        digitalWrite(bigLed5, HIGH);
        digitalWrite(bigLed6, HIGH);
      }
      else if (button1Pressed && button2Pressed && FirstTimePress && !FirstTimePress2)
      {
        brightness = 0;
        digitalWrite(bigLed1, HIGH);
        digitalWrite(bigLed2, HIGH);
        digitalWrite(bigLed3, HIGH);
        digitalWrite(bigLed4, HIGH);
        brightness2 = 0;
        digitalWrite(bigLed5, LOW);
        digitalWrite(bigLed6, LOW);
      }
      else if (button1Pressed && button2Pressed && !FirstTimePress && !FirstTimePress2)
      {
        brightness = 0;
        digitalWrite(bigLed1, LOW);
        digitalWrite(bigLed2, LOW);
        digitalWrite(bigLed3, LOW);
        digitalWrite(bigLed4, LOW);
        brightness2 = 0;
        digitalWrite(bigLed5, LOW);
        digitalWrite(bigLed6, LOW);
      }
      if(button1Pressed && !button2Pressed && FirstTimePress)
      {
        brightness = 255;
        digitalWrite(bigLed1, HIGH);
        digitalWrite(bigLed2, HIGH);
        digitalWrite(bigLed3, HIGH);
        digitalWrite(bigLed4, HIGH);
      }
      else if (!FirstTimePress)
      {
        brightness = 0;
        digitalWrite(bigLed1, LOW);
        digitalWrite(bigLed2, LOW);
        digitalWrite(bigLed3, LOW);
        digitalWrite(bigLed4, LOW);
      }
      if(!button1Pressed && button2Pressed && FirstTimePress2)
      {
        brightness2 = 255;
        digitalWrite(bigLed5, HIGH);
        digitalWrite(bigLed6, HIGH);
      }
      else if (!FirstTimePress2)
      {
        brightness2 = 0;
        digitalWrite(bigLed5, LOW);
        digitalWrite(bigLed6, LOW);
      }
    }
    else if (state == RUN){
      // consistently check the SensorSignal() to cancel the operations
      FirstTimePress = false;
      FirstTimePress2 = false;
      while(currentTime1 < finishTime1 || currentTime2 < finishTime2){
        Serial.print("The state is : RUN\n");
        if(SensorSignal() == OPEN)
        {
          canceled = true;
          break;
        }
        delay(50);
        analogWrite(MOSFET, MOSFET_ON);
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
            bottomDone = true;
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
              topDone = true;
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
            topDone = true;

          }
        if (currentTime2>=finishTime2) 
        {
         topDone = true;
          break;
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
            bottomDone = true;
          }  
          if (currentTime1>=finishTime1) 
          {
            bottomDone = true;
            break;
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
      analogWrite(MOSFET, MOSFET_OFF);
      Serial.print("The state is : CANCEL\n");
      canceled = false;
      if(digitalRead(button1) == HIGH){
        FirstTimePress = !FirstTimePress;
        if(FirstTimePress){
          button1Pressed = true;
          currentTime1 = 0;
          brightness = 255;
          userCanceled1 = false;
          analogWrite(bigLed1, brightness); 
          analogWrite(bigLed2, brightness);
          analogWrite(bigLed3, brightness);
          analogWrite(bigLed4, brightness);
        }
        else
        {
          currentTime1 = BOTTOM_MAX_TIME + 10;  // The plus 10 is just to make sure it is not going to run
          brightness = 0;
          userCanceled1 = true;
          analogWrite(bigLed1, brightness); 
          analogWrite(bigLed2, brightness);
          analogWrite(bigLed3, brightness);
          analogWrite(bigLed4, brightness);
        }
        delay(10);
        while(digitalRead(button1) == HIGH);
      }
      else if(digitalRead(button2) == HIGH){
        button2Pressed = true;
        FirstTimePress2 = !FirstTimePress2;
        if (FirstTimePress2)
        {
          currentTime2 = 0;       
          brightness2 = 255;
          userCanceled2 = false;
          analogWrite(bigLed5, brightness2); 
          analogWrite(bigLed6, brightness2);
        }
        else 
        {
          currentTime2 = TOP_MAX_TIME + 10;       
          brightness2 = 0;
          userCanceled2 = true;
          analogWrite(bigLed5, brightness2); 
          analogWrite(bigLed6, brightness2);
        }
        delay(10);
        while(digitalRead(button2) == HIGH);
      }
    
      // Double-Check the Mosfet signal one more time to turn off the device
      // Display some sort of message using the light dance that the device is in resume state
    }
    else if (state == DONE){
      Serial.print("state = DONE\n");
      // Turn off the mosfet signal 
      analogWrite(MOSFET, MOSFET_OFF);
      // nothing else I can think of at the moment 
        
    }

    else if (state == LIGHT_DANCE_2){
      brightness = brightness2;
      Serial.print(brightness);
      analogWrite(bigLed1, brightness);
      analogWrite(bigLed2, brightness);
      analogWrite(bigLed3, brightness);
      analogWrite(bigLed4, brightness);
      analogWrite(bigLed5, brightness2);
      analogWrite(bigLed6, brightness2);
      digitalWrite(buttonLed, HIGH);
      digitalWrite(buttonLed2, HIGH);
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
      delay(50);
      analogWrite(MOSFET, MOSFET_OFF);
      Serial.print("The state is : LIGHT_DANCE_2\n"); 
      
    }
    else if (state == COMPLETED)
    {
      analogWrite(MOSFET, MOSFET_OFF);
      brightness = 0;
      brightness2 = 0;
      digitalWrite(bigLed1, brightness);
      digitalWrite(bigLed2, brightness);
      digitalWrite(bigLed3, brightness);
      digitalWrite(bigLed4, brightness);
      digitalWrite(bigLed5, brightness2);
      digitalWrite(bigLed6, brightness2);
    }
    else
    {
      Serial.print("The state is : UNKNOWN\n"); 
    }
  }

  int SensorSignal(){
    if(digitalRead(senseSig) == HIGH)
      return OPEN;
    else return CLOSE;
  }

  int checkCancel()
  {
    if(digitalRead(button1) == HIGH) return true;
    else return false;
  }
