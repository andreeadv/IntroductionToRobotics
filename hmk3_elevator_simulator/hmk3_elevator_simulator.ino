//pins declarations
const int buttonPin1 = 2;
const int buttonPin2 = 7;
const int buttonPin3 = 8;
 
const int ledPin1 = 13;
const int ledPin2 = 12;
const int ledPin3 = 11;

const int ledPinOperational = 4;
 
const int buzzerPin = 10;
 
//initial states declarations
byte buttonState1 = LOW;
byte buttonState2 = LOW;
byte buttonState3 = LOW;
 
byte ledState1 = HIGH;
byte ledState2 = LOW;
byte ledState3 = LOW;
byte opLedState = HIGH;
 
 
//variables for debounce

byte reading1 = LOW;
byte lastReading1 = LOW;
unsigned int lastDebounceTime1 = 0;
 
byte reading2 = LOW;
byte lastReading2 = LOW;
unsigned int lastDebounceTime2 = 0;
 
byte reading3 = LOW;
byte lastReading3 = LOW;
unsigned int lastDebounceTime3 = 0;
//interval de timp pentru aprinderile ledurilor 
unsigned long movementDelay = 2000; 
unsigned int debounceDelay = 50;

//ia numarul etajului corespunzator butonului apasat
int calledfloor = 0;
//default state e la primu etaj 
int currentFloor = 1;

 
int previousMillis1 = 0;
int previousMillis2 = 0;
int previousMillis3 = 0;
int previousMillis4 = 0;

//flag pentru prima iterare prin loop
int cntFirstIteration = 0;

//for blinking the operational led
long startBlink;
long lastBlink=0;
int blinkingInterval = 500;
//variabila folosita pentru ca liftul sa nu treaca de mai multe ori prin starile intermediare pana la etajul cerut
boolean ledsActivated = false;

void setup() {

      pinMode(buttonPin1, INPUT_PULLUP);
      pinMode(buttonPin2, INPUT_PULLUP);
      pinMode(buttonPin3, INPUT_PULLUP);
 
      pinMode(ledPin1, OUTPUT);
      pinMode(ledPin2, OUTPUT);
      pinMode(ledPin3, OUTPUT);
      pinMode(ledPinOperational, OUTPUT);
      
      
      digitalWrite(ledPinOperational, HIGH);
      digitalWrite(ledPin1, HIGH);
      Serial.begin(9600);
}
void loop() {
     

    //mecanism de debounce
    reading1 = digitalRead(buttonPin1);
    if (reading1 != lastReading1) {
      lastDebounceTime1 = millis();
    }
    if ((millis() - lastDebounceTime1) > debounceDelay) {
      if (reading1 != buttonState1) {
          buttonState1 = reading1;
      if (buttonState1 == HIGH) {
          calledfloor = 1;
          cntFirstIteration +=1;//to not enter the switch at first itteration
          
        }
      }
    }
 
    reading2 = digitalRead(buttonPin2);
    if (reading2 != lastReading2) {
        lastDebounceTime2 = millis();
    }
    if ((millis() - lastDebounceTime2) > debounceDelay) {
      if (reading2 != buttonState2) {
        buttonState2 = reading2;
        if (buttonState2 == HIGH) {
              calledfloor =2;
              cntFirstIteration +=1;
              
        }
    }
    }
    reading3 = digitalRead(buttonPin3);
    if (reading3 != lastReading3) {
      lastDebounceTime1 = millis();
    }
    if ((millis() - lastDebounceTime1) > debounceDelay) {
      if (reading3 != buttonState3) {
          buttonState3 = reading3;
      if (buttonState3 == HIGH) {
          calledfloor=3;
          cntFirstIteration +=1;
        }
    }
    }


    lastReading1 = reading1;
    lastReading2 = reading2;
    lastReading3 = reading3;

    startBlink = millis();
    
    //contor sa nu intre pentru prima iteratie, >3 sa nu aiba etaju 3 ca etaj default
    if(cntFirstIteration > 3) { 
        switch (calledfloor) {
          case 3:
             //currenFloor actualizat dupa ce a trecut intervalul de timp si a ajuns la etajul 3 
             //cand apas pe 3 si e deja la 3 nu mai face nimic
            if(currentFloor != 3){
                //every time the elevator is at a level different from 3 or in between floors the op led blinks 
                if (startBlink - lastBlink >= blinkingInterval) {
                   lastBlink = startBlink;
                  if (digitalRead(ledPinOperational) == LOW) {
                    opLedState = HIGH;
                  }
                  else {
                    opLedState = LOW;
                  }
                }
                unsigned long currentMillis = millis();
                //intra pt ca prima data ledsActivated e false
                if (!ledsActivated) {
                  tone(buzzerPin, 500, 300);
                  // Stingerea primului bec după 2 secunde
                  if (currentMillis - previousMillis1 >= movementDelay) {
                    previousMillis1 = currentMillis;
                    ledState1 = LOW;
                  }
                  // Aprinderea celui de-al doilea bec după 4 secunde
                  if (currentMillis - previousMillis2 >= 2*movementDelay) {
                    previousMillis2 = currentMillis;
                    ledState2 = HIGH;
                    ledsActivated = true;// Setăm flagul că ledurile au fost activate
                    // Serial.print("Aprins\n");
                  }//repeta cele 2 ifuri fara ledsActivated 
                }
    
              if (ledsActivated) {
                tone(buzzerPin, 500, 700);
                // Aprinderea celui de-al treilea bec după încă 2 secunde(dupa 6 sec)
                if (currentMillis - previousMillis3 >= 3*movementDelay) {
                  previousMillis3 = currentMillis;
                  ledState2 = LOW;
                  ledState1 = LOW;
                  // Serial.print("Stins\n");
                }
                //pt ca becul etajului dorit sa ramana aprins la sfarsit
                if (currentMillis - previousMillis4 >= 4*movementDelay) {
                  previousMillis4 = currentMillis;
                  ledState3 = HIGH;
                  currentFloor = 3;//ca sa nu mai intre deloc in if 
                  opLedState = HIGH;//to make the op led stay high when the elevator reaches the called floor 
                  tone(buzzerPin, 1000, 700);
                  ledsActivated = false;
                }
              }
            }
          break;

        case 1:
              if(currentFloor != 1){ 
                if (startBlink - lastBlink >= blinkingInterval) {
                   lastBlink = startBlink;
                  if (digitalRead(ledPinOperational) == LOW) {
                    opLedState = HIGH;
                  }
                  else {
                    opLedState = LOW;
                  }
                }
                unsigned long currentMillis = millis();
                //intra pt ca prima data ledsActivated e false
                if (!ledsActivated) {
                  tone(buzzerPin, 500, 500);
                  // Stingerea celui de al treiea bec la 2 sec dupa apasarea butonului 1
                  if (currentMillis - previousMillis3 >= movementDelay) {
                    previousMillis3 = currentMillis;
                    ledState3 = LOW;
                  }
                  if (currentMillis - previousMillis2 >= 2*movementDelay) {
                    previousMillis2 = currentMillis;
                    ledState2 = HIGH;
                    ledsActivated = true; 
                    
                  }
                }
              if (ledsActivated) {
                tone(buzzerPin, 500, 700);
                if (currentMillis - previousMillis1 >= 3*movementDelay) {
                  previousMillis1 = currentMillis;
                  ledState3 = LOW;
                  ledState2 = LOW;
                }
                //pt ca becul etajului dorit sa ramana aprins la sfarsit
                if (currentMillis - previousMillis4 >= 4*movementDelay) {
                  previousMillis4 = currentMillis;
                  ledState1 = HIGH;
                  currentFloor = 1;
                  opLedState = HIGH;
                  tone(buzzerPin, 1000, 700);
                  ledsActivated = false;
                }
              }
            }
      
            break;    
    
        default:
          // Caz implicit
          break;
        }
        
  }
  
  digitalWrite(ledPin1, ledState1);
  digitalWrite(ledPin2, ledState2);
  digitalWrite(ledPin3, ledState3);
  digitalWrite(ledPinOperational, opLedState);
     
  }
  

