/* Ean Barnawell
   Kevin Lu
   Copyright 2023 Ean Barnawell, Kevin Lu */

/* ##################################### Initial Definitions ###################################### */
unsigned long previousTime = 0;
unsigned long currentTime = millis();

// Macros for setting and clearing a bit of a register
#define bit_set(reg, bit) ((reg) |= (1 << bit));
#define bit_clr(reg, bit) ((reg) &= !(1 << bit));

#define LEDP13 PB7

// Defined hertz values for musical notes
#define NOTE_c 261
#define NOTE_d 294
#define NOTE_e 329
#define NOTE_f 349
#define NOTE_g 392
#define NOTE_a 440
#define NOTE_b 493

// Defines 8x8 Grid spiTransfer functionality values
#define OP_DECODEMODE  8
#define OP_SCANLIMIT   10
#define OP_SHUTDOWN    11
#define OP_DISPLAYTEST 14
#define OP_INTENSITY   10

// 8x8 Grid Pin Inputs
#define DIN 6
#define CS 5
#define CLK 4

// Thumbstick analog and pin port inputs
#define x_port A1
#define y_port A2
#define SW 5

// spi shift register uses 16 bits, 8 for ctrl and 8 for data
byte spidata[2];

// Sequence of musical notes that play 'Mary Had a Little Lamb'
int melody[] = { NOTE_e, NOTE_d, NOTE_c, NOTE_d, NOTE_e, NOTE_e, NOTE_e,
                    NOTE_d, NOTE_d, NOTE_d, NOTE_e, NOTE_g, NOTE_g, NOTE_e, 
                    NOTE_d, NOTE_c, NOTE_d, NOTE_e, NOTE_e, NOTE_e, NOTE_e,
                    NOTE_d, NOTE_d, NOTE_e, NOTE_d, NOTE_c};

/* ###################################### Function Definitions ###################################### */

//Transfers 1 SPI command to LED Matrix for given row
//Input: row - row in LED matrix
//       data - bit representation of LEDs in a given row; 1 indicates ON, 0 indicates OFF
void spiTransfer(volatile byte row, volatile byte data);

//Initializes the 16-bit timer/counter to play a frequency
//Input: hertz - the frequency in which the speaker will play
void setup_timer4(int hertz);

// Flashes high/low signals for 333ms to pins 47, 48, 49 in a sequential manner 
void TaskA();

// Repeats the frequency 400hz, 250hz, 800hz, for 1 second then rest for 1 second
void TaskB();

// Plays the melody of 'Mary Had a Little Lamb' one time
void TaskC();

// Reads the analog inputs from the thumbstick to update a corresponding location on 8x8 grid
void TaskD();

// Runs the functionality of Task A and Task C in tandem for 10 seconds
void TaskE();

// Runs the functionality of Task C and Task D in tandem forever
void TaskF();

// Runs the functionality of Task A and Task B in tandem forever
void TaskG();

/* ###################################### Function Implementations ###################################### */

void setup() {
  bit_set(DDRH, PORTH3);
}

void loop(){
  // TaskA();
  // TaskB();
  // TaskC();
  // TaskD();
  // TaskE();
  // TaskF();
  TaskG();
}

void setup_timer4(int hertz){
  // Initialize registers
  TCCR4B = 0;
  TCCR4A = 0;

  // Set up the timer with prescaler = 256 with CTC mode
  // set your TCCR4B to WGM42
  bit_set(TCCR4B, WGM42);

  //set your TCCR4B to CS42 (Clock Select bit 2) needs to set to 1
  bit_set(TCCR4B, CS41);

  // initialize timer/counter 4 to 0
  TCCR4A = (1 << COM4A0);

  //Initialize output compare register to the value that it will count up to
  OCR4A = F_CPU / (2 * hertz * 8) - 1;
}

void spiTransfer(volatile byte opcode, volatile byte data){
  int offset = 0; //only 1 device
  int maxbytes = 2; //16 bits per SPI command
  
  for(int i = 0; i < maxbytes; i++) { //zero out spi data
    spidata[i] = (byte)0;
  }
  //load in spi data
  spidata[offset+1] = opcode+1;
  spidata[offset] = data;
  bit_clr(PORTB, CS); //
  for(int i=maxbytes;i>0;i--)
    shiftOut(12,10,MSBFIRST,spidata[i-1]); //shift out 1 byte of data starting with leftmost bit
  bit_set(PORTB, CS);
}

void TaskA(int seconds) {

  // Sets the pins to OUTPUT mode
  bit_set(DDRL, 2);
  bit_set(DDRL, 1);
  bit_set(DDRL, 0);
  
  // Sends high/low signals to pins to make them flash
  for (int i = 0; i < seconds; i++) {
    bit_set(PORTL, 2);
    delay(333);
    bit_clr(PORTL, 2);
    bit_set(PORTL, 1);
    delay(333);
    bit_clr(PORTL, 1);
    bit_set(PORTL, 0);
    delay(333);  
    bit_clr(PORTL, 0);
  }
}

void TaskB() { 
  setup_timer4(400);
  delay(1000);
  setup_timer4(250);
  delay(1000);
  setup_timer4(800);
  delay(1000);
  setup_timer4(0);
  delay(1000);
}

void TaskC(){
  // Plays 'Mary Had a Little Lamb' melody
  for (int i = 0; i < sizeof(melody); i++) {
    setup_timer4(melody[i]);
    delay(550);
    setup_timer4(0);
    delay(250);
  }
}

void TaskD() {
  // Sets the 8x8 grid pins to OUTPUT mode
  bit_set(DDRB, DIN);
  bit_set(DDRB, CS);
  bit_set(DDRB, CLK);
  bit_set(DDRH, SW);
  bit_set(PORTB, CS);

  // Setup spiTransfer function
  spiTransfer(OP_DISPLAYTEST,0);
  spiTransfer(OP_SCANLIMIT,7);
  spiTransfer(OP_DECODEMODE,0);
  spiTransfer(OP_SHUTDOWN,1);

  // Read the analog inputs from the thumbstick
  int x_value = analogRead(x_port) / 128;
  int y_value = analogRead(y_port) / 128;

  // Convert the y coordinate to a 8-bit value
  unsigned char y_coordinate = 0b00000000;
  y_coordinate |= (1 << y_value);

  // Update the grid
  delay(0.5);
  spiTransfer(x_value, y_coordinate);

  // Clear the grid
  for (int i = 0; i < 8; i++){
    spiTransfer(i, 0b00000000);
  }
}

void TaskE(unsigned long currentTime) {
  // Sets the pins to OUTPUT mode
  bit_set(DDRL, 2);
  bit_set(DDRL, 1);
  bit_set(DDRL, 0);
  int prt = 0;

  for (int i = 0; i < sizeof(melody); i++) {
    currentTime = millis();

    // Plays the melody in conjunction with LEDs flashing
    bit_set(PORTL, ((prt + 2) % 3));
    setup_timer4(melody[i]);
    delay(222);
    setup_timer4(0);
    delay(111);
    bit_clr(PORTL, ((prt + 2) % 3));

    prt++;    // Updates LED pin port

    // Do nothing after 10 seconds
    if ((currentTime - previousTime) > 10000) {
      while(1) {}
    }
  }
}

void TaskF() {
  while (1) {
    for (int i = 0; i < sizeof(melody); i++) {
      setup_timer4(melody[i]);

      // Runs thumbstick sensor while playing the tone
      for (int j = 0; j < 111; j++) {
        TaskD();
      }
      setup_timer4(0);
      for (int j = 0; j < 56; j++) {
        TaskD();
      }
    }
  }
}

void TaskG() {
  setup_timer4(400);
  TaskA(1);
  setup_timer4(250);
  TaskA(1);
  setup_timer4(800);
  TaskA(1);
  setup_timer4(0);
  TaskA(1);
}
