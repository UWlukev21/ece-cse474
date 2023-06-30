/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
int PIN_10 = 10;
int PIN_2 = 2;
int count = 0;
unsigned long previousTime = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN, PIN_10, and PIN_2 as an output.
  pinMode(PIN_10, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_2, OUTPUT);
}

/*
//Task 2.2 - Blink onboard LED @ 200ms

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)  
  // delay(1000); for task 1.8
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  // delay(1000); for task 1.8
  delay(200);
}
*/

/*
//Task 3.3 - Blink off-board LED @ 200ms

void loop() {
  digitalWrite(PIN_10, HIGH); //turn the LED on (HIGH is the voltage level)
  delay(200);
  digitalWrite(PIN_10, LOW); //turn the LED off by making the voltage LOW
  delay(200);
}
*/

/*
//Task 4.2/4.3 - Blink onboard and off-board LEDs with speaker clicks.

void loop() {
  digitalWrite(PIN_10, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW

  digitalWrite(PIN_2, HIGH);   // sound the speaker by making voltage high
  delay(200);  
  digitalWrite(PIN_2, LOW);    // sound the speaker by making voltage low    

  digitalWrite(PIN_10, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)  

  digitalWrite(PIN_2, HIGH);   // sound the speaker by making voltage high
  delay(200);
  digitalWrite(PIN_2, LOW);    // sound the speaker by making voltage low
}

*/
 

//Task 5.1 - Make a tone @ 250Hz with LEDs flashing @ 200ms without audible glitches.

void loop() {
  unsigned long currentTime = millis(); 
  if (count < 2000) { // stop sounding the speaker after 2000 cycles
    digitalWrite(PIN_2, HIGH);
    delay(2);  //changes hertz
    digitalWrite(PIN_2, LOW);
    delay(2);
    count++;
  }
  if (currentTime - previousTime < 200) { // turns the external LED on and onboard LED off
    digitalWrite(PIN_10, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
  } else {   // turns the external LED off and the onboard LED on
    digitalWrite(PIN_10, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (currentTime - previousTime >= 400) {  // marks the end of a cycle
    previousTime = currentTime;
  }
}
