// Define connections to the shift register
const int latchPin = 11; // Connects to STCP (latch pin) on the shift register
const int clockPin = 10; // Connects to SHCP (clock pin) on the shift register
const int dataPin = 12; // Connects to DS (data pin) on the shift register

const int startButtonPin = 2;
const int resetButtonPin = 9;
const int saveLapButtonPin = 8;

byte startButtonState = HIGH;
byte resetButtonState = HIGH;
byte saveLapButtonState = HIGH;

// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

// Store the digits in an array for easy access
int displayDigits[] = {segD1, segD2, segD3, segD4};

// Number of digits in the display
const int displayCount = 4; 

// Define the number of unique encodings (0-9, A-F for hexadecimal)
const int encodingsNumber = 16;

// Define byte encodings for the hexadecimal characters 0-F
byte byteEncodings[encodingsNumber] = {
//A B C D E F G DP
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110 // F
};
byte dpEncoding = B00000001;
// Variables for controlling the display update timing
unsigned long lastIncrement = 0;

// Delay between updates (milliseconds)
unsigned long delayCount = 50;

// The number being displayed
unsigned long number = 0; 

//Variable used to store the value of number at pause
unsigned long pausedNumber = 0;
unsigned long lapNumber = 123;
unsigned long valueForLap = 0;
unsigned long counterForLap = 0;
unsigned long savedLaps[4];

//debounce for start button
byte readingStart = HIGH;
byte lastReadingStart = HIGH;
unsigned int lastDebounceTimeStart = 0;
unsigned int debounceDelayStart = 50;

//debounce for reset button
byte readingReset = HIGH;
byte lastReadingReset = HIGH;
unsigned int lastDebounceTimeReset = 0;
unsigned int debounceDelayReset = 50;

//debounce for lap button 
byte readingLap = HIGH;
byte lastReadingLap = HIGH;
unsigned int lastDebounceTimeLap = 0;
unsigned int debounceDelayLap = 50;

//flags for start and pause 
unsigned int startOn = 0;
unsigned int pauseOn = 0;
unsigned int lapButtonOn = 0;
unsigned int crtDisplay;
unsigned int resetOn =0;
unsigned int saveLaps=0;
unsigned int currentLap=0;

//used for not showing the initial zero state when a display is being updated
unsigned int notUpdatedDisplay = 4;

void setup() {

      // Initialize the pins connected to the shift register as outputs
      pinMode(latchPin, OUTPUT);
      pinMode(clockPin, OUTPUT);
      pinMode(dataPin, OUTPUT);
      pinMode(startButtonPin, INPUT_PULLUP);
      pinMode(resetButtonPin, INPUT_PULLUP);
      pinMode(saveLapButtonPin, INPUT_PULLUP);

      // Initialize digit control pins and set them to LOW (off)
      for (int i = 0; i < displayCount; i++) {
          pinMode(displayDigits[i], OUTPUT);
          digitalWrite(displayDigits[i], LOW);
      }

      // Begin serial communication for debugging purposes
      Serial.begin(9600);
      
      
       
}
void loop() {

      // Display the initial state only once
      displayInitialState();
    
      readingStart = digitalRead(startButtonPin);
      if(readingStart != lastReadingStart){
        lastDebounceTimeStart = millis();
      }
      if ((millis() - lastDebounceTimeStart) > debounceDelayStart) {
        if (readingStart != startButtonState) {
              startButtonState = readingStart;
              if(startButtonState == HIGH){
                if(startOn == 1 ){
                  pauseOn = 1;
                  startOn = 0;
                } else {
                        if(lapButtonOn == 1){
                          number = valueForLap;
                          lapButtonOn = 0;
                          writeReg(B00000000);
                        } 
                        startOn = 1;
                        pauseOn = 0;
                    }
              } 
             
        }
      }
      lastReadingStart = readingStart;
      
      //pressing the reset button resets the displays to 000.0 only in pause mode
      if(pauseOn){
          readingReset = digitalRead(resetButtonPin);
          if(readingReset != lastReadingReset){
            lastDebounceTimeReset = millis();
          }
          if ((millis() - lastDebounceTimeReset) > debounceDelayReset) {
            if (readingReset != resetButtonState) {
                  resetButtonState = readingReset;
                  if(resetButtonState == HIGH){
                       resetDisplays();
                       resetOn=1;
                  } 
                
              }
          }
          lastReadingReset = readingReset;
      }

      if(startOn == 1 && pauseOn == 0){
          if (millis() - lastIncrement > delayCount) {
              number++;
              //Ensure that 'number' wraps around after reaching 9999 to start over from 0
              number %= 10000; 
              lastIncrement = millis();
          }
          lapNumber = number;
          writeNumber(number);
      } else {
          pausedNumber = number;
          displayPausedState(pausedNumber);

        }
      readingLap = digitalRead(saveLapButtonPin);
      
      if (readingLap != lastReadingLap) {
          lastDebounceTimeLap = millis();
      }
      if ((millis() - lastDebounceTimeLap) > debounceDelayLap) {
          if (readingLap != saveLapButtonState) {
              saveLapButtonState = readingLap;
              if (saveLapButtonState == HIGH) {

                //laps are not saved in pause mode
                if( resetOn == 0 && pauseOn == 0){
                  saveLap(lapNumber);
                  currentLap++;
                }
                
                if(resetOn == 1 && startOn == 0 && pauseOn == 1){
                          lapButtonOn = 1;
                          valueForLap = savedLaps[counterForLap];
                          counterForLap++;
                          //notUpdatedDisplay = 4 - valueForLap/10;//for updating the necesarry zeros 
                          if(counterForLap > 3){
                            counterForLap = 0;
                          }

                    }
                  
              }
           }
        }
    lastReadingLap = readingLap;

    if(lapButtonOn == 1){
      displayPausedState(valueForLap);
    }
   
}

void saveLap(int numberToSave) {
    if(currentLap > 3){
      currentLap = 0;
    }
    savedLaps[currentLap] = numberToSave;
}

void displayPausedState(int pausedNumber) {
    // Display the paused number on the 7-segment display
    for (int i = 0; i < displayCount; i++) {
        digitalWrite(displayDigits[i], LOW);
        // Display the paused number on all displays
        writeNumber(pausedNumber);
    }
    writeReg(B00000000);  // Clear the register to avoid ghostin
}


void displayInitialState() {
    //activates third display for showing the decimal point 
    activateDisplay(2);
    writeReg(dpEncoding);
    //without it not only the third decimal point is displayed 
    writeReg(B00000000);

     // Turn off all digit control pins to avoid ghosting
    for (int i = 0; i < displayCount; i++) {
        digitalWrite(displayDigits[i], HIGH);
    }

    for (int i = 0; i < notUpdatedDisplay; i++) {
        digitalWrite(displayDigits[i], LOW);
        writeReg(byteEncodings[0]);
    }
    
    // Clear the register to avoid ghosting
    writeReg(B00000000);
    
}


void writeReg(int digit) {
      // Prepare to shift data by setting the latch pin low
      digitalWrite(latchPin, LOW);
      // Shift out the byte representing the current digit to the shift register
      shiftOut(dataPin, clockPin, MSBFIRST, digit);
      // Latch the data onto the output pins by setting the latch pin high
      digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
      // Turn off all digit control pins to avoid ghosting
      for (int i = 0; i < displayCount; i++) {
           digitalWrite(displayDigits[i], HIGH);
      }
      // Turn on the current digit control pin
      digitalWrite(displayDigits[displayNumber], LOW);
       
}

void resetDisplays(){
      displayInitialState();
      notUpdatedDisplay = 4;
      number = 0;
      currentLap = 0;  
}


void writeNumber(int number) {
      int currentNumber = number;

      // Start with the least significant digit
      int displayDigit = 3; 

      int lastDigit = 0;
      //Loop through each digit of the current number
      while (currentNumber != 0) {

          if(notUpdatedDisplay > displayDigit){
             notUpdatedDisplay = displayDigit;
          }
          //Extract the last digit of the current number
          lastDigit = currentNumber % 10;

          //Activate the current digit on the display
          activateDisplay(displayDigit);

          //Output the byte encoding for the last digit to the display
          writeReg(byteEncodings[lastDigit]);

          //Move to the next digit
          displayDigit--;

          //Update 'currentNumber' by removing the last digit
          currentNumber /= 10;

          //Clear the display to prevent ghosting between digit activations
          writeReg(B00000000); 
      }
}