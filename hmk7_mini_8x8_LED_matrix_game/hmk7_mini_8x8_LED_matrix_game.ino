#include "LedControl.h" // need the library
const byte dinPin = 12; // pin 12 is connected to the MAX7219 pin 1
const byte clockPin = 11; // pin 11 is connected to the CLK pin 13
const byte loadPin = 10; // pin 10 is connected to LOAD pin 12
const byte matrixSize = 8; // 1 as we are only using 1 MAX7219

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER

byte matrixBrightness = 4;

//matrix for the walls 
byte walls[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

//matrix that contains 1 in the current position of the player 
byte isBlinking[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
//matrix that contains 1 in the positions in which bombs are place(before they explode)
byte bombPlacement[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int crtPlayerRow;//row that has associated 1 in isBlinking matrix
int crtPlayerCol;//col that has associated 1 in isBlinking matrix

// declare all the pins for joystick
const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

byte swState = LOW;

int xValue = 0;
int yValue = 0;

unsigned long previousMillis = 0;

unsigned int playerBlinkingRate = 600;
unsigned int bombBlinkingRate = 300;

//variable used for blinking the player and the bomb
bool playerLedState = 0;
bool bombLedState = 0;

bool joyMoved = false;
int minThreshold = 400;
int maxThreshold = 600;

//variables used as direction indicators that are set according to the movement of the joystick
unsigned int right = 0;
unsigned int left = 0;
unsigned int up = 0;
unsigned int down = 0;

//used for generating the walls one, at tha start of the game
unsigned int generatedWalls = 0;

unsigned long previousMillisB = 0;
unsigned long previousMillisP = 0;
const unsigned int playerMoveInterval = 200; 

byte SwButtonState = HIGH;
//debounce for sw button
byte readingSw = HIGH;
byte lastReadingSw = HIGH;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

unsigned int placedBomb = 0;

//variables used for saving the position in which a bombed was placed 
unsigned int currentRow;
unsigned int currentCol;
unsigned int bombRow;
unsigned int bombCol;

//when a bomb explodes it distroys wals of length 3 in each direction
unsigned int nrToExplode = 3;

//used for setting a number of blinks until a bomb explodes
unsigned int currentBombTicks = 0, bombTicks = 15;

void setup() { 

    pinMode(pinSW, INPUT_PULLUP); // activate pull-up resistor on the // push-button pin

    // the zero refers to the MAX7219 number, it is zero for 1 chip
    lc.shutdown(0, false); // turn off power saving, enables display
    lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
    lc.clearDisplay(0);// clear screen
    Serial.begin(9600);

    randomSeed(analogRead(pinX));//seed random 

    //for making the initial position of the player random
    setInitialPlayerPosition();
}
void loop() {

    unsigned long currentMillis = millis();
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);

    //generate walls only at the start of the game 
    if(!generatedWalls){
      generatedWalls = 1;
      generateWalls();
    }

    //continuously blinks the led associated with the player 
    blinkPlayer(crtPlayerRow, crtPlayerCol);

    
    //function for setting the position where a bomb was placed 
    setBombPosition();

    //function that handles bomb associated led blinking and distroying the walls when the bomb explodes 
    explodeBomb();
    
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            //if this led is blinking
            if(isBlinking[row][col]){
                //sets the position variables according to joystick movements 
                movePlayer(row, col);
                //retains the position where a bomb was placed 
                currentRow = row;
                currentCol = col;
                //sets current position of the player
                setCrtPos(row, col);
                //sets the precedent position in isBlinking matrix to 0, since the player is not there anymore 
                isBlinking[row][col] = 0;
            }
    }
  }
  if (currentMillis - previousMillis >= playerMoveInterval) {
        previousMillis = currentMillis;
        isBlinking[crtPlayerRow][crtPlayerCol] = 1;
        //turns on the led for the player in the new position and updates isBlinking matrix
        player(crtPlayerRow, crtPlayerCol); 
        resetDirVariables();
  }
    
}

void setInitialPlayerPosition(){
    crtPlayerRow = random(matrixSize);
    crtPlayerCol = random(matrixSize);
    isBlinking[crtPlayerRow][crtPlayerCol] = 1;
}

void setBombPosition(){
  //debouce for sw button 
    readingSw = digitalRead(pinSW);
   
    if(readingSw != lastReadingSw){
          lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      if(readingSw != SwButtonState){
              SwButtonState = readingSw;
              if(SwButtonState == HIGH){
                  placeBombWarning();
                  bombPlacement[currentRow][currentCol] = 1;
                  bombRow = currentRow;
                  bombCol = currentCol;
              }
          }
    }
    lastReadingSw = readingSw;
}
void explodeBomb(){

  unsigned long currentMillis = millis();

  if(bombPlacement[bombRow][bombCol]) {
      lc.setLed(0, bombRow, bombCol, bombLedState);
      if (currentMillis - previousMillisB >= bombBlinkingRate) {
            previousMillisB = currentMillis;
            bombLedState = !bombLedState;
            currentBombTicks++;
      }
  }
  //if ticks until explode have passed
  if(currentBombTicks == bombTicks) {
    //set with the number of leds that can be turned off according to the position in the matrix
    int top, right, bottom, left;

    //for top
    if(bombRow < nrToExplode) {
      top = bombRow;
    } else {
      top = 3;
    }
    for(int i = 0; i<=top; i++){
       lc.setLed(0, bombRow - i, bombCol, false);
       walls[bombRow - i][bombCol] = 0;
    }
    //for right
    if(matrixSize - bombCol < nrToExplode) {
      right = matrixSize - bombCol;
    } else {
      right = 3;
    }
    for(int i = 0; i<=right; i++){
       lc.setLed(0, bombRow, bombCol + i, false);
       walls[bombRow][bombCol + i] = 0;
    }
    //for bottom
    if(matrixSize - bombRow < nrToExplode) {
      bottom = matrixSize - bombRow;
    } else {
      bottom = 3;
    }
    for(int i = 0; i<=bottom; i++){
       lc.setLed(0, bombRow + i, bombCol, false);
       walls[bombRow + i][bombCol] = 0;
    }
    //for left
    if(bombCol < nrToExplode) {
      left = bombCol;
    } else {
      left = 3;
    }
    for(int i = 0; i<=left; i++){
       lc.setLed(0, bombRow, bombCol - i, false);
       walls[bombRow][bombCol - i] = 0;
    }
    //bomb exploded so is not there anymore 
    bombPlacement[bombRow][bombCol] = 0;
    //retets bomb ticks
    currentBombTicks = 0;
    Serial.println("Kaboom");

    lc.setLed(0, bombRow, bombCol, false);
  }
}

void placeBombWarning(){
    Serial.println("Bomb placed!");
}

void resetDirVariables(){
  left = 0;
  right = 0;
  up = 0; 
  down = 0;
}

void player(int newRow, int newCol) {
    static int prevRow = -1;
    static int prevCol = -1;
    
    // Turn off the previously lit LED if it was initialized, third condition for not turning off the led associated with a bomb 
    if (prevRow >= 0 && prevCol >= 0 && bombPlacement[prevRow][prevCol] == 0) {
          lc.setLed(0, prevRow, prevCol, false);
    }
    // Update the previous row and column
    prevRow = newRow;
    prevCol = newCol;
    // Turn on the new LED
    lc.setLed(0, newRow, newCol, true);
}

void movePlayer(int row, int col){
      //move player down
      if(xValue < minThreshold && joyMoved == false){
            down = 1;
            Serial.println("down");
            joyMoved = true;
            xValue = 0;
      }
      //move player up
      if(xValue > maxThreshold && joyMoved == false){
              up = 1;
              Serial.println("up");
              joyMoved = true;
              xValue = 0;
            }
      //move player right
      if(yValue > maxThreshold && joyMoved == false){
              right = 1;
              Serial.println("to right");
              joyMoved = true;
              yValue = 0;
            }
      //move player left
      if(yValue < minThreshold && joyMoved == false){
              left = 1;
              Serial.println("to left");
              joyMoved = true;
              yValue = 0;  
            }

    if (xValue >= minThreshold && xValue <= maxThreshold) {
      joyMoved = false;  
    }
    if (yValue >= minThreshold && yValue <= maxThreshold) {
      joyMoved = false;  
    }
}

// Function to generate walls randomly on 50% of the matrix surface
void generateWalls() {
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
          //no walls in close proximity of the player
          //formula source(chat gpt)
          int distanceToPlayer = abs(row - crtPlayerRow) + abs(col - crtPlayerCol);
          // Randomly decide whether to set the wall (50% chance)
          if (distanceToPlayer >= 3) {  // Adjust the distance as needed
              walls[row][col] = random(2);
              lc.setLed(0, row, col, walls[row][col]);
          }
        }
    }
    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            lc.setLed(0, row, col, walls[row][col]);
        }
    }
}

void setCrtPos(int row, int col) {
  //move player only if in the new pos is not a wall
  if (down == 1 && row + 1 < matrixSize && walls[row + 1][col] == 0) {
    crtPlayerRow = row + 1;
    crtPlayerCol = col;
  }
  if (up == 1 && row - 1 >= 0 && walls[row - 1][col] == 0) {
    crtPlayerRow = row - 1;
    crtPlayerCol = col;
  }
  if (right == 1 && col + 1 < matrixSize && walls[row][col + 1] == 0) {
    crtPlayerRow = row;
    crtPlayerCol = col + 1;
  }
  if (left == 1 && col - 1 >= 0 && walls[row][col - 1] == 0) {
    crtPlayerRow = row;
    crtPlayerCol = col - 1;
  }
}

void blinkPlayer(int row, int col){
  unsigned long currentMillis = millis();
  if(!bombPlacement[row][col]) {
  lc.setLed(0, row, col, playerLedState);
  if (currentMillis - previousMillisP >= playerBlinkingRate) {
          previousMillisP = currentMillis;
          playerLedState = !playerLedState;
    }
  }
}
