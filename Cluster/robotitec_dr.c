#include <stdlib.h>

void send_to_arduino(int length, char * word){
    system("stty -F /dev/ttyACM0 -hupcl");
    int length_command = length + 29;
    char * command_base = "echo \"\" > /dev/arduino_driver";
    char echoOut [length_command];
    for(int i = 0; i < length_command; i++){
        if(i <= 5 || i >=length + 6){
            if(i >= length + 5){
                echoOut[i] = command_base[i-length];
            }else{
                echoOut[i] = command_base[i];
            }
        }else{
            echoOut[i] = word[i-6];
        }
    }
    system(echoOut);
    system("cat /dev/arduino_driver > /dev/ttyACM0");
}