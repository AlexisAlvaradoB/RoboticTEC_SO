- -- - - - --  -- DRIVER - - --  -- - - -- - - -- - 

Requerimientos: 
En caso de tener errores con linux/init.h, ejecutar lo siguiente:
    >sudo apt update
    >sudo apt install linux-headers-generic

- Cargar el driver:

Sobre la carpeta RoboticTEC_SO/driver ejecutar
    > make

Luego, se carga el dispositivo al kernel a los espacios para drivers de dispositivos de caracteres con:
    >sudo insmod module_driver.ko

Revisar que se ha cargado el dispositivo con:
    > cat /proc/devices | grep arduino_hand_driver

- Desmontar el driver:

    >sudo rmmod module_driver

- Crear archivo de dispositivo:

    >sudo mknod /dev/arduino_hand c [MAJOR] 0  //Ej: >sudo mknod /dev/arduino_hand c 95 0
    > ls /dev/arduino_driver -al

- Revisar los mensajes que deja en el kernel el driver con printk:

    >sudo dmesg | tail