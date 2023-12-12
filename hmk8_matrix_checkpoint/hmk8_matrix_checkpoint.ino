#include <LiquidCrystal.h>
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int lcdRows = 2;
int lcdCols = 16;
int xValue = 0;
int yValue = 0;
// declare all the pins for joystick
const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

byte SwButtonState = HIGH;
//debounce for sw button
byte readingSw = HIGH;
byte lastReadingSw = HIGH;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

const int minThreshold = 300; // Adjust this threshold based on your joystick
const int maxThreshold = 700; // Adjust this threshold based on your joystick


int joyMoved = false;
unsigned long previousMillisAbt;
int navigateMenuOn = 0;
int displayIntroTime = 3000;
int displayOnLcdInterval = 70;//message was flickerring witout it 
byte star[8] = {
 B00000,
  B10101,
  B01110,
  B11011,
  B01110,
  B10101,
  B00000,
  B00000
};
byte arrowDownUp[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};
byte optionArrow[8] = {
  B01000,
  B00100,
  B00010,
  B00001,
  B00010,
  B00100,
  B01000,
  B00000
};
unsigned long startTime = 0;
unsigned long previousMillis = 0;
int crtMainOption, crtMainOptionDisplayed = 0;
int crtSecondOption, crtSecondOptionDisplayed = 0;
int scrollPrevMillis=0;
int subMenuOn=0;
int lcdBrightness;
int matrixBrightness;
int inAbout = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(pinSW, INPUT_PULLUP);
  lcd.begin(lcdCols, lcdRows);
  lcd.createChar(0, star);  // Create the custom character after lcd.begin()
  lcd.createChar(1, arrowDownUp);  
  lcd.createChar(2, optionArrow);
  startTime = millis();
  Serial.begin(9600);
}

void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - startTime <= displayIntroTime) {
        DisplayIntroMsg();
  }else{
    if(currentMillis - previousMillis >= displayOnLcdInterval && navigateMenuOn == 0 && crtMainOption == 0){
      previousMillis = currentMillis;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Press for menu");

    }
  }
  readingSw = digitalRead(pinSW);
   
  if(readingSw != lastReadingSw){
        lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if(readingSw != SwButtonState){
              SwButtonState = readingSw;
              if(!navigateMenuOn && !subMenuOn && SwButtonState == HIGH){
                  navigateMenuOn = 1;
                  Serial.println("in menu");
              }
              if(navigateMenuOn && !subMenuOn && SwButtonState == HIGH){
                  crtMainOption = crtMainOptionDisplayed;
                  Serial.print("crtMainOption: ");
                  Serial.println(crtMainOption);
              }
              if(navigateMenuOn && !subMenuOn && crtMainOption == 2 && SwButtonState == HIGH){
                  subMenuOn = 1;
                  Serial.println("activated submenu");
              }
              if(navigateMenuOn && subMenuOn && SwButtonState == HIGH){
                  crtSecondOption = crtSecondOptionDisplayed;
                  Serial.print("crtSecondOption: ");
                  Serial.println(crtSecondOption);
              }
               if(navigateMenuOn && subMenuOn && crtSecondOption == 3 && SwButtonState == HIGH){
                  subMenuOn = 0;
                  crtSecondOption = 0;// no scnd option currently displayed or selected
                  crtSecondOptionDisplayed = 0;
                  Serial.println("submenu dezactivated");
              }
              if(navigateMenuOn && subMenuOn && crtSecondOption == 1 && SwButtonState == HIGH){
                  Serial.println("Saving matrix br...");
              }
              if(navigateMenuOn && subMenuOn && crtSecondOption == 2 && SwButtonState == HIGH){
                  Serial.println("Saving LCD br...");
              }
              if(navigateMenuOn && !subMenuOn && crtMainOption == 3 && SwButtonState == HIGH){  
                printAbout();
                inAbout = 1;
                lcd.clear();
                navigateMenuOn = 0;
              }
              if(!navigateMenuOn && inAbout && SwButtonState == HIGH){
                navigateMenuOn = 1;
                crtMainOption = 0;
                crtMainOptionDisplayed=0;
                lcd.clear();
                displayOption();
                Serial.print("out of about");
              }
          }
    }
  lastReadingSw = readingSw;
    
  
  if(navigateMenuOn){
    if(currentMillis - previousMillis >= displayOnLcdInterval){
      previousMillis = currentMillis;
      lcd.clear();
      displayJoystickDirections();
      displayArrowOption();
      lcd.setCursor(3, 0);
      lcd.print("Main Menu: ");
      displayOption();
      //if button is pressed then crt =Main option takes tha value of te displayed opt 
    }
  }
 if(currentMillis - scrollPrevMillis >= 250){
    scrollPrevMillis = currentMillis;
    // Read joystick values
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    // Check joystick movements
    if(xValue < minThreshold && joyMoved == false) {
      navigateDown();
      Serial.println("down");
      joyMoved = true;
    }

    if (xValue > maxThreshold && joyMoved == false) {
      navigateUp();
      Serial.println("up");
      joyMoved = true;
    }

    // Reset joyMoved flag when joystick is back to the center
    if (xValue >= minThreshold && xValue <= maxThreshold) {
      joyMoved = false;
    }

    if (yValue >= minThreshold && yValue <= maxThreshold) {
      joyMoved = false;
    }
  }
    
  
  
}

void navigateDown() {
  // Implement logic for moving down in the menu
  if(!subMenuOn){
     if(crtMainOptionDisplayed < 3){
       crtMainOptionDisplayed++;
      }
      else crtMainOptionDisplayed = 1;
      
  }else{
     if(crtSecondOptionDisplayed < 3){
       crtSecondOptionDisplayed++;
      }
      else crtSecondOptionDisplayed = 1;
  }
 
}

void navigateUp() {
  // Implement logic for moving up in the menu
  if(!subMenuOn){
      if(crtMainOptionDisplayed > 1){
        crtMainOptionDisplayed--;
      }
      else crtMainOptionDisplayed = 3;
     
      
  }else{
      if(crtSecondOptionDisplayed > 1){
        crtSecondOptionDisplayed--;
      }
      else crtSecondOptionDisplayed = 3;
  }
 
}


void DisplayIntroMsg(){
  lcd.setCursor(0, 0);
  lcd.print("Dive into matrix");
  lcd.setCursor(2, 1);
  lcd.write(byte(0));  // Display the custom character with index 0
  lcd.setCursor(3, 1);
  lcd.print("BOMBERMAN");
  lcd.setCursor(12, 1);
  lcd.write(byte(0));  // Display the custom character with index 0
}

void displayOption() {
  lcd.setCursor(4, 1);
  if(!subMenuOn){
    switch(crtMainOptionDisplayed){
        case 1: lcd.print("Start Game");
                break;
        case 2: lcd.print("Settings");
                break;
        case 3: lcd.print("About");
                break;
        default: lcd.print("Start Game");//by default show the first option
        break;
            
    }
  }else{
     switch(crtSecondOptionDisplayed){
        case 1: lcd.print("Matrix Br");
                break;
        case 2: lcd.print("LCD Br");
                break;
        case 3: lcd.print("Back");
            break; 
        default:lcd.print("Matrix Br");//by default show the first option
        break;
    }
  }
 
}

void displayJoystickDirections(){
  lcd.setCursor(15, 1);
  lcd.write(byte(1));
}


void displayArrowOption(){
  lcd.setCursor(3, 1);
  lcd.write(byte(2));
}

void printAbout() {
  lcd.clear();

  
  // Add your "About" text here
  String aboutText = "navigate a maze";
  aboutText += " and place bombs";
  
  lcd.setCursor(0, 0);
  lcd.print(aboutText);

  // Scroll the text using the autoscroll function
  for (int i = 0; i < aboutText.length() + lcdCols - 1; ++i) {
     
      lcd.scrollDisplayLeft();
      delay(400); // Adjust the delay time for scrolling speed
  }

  // Clear the screen after auto-scrolling
  lcd.clear();
}


