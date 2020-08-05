/*
  Fade

  This example shows how to fade an LED on pin 9
  using the analogWrite() function.

  This example code is in the public domain.
*/
int button = A7;
int button2 = A6;
int bigLed1 = 3, bigLed2 = 5, bigLed3 = 6, bigLed4 = 9;
int bigLed5 = 10, bigLed6 = 11;
int smallLed = 7;
int smallLed2 = 2;
int led = 12;           // the pin that the LED is attached to
int brightness = 0;    // how bright the LED is
int brightness2 = 0;
int fadeAmount = 5;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  pinMode(button2, INPUT);
  pinMode(bigLed1, OUTPUT);
  pinMode(bigLed2, OUTPUT);
  pinMode(bigLed3, OUTPUT);
  pinMode(smallLed, OUTPUT);
  pinMode(smallLed2, OUTPUT);

  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness of pin 9:
  //  Serial.print(analogRead(button));
  if (analogRead(button) >= 1000 ) {
    analogWrite(bigLed1, brightness);
    analogWrite(bigLed2, brightness);
    analogWrite(bigLed3, brightness);
    analogWrite(bigLed4, brightness);
    digitalWrite(smallLed , HIGH);
    digitalWrite(smallLed2 , LOW);
    Serial.print("We are here\n");
    Serial.print(brightness);
    Serial.print("\n");
    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (brightness == 0 || brightness == 255) {
      fadeAmount = -fadeAmount ;
    }
    if (brightness < 0) {
      brightness = 0 ;
    }
    if (brightness > 255) {
      brightness = 255 ;
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(10);
  }
  else {
    Serial.print(" We are At ELSE\n");
    //    digitalWrite(bigLed,  HIGH);
    brightness = 0;
    analogWrite(bigLed1, brightness);
    analogWrite(bigLed2, brightness);
    analogWrite(bigLed3, brightness);
    analogWrite(bigLed4, brightness);
    while (analogRead(button) > 1000);
  }
   if (analogRead(button2) >= 1000 ) {
    analogWrite(bigLed5, brightness2);
    analogWrite(bigLed6, brightness2);
    digitalWrite(smallLed , LOW);
    digitalWrite(smallLed2 , HIGH);
    brightness2 = brightness2 + fadeAmount;
    if (brightness2 == 0 || brightness2 == 255) {
      fadeAmount = -fadeAmount ;
      brightness += fadeAmount;
    }
    if (brightness2 < 0) {
      brightness2 = 0 ;
    }
    if (brightness2 > 255) {
      brightness2 = 255 ;
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(10);
  }
  else{
    Serial.print(" We are At ELSE2\n");
    brightness2 = 0;
    analogWrite(bigLed5, brightness2);
    analogWrite(bigLed6, brightness2);
    while (analogRead(button2) > 1000);
  }
}
