//variables for current main option 
int currentMainOption = 0;
int currentSecondOption = 0;

String inputString = ""; 
char mainChoice = 0;
char secondChoice = 0;
//constrol variables 
int inSubmenu = 0;
char requestedAction = 0;
int promptedNumber = 0;
int activeSubMenuOpt = 0;
int activeSubMenuOpt11 = 0;
int manualColorContol = 0;
int automaticOnOff = 0;
int resetOn=0;
int displaySetting = 0;
int currentSensorReading = 0;

//for sampling sansor data according to the sampling rate 
unsigned long previousMillis = 0;
unsigned long previousMillisForLDR = 0;

// will get the value of the prompted number when 1.1 option is selected 
long samplingRate; 

//variables for ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;
long duration = 0;
int distance = 0;
int setSamplingRate = 0;

//value used to set the minimum distance
int  thresholdMinValue = 0;
int setThresholdMinValue = 0;


//values used to set minimum value as a sign for night coming
int thresholdMinValueLDR = 0;
int nightComing = 0;

//RGB led pins
const int redledPin = 11;
const int greenledPin = 6;
const int blueledPin = 5;

//for LDR
const int photocellPin = 0; // the cell and 10K pulldown are connected to a0
int photocellValue; // the analog reading from the sensor divider

void setup() {

      pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
      pinMode(echoPin, INPUT); // Sets the echoPin as an Input
      pinMode(redledPin, OUTPUT);
      pinMode(greenledPin, OUTPUT);
      pinMode(blueledPin, OUTPUT);

      // Initialize serial communication at 9600 bits per second
      Serial.begin(9600);

      // Display the initial menu interface to the Serial Monitor upon startup
      printMenu();
}

void loop() {
      
      navigateMenu();
      ultrasonicSensorResponse();
      alertAtTresholdValue();
      ldrResponse();
      alertNightComing();
      if(resetOn == 1){
        resetSensorsData();
      }
      if(displaySetting == 1 && currentSecondOption == 2 && currentMainOption == 3){
        displaySensorSettings();
      }
      
  
}
void ultrasonicSensorResponse(){
    if(setSamplingRate == 1){
        unsigned long currentMillis = millis();
        
        // Serial.print("SamplingRate: ");
        // Serial.println(samplingRate);
        //it is given the number of seconds, so samplingRate must be multiplied by 1000 for miliseconds
        if(currentMillis - previousMillis >= samplingRate*1000) {
          // Save the last time we took a measurement
          previousMillis = currentMillis;
          // Clears the trigPin
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          // Sets the trigPin on HIGH state for 10 micro seconds
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          // Reads the echoPin, returns the sound wave travel time in microseconds
          duration = pulseIn(echoPin, HIGH);
          // Sound wave reflects from the obstacle, so to calculate the distance we
          // consider half of the distance traveled.
          distance = duration*0.034/2;
          //displayed only when 1.2 option is active
          //when 3.1 is active 
          if(currentSensorReading == 1 && currentSecondOption == 1 && currentMainOption == 3){
              // Prints the distance on the Serial Monitor
              Serial.print("Distance = ");
              Serial.println(distance);
          }
         
        }
    }
}

void alertAtTresholdValue(){
  if(setThresholdMinValue == 1){
    if(distance <= thresholdMinValue){
         Serial.println("!!!!!!! MINIMUM DISTANCE HAS BEEN REACHED !!!!!!!");
    }
      if(distance <= thresholdMinValue || photocellValue <= thresholdMinValueLDR){
        
         //if led is in automatic mode, it should turn red at this moment, otherwise it should remain green
         if(automaticOnOff == 1){
          digitalWrite(greenledPin, LOW);
          digitalWrite(redledPin, HIGH);
         }
      }
      else {
        digitalWrite(redledPin, LOW);
        digitalWrite(greenledPin, HIGH);
      }
  }
}
void ldrResponse(){
  if(setSamplingRate == 1){
    
      unsigned long currentMillis = millis();  
      //it is given the number of seconds, so samplingRate must be multiplied by 1000 for miliseconds
      if(currentMillis - previousMillisForLDR >= samplingRate*1000) {
          // Save the last time we took a measurement
          previousMillisForLDR = currentMillis;
          photocellValue = analogRead(photocellPin);
          //when 3.1 is activ
          if(currentSensorReading == 1 && currentSecondOption == 1 && currentMainOption == 3){
            Serial.print("LDR response = ");
            Serial.println(photocellValue); // the raw analog reading
          }
      }
  }
}

void alertNightComing(){
  if(nightComing == 1){
    if(photocellValue < thresholdMinValueLDR){
      Serial.println("!!!!!!! NIGHT IS COMING !!!!!!!");
    }
    //for cases when thr values are not reached at the same time 
      if(photocellValue < thresholdMinValueLDR || distance <= thresholdMinValue){
         
         //if led is in automatic mode, it should turn red at this moment, otherwise it should remain green
         if(automaticOnOff == 1){
          digitalWrite(greenledPin, LOW);
          digitalWrite(redledPin, HIGH);
         }
      }else {
        digitalWrite(redledPin, LOW);
        digitalWrite(greenledPin, HIGH);
      }
  }
}
void navigateMenu(){
      // Continuously checks for incoming serial data
      if (Serial.available()) {

        if(!inSubmenu){
            mainChoice = (char) Serial.read();
            inputString += mainChoice;

            if (mainChoice == '\n') {
                currentMainOption = inputString.toInt();
                printSubmenu(currentMainOption);
                inSubmenu = 1;
                inputString = "";
            }
        }
        else {    
          //if no submenu option is active then a submenu option can be selected 
              if(!activeSubMenuOpt){
                      secondChoice = (char) Serial.read();
                      inputString += secondChoice;

                      if (secondChoice == '\n') {
                          currentSecondOption = inputString.toInt();
                          //says the chosen option and sets flags for actions
                          takeAction(currentMainOption, currentSecondOption);//activeSubMenuOpt = 1; in take action
                          inputString = "";
                      }
                      
              }else{
                      if(currentMainOption == 1){
                            getAndUsePromptedValue();//used to get and use user input 
                      }
                }
              }   
        }
}
void getAndUsePromptedValue(){

        requestedAction = (char) Serial.read();
        inputString += requestedAction;

        if (requestedAction == '\n') {
        promptedNumber = inputString.toInt();
        Serial.print("Introduced number: ");
        Serial.println(promptedNumber);
        Serial.println("Introduce 4 for main menu");
        usePromptedValue();

        inputString = "";
        activeSubMenuOpt = 0;//0 here to go select another option in the sub menu 
    }
  
 
}

void usePromptedValue(){
      switch(currentSecondOption){
          case 1: useAsSamplingRate();//same sampling rate for both sensors 
          case 2: setThresholdUltrasonic();
          case 3: setThresholdLDR();
      }
}
void useAsSamplingRate(){
    samplingRate = promptedNumber;
    setSamplingRate = 1;
}

//when something is too close an alert is given
void setThresholdUltrasonic(){
    //promptedNumber seen as a min value;
    thresholdMinValue = promptedNumber;
    setThresholdMinValue = 1;
}
void setThresholdLDR(){
    thresholdMinValueLDR = promptedNumber;
    nightComing = 1;
}
// Function to display a menu of options to the user
void printMenu() {
  Serial.println("Main Menu:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
}

// Function to print different messages based on the user's selection
void printSubmenu(int option) {
  
  switch (option) {
    case 1:
          Serial.println("\n1. Sensor Settings: ");
          Serial.println("      1.1 Sensors Sampling Interval.");
          Serial.println("      1.2 Ultrasonic Alert Threshold.");
          Serial.println("      1.3 LDR Alert Threshold.");
          Serial.println("      1.4 Back");
          
      break;

    case 2:
          Serial.println("\n2. Reset Logger Data: ");
          Serial.println("Are you sure you want to delete all data?");
          Serial.println("    2.1 Yes.");
          Serial.println("    2.2 No.");
      break;

    case 3:
          Serial.println("\n3. System Status: ");
          Serial.println("      3.1 Current Sensor Readings.");
          Serial.println("      3.2 Current Sensor Settings.");
          Serial.println("      3.3 Display Logged Data.");
          Serial.println("      3.4 Back.");
      break;

    case 4:
          Serial.println("\n4. RGB LED Control: ");
          Serial.println("      4.1 Manual Color Control.");
          Serial.println("      4.2 LED: Toggle Automatic ON/OFF.");
          Serial.println("      4.3 Back");
      break;
    
    default:
      // Handles any choices outside the defined options
      Serial.println("Invalid main option selected\n");
  }
}
void resetSensorsData(){
    Serial.println("Deleted sensors data!");
    samplingRate = 200;//a max value to not sample
    thresholdMinValue = 0;
    thresholdMinValueLDR = 0;
    resetOn=0;
}
void displaySensorSettings(){
  
  
    //Serial.print("Sampling rate: ");
    Serial.println(samplingRate);
    // Serial.print("Threshold value for ultrasonic sensor: ");
    Serial.println(thresholdMinValue);
    // Serial.print("Threshold value for LDE:");
    Serial.println(thresholdMinValueLDR);
    
  
   
}
//function for actioning according to the selected option in the submenu
void takeAction(int mainOption, int secondOption){
    if(mainOption == 1){
      switch(secondOption){

        case 1:  Serial.println("     Chosen option: 1.1 Sensors Sampling Interval.");
                 Serial.println("Introduce Number between 1 and 10 seconds! ");
                 activeSubMenuOpt = 1;
                 

        break;

        case 2: Serial.println("      Chosen option: 1.2 Ultrasonic Alert Threshold.");
                Serial.println("Introduce threshold value for the ultrasonic sensor! ");
                activeSubMenuOpt = 1;
        break;

        case 3: Serial.println("      Chosen option: 1.3 LDR Alert Threshold.");
                Serial.println("Introduce threshold  value  for  the  LDR  sensor! ");
                activeSubMenuOpt = 1;
        break;

        case 4:Serial.println("       Chosen option: 1.4 Back.");
               printMenu(); 
               inSubmenu=0;
               currentMainOption=0;
               currentSecondOption = 0;
        break;

        default: Serial.println("Invalid second choice!"); 
        break;
      }
    }
    if(mainOption == 2){
     
      switch(secondOption){
        case 1: Serial.println("      Chosen option: 2.1 Yes.");
                Serial.println("Press 3 to return to main menu.");
                resetOn = 1;
              
        break;
        //if user chooses to not delete sensor data it will be automatically directed to the main menu
        case 2: Serial.println("      Chosen option: 2.2 No.");
                printMenu(); 
                inSubmenu=0;
                currentMainOption = 0;
                currentSecondOption = 0;
        break;

        case 3: Serial.println("      Exit!");
                printMenu(); 
                inSubmenu=0;
                currentMainOption = 0;
                currentSecondOption = 0;
        break;

        default: Serial.println("Invalid second choice!"); 
        break;
      }
    }
    if(mainOption == 3){
      switch(secondOption){

        case 1: Serial.println("      Chosen option: 3.1 Reading sensors, press 4 to exit");
                currentSensorReading = 1;
        break;

        case 2: Serial.println("      Chosen option: 3.2 Current Sensor Settings.");
                displaySetting = 1;
        break;

        case 3: Serial.println("      Chosen option: 3.3 Display Logged Data..");
        break;

        case 4: Serial.println("      Chosen option: 3.4 Back.");
                currentSensorReading = 0;
                printMenu(); 
                inSubmenu=0;
                currentMainOption = 0;
                currentSecondOption = 0;
                
        break;

        default: Serial.println("Invalid second choice!"); 
        break;
      }
    }
    if(mainOption == 4){
      switch(secondOption){

        case 1: Serial.println("       Chosen option: 4.1 Manual Color Control.");
                manualColorContol = 1;
                automaticOnOff = 0;
                Serial.println("AUTOMATIC CONTROL OF RGB OFF");

        break;

        case 2: Serial.println("       Chosen option: 4.2 LED: Toggle Automatic ON/OFF.");
                automaticOnOff = 1;
                manualColorContol = 0;
                digitalWrite(greenledPin, HIGH);
                Serial.println("AUTOMATIC CONTROL OF RGB ON");
        break;

        case 3:Serial.println("        Chosen option: 4.3 Back.");
               printMenu(); 
               inSubmenu=0;
               currentMainOption = 0;
               currentSecondOption = 0;
        break;

        default: Serial.println("Invalid second choice!"); 
        break;
      }
    }
}
