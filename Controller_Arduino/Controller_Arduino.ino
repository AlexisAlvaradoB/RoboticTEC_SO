#include <Stepper.h>
#include "Button.h"

#define TOTAL_STATES 4
#define NINETY_DEGREE 509

const int stepSize = 1;
const int stepsPerRevolution = 2038;
Stepper stprRot = Stepper(stepsPerRevolution, 2, 4, 3, 5);
Stepper stprNdle = Stepper(stepsPerRevolution, 8,10,9,11);

int base_speed = 15;

Button bot_calibra = Button(13, 10);
Button bot_opcion = Button(12, 500);



int executionState = 0;
float translacion = 0;

int current_letter = 0; //Está parado sobre la letra A


void setup() {
  Serial.begin(9600);
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  stprRot.setSpeed(base_speed);
  stprNdle.setSpeed(base_speed);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  setLights(0);
}

void loop() {
  if(bot_opcion.validPress()){
    executionState++;
    if (executionState >= TOTAL_STATES) {
      executionState = 0;
    }
    setLights(executionState);
  }
  if(bot_calibra.validPress()){
    if(executionState == 0) {
      stprRot.step(1);
    }
    if (executionState == 1) {
      stprNdle.step(1);
    }if (executionState == 2){
      stprNdle.step(-509);
      executionState++;
    }
  }
  if(Serial.available() > 0 && executionState ==3 ){
    String message = Serial.readString();
    Serial.print("I recieved: ");
    Serial.print(message);
    Serial.print(" length: ");
    Serial.println(message.length());
    int largo = message.length() - 1;
    int * numbers = new int[largo];
    numbers = getNumbers(message, numbers);
    ejecutaPalabra(numbers, largo);
    delete[] numbers;
    numbers = nullptr;
  }
}

void setLights(int state){
  int light_state [2] = {0,0};
  switch(state){
    case 1:
      light_state[0] = 1;
      break;
    case 2:
      light_state[1] = 1;
      break;
    case 3:
      light_state[0] = 1;
      light_state[1] = 1;
      break;
    default:
      break;
  }
  digitalWrite(7, light_state[0]);
  digitalWrite(6, light_state[1]);
}

int ejecutaPalabra(int * palabra, int largo_palabra){
  int curr = current_letter;
  for(int i = 0; i < largo_palabra; i++){
    int distance = palabra[i] - curr;
    if(distance > 13){
      distance -= 27;
    }
    if(distance < -13){
      distance += 27;
    }
    int move = round(75.5 * distance);
    stprRot.step(-move);
    stprNdle.step(NINETY_DEGREE);
    delay(500);
    stprNdle.step(-NINETY_DEGREE);
    delay(500);
    curr = palabra[i];
  }
  return curr;
}

int * getNumbers(String palabra, int * numbers){
  int length = palabra.length() - 1;
  for(int i = 0; i < length; i++){
    switch(palabra[i]){
      case 'a':
        (numbers[i]) = 0;
        break;
      case 'b':
        (numbers[i]) = 1;
        break;
      case 'c':
        (numbers[i]) = 2;
        break;
      case 'd':
        numbers[i] = 3;
        break;
      case 'e':
        (numbers[i]) = 4;
        break;
      case 'f':
        (numbers[i]) = 5;
        break;
      case 'g':
        (numbers[i]) = 6;
        break;
      case 'h':
        (numbers[i]) = 7;
        break;
      case 'i':
        (numbers[i]) = 8;
        break;
      case 'j':
        (numbers[i]) = 9;
        break;
      case 'k':
        (numbers[i]) = 10;
        break;
      case 'l':
        (numbers[i]) = 11;
        break;
      case 'm':
        (numbers[i]) = 12;
        break;
      case 'n':
        (numbers[i]) = 13;
        break;
      case 'ñ':
        (numbers[i]) = 14;
        break;
      case 'o':
        (numbers[i]) = 15;
        break;
      case 'p':
        (numbers[i]) = 16;
        break;
      case 'q':
        (numbers[i]) = 17;
        break;
      case 'r':
        (numbers[i]) = 18;
        break;
      case 's':
        (numbers[i]) = 19;
        break;
      case 't':
        (numbers[i]) = 20;
        break;
      case 'u':
        (numbers[i]) = 21;
        break;
      case 'v':
        (numbers[i]) = 22;
        break;
      case 'w':
        (numbers[i]) = 23;
        break;
      case 'x':
        (numbers[i]) = 24;
        break;
      case 'y':
        (numbers[i]) = 25;
        break;
      case 'z':
        (numbers[i]) = 26;
        break;
    }
  }
  return numbers;
}




 
