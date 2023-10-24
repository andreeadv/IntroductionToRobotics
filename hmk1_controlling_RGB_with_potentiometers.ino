//declarare pini pentru potentiometre
const int greenpotPin = A0;
const int redpotPin = A1;
const int bluepotPin = A2;

//declarare pini RGB
const int greenledPin = 9;
const int redledPin = 10;
const int blueledPin = 11;

//valori potentiometre
int greenpotValue = 0;
int redpotValue = 0;
int bluepotValue = 0;

//valori pentru fiecare culoare a ledului RGB
int greenledValue = 0;
int redledValue = 0;
int blueledValue = 0;

void setup() {
  
      Serial.begin(9600);
      
}
void loop() {
  
      //citire valori potentiometre si atribuirea lor variabilelor corespunzatoare
      greenpotValue = analogRead(greenpotPin);
      redpotValue = analogRead(redpotPin);
      bluepotValue = analogRead(bluepotPin);

      //pentru scrierea de valori din intervalul [0...255]
      greenledValue = map(greenpotValue, 0, 1023, 0, 255);
      redledValue = map(redpotValue, 0, 1023, 0, 255);
      blueledValue = map(bluepotValue, 0, 1023, 0, 255);

      //setarea culorilor RGB
      analogWrite(greenledPin, greenledValue);
      analogWrite(redledPin, redledValue);
      analogWrite(blueledPin, blueledValue);

}