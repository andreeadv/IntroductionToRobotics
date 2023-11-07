
// Declarations of all the joystick pins
const int pinSW = 2; // Digital pin connected to switch output
const int pinX = A0; // A0 - Analog pin connected to X output
const int pinY = A1; // A1 - Analog pin connected to Y output
 
// Declarations of all the segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;
 
const int segSize = 8;
int index = 0;
 
bool commonAnode = false; 
 
byte state = HIGH;
 
bool flagReset = false;
byte dpState = LOW;
byte swState = HIGH;

 
int xValue = 0;
int yValue = 0;
 
bool joyMoved = false;
int minThreshold = 400;
int maxThreshold = 600;
 
byte actualSegStates[segSize] ={LOW, LOW, LOW, LOW,LOW,LOW,LOW,LOW};
 
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
 
int position=7;
 

int prevPressSw = 0;

int pressSw;
long pressTime = 0;

void setup() {
  
  pinMode(pinSW, INPUT_PULLUP);
  //Initialization of all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  Serial.begin(9600);
 
}
 
void loop() {
 
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSW);

 
  blink(segments[position], state);
  
  if(swState == LOW){
     
      Serial.println("button pressed");
      //reset segments 
      // if (pressTime == 0) {
      //    pressTime = millis();
      // }
      // if (millis() - pressTime >= 3000) {
      //   flagReset=true;
      //   for(int i=0; i < segSize; i++){
      //     actualSegStates[i] = LOW;
        
      //   }
      //   position = 7 ;
      // }
      pressSw++;
  }

  if(pressSw != prevPressSw ){
      actualSegStates[position] = !actualSegStates[position];
      prevPressSw = pressSw;
  }
  else {
      for(int i=0; i < segSize; i++){
      digitalWrite(segments[i],actualSegStates[i]);
    }
  }
 
  Serial.println(swState);
  
  switch(position){
 
    case 0: //left
            if(xValue > maxThreshold && joyMoved == false){
              position = 5;
              joyMoved = true;
              xValue = 0;
            }//right
            else if(xValue < minThreshold && joyMoved == false){
              position = 1;
              joyMoved = true;
              xValue = 0;
            }
            else if(yValue < minThreshold && joyMoved == false){
              position = 6;
              joyMoved = true;
              yValue = 0;  
            }
            break;
    case 1: 
            if(xValue > maxThreshold && joyMoved == false){
              position = 5;
              joyMoved = true;
              xValue = 0;
            }//up
            else if(yValue > maxThreshold && joyMoved == false){
              position = 6;
              joyMoved = true;
              yValue = 0;
            }
            else if(yValue < minThreshold && joyMoved == false){
              position = 6;
              joyMoved = true;
              yValue = 0;  
            }
            break;    
 
    case 2: 
            if(xValue > maxThreshold && joyMoved == false){
              position = 4;
              joyMoved = true;
              xValue = 0;
            }
            else if(yValue > maxThreshold && joyMoved == false){
              position = 6;
              joyMoved = true;
              yValue = 0;
            }//down
            else if(yValue < minThreshold && joyMoved == false){
              position = 3;
              joyMoved = true;
              yValue = 0;  
            }
            else if(xValue < minThreshold && joyMoved == false){
              position = 7;
              joyMoved = true;
              xValue = 0;
            }
            break;
    case 3:
            if(xValue > maxThreshold && joyMoved == false){
              position = 4;
              joyMoved = true;
              xValue = 0;
            }else if(yValue > maxThreshold && joyMoved == false){
              position = 6;
              joyMoved = true;
              yValue = 0;
            }else if(xValue < minThreshold && joyMoved == false){
              position = 2;
              joyMoved = true;
              yValue = 0;  
            }
            break;
    case 4: 
            if(xValue < minThreshold && joyMoved == false){
              position = 2;
              joyMoved = true;
              xValue = 0;
            }else if(yValue > maxThreshold && joyMoved == false){
              position = 6;
              joyMoved = true;
              yValue = 0;
            }else if(yValue < minThreshold && joyMoved == false){
              position = 3;
              joyMoved = true;
              yValue = 0;  
            }
            break;
    case 5: 
            if(xValue < minThreshold && joyMoved == false){
              position = 1;
              joyMoved = true;
              xValue = 0;
            }
            else if(yValue > maxThreshold && joyMoved == false){
              position = 0;
              joyMoved = true;
              yValue = 0;
            }
            else if(yValue < minThreshold && joyMoved == false){
              position = 6;
              joyMoved = true;
              yValue = 0;  
            }
            break;
    case 6: //up
            if(yValue > maxThreshold && joyMoved == false){
              position = 0;
              joyMoved = true;
              yValue = 0;
            }//down
            else if(yValue < minThreshold && joyMoved == false){
              position = 3;
              joyMoved = true;
              yValue = 0;  
            }
            break;
    case 7:  if(xValue > maxThreshold && joyMoved == false){
             position = 2;
             joyMoved == true;
             xValue = 0;
             }
             break;
    
    default: Serial.println("No matching state");
            break;
  }
  
  if (xValue >= minThreshold && xValue <= maxThreshold) {
    joyMoved = false;
  }
  if (yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }

}
 
 
void blink(int segment, byte segmentState){
  digitalWrite(segment, segmentState);
  delay(300);
  digitalWrite(segment, !segmentState);
  delay(300);
}