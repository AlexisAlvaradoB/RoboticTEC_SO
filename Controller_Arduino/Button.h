#ifndef BUTTON_H
#define BUTTON_H

  class Button{
    public:
      Button(int pin, long span);
      bool validPress();
    private:
      unsigned long time_since_last_push;
      int pin;
      long time_span = 10;
  };

#endif