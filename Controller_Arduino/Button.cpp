#include "HardwareSerial.h"
#include "Arduino.h"
#include "Button.h"

Button::Button(int pin_set, long time_span_set){
  pin = pin_set;
  time_span = time_span_set;
}

bool Button::validPress(){
  if(digitalRead(pin) && millis() - time_since_last_push > time_span){
    time_since_last_push = millis();
    
    return true;
  }else{
    return false;
  }
}