# Arduino-Media-Controller

This project involves using the internal APDS9960 Gesture Sensor to read gesture data and use the HID features to play/pause, mute/unmute, and change music tracks playing on your laptop. 

The datasheet for the Arduino Nano 33 BLE states that the device is HID compatible but the existing keyboard and mouse libraries by Arduino are not compatible with the Nano 33 BLE (as of now). 
The Nano 33 BLE boards have their own “USBHID” library that uses the Mbed OS HID functionality. Unfortunately, these libraries do not come with examples as of now.



Goals and objectives

•	Learn how to convert Nano 33 BLE SENSE into a HID device and send keyboard commands to the computer. The following areas were explored:

•	Home automation

•	Music

•	Remote control



Hardware and Software Required

•	An Arduino Nano 33 BLE Sense board

•	A Micro USB cable to connect the Arduino board to your desktop machine.

•	To program the board, I used the  Arduino IDE. 

•	Any compatible media player capable of playing mp3 or mp4 on a computer.




Getting Started with HID

Things worth noting:

When sketches with HID functionality are uploaded into the board, you cannot use serial communication and also cannot get the Arduino IDE to upload or talk to the board again.

You can get the board back into bootloader mode by pressing on the reset button twice in quick succession and then upload your new sketches. If the reset is done correctly, you should see the “L” LED pulsing to indicate it’s in bootloader mode. If you don’t see the LED pulsing, try the double reset again until you get the timing right. Once the board is in bootloader mode, you will see a port and be able to upload again. Note that the port number may change when it’s in bootloader mode, so make sure to select the correct port from the Arduino IDE’s Tools > Port menu before uploading.

While the USBHID supports both mouse and keyboard interfaces, I only focused on the keyboard implementation. 
This is an example code from the library files, note that the wait()function was removed from 1.5.1 initially found in the in-built library.

I used delay()  Instead as shown below:

#include "mbed.h"
#include "USBKeyboard.h"
USBKeyboard key;
void setup() {
   key.printf("Hello World\r\n");
}
void loop() {
}
int main(void)
{
 delay(100); // for the keyboard to have time to be recognized
 setup();
 while(1) loop();
}




Combining HID and Gesture Sensor


Project Setbacks

•	Since I have not worked with the APDS9960 before, I had to go through the inbuilt examples in the IDE for clarity. 

•	While the above code does work on its own, I was unable to get the APDS9960 to work with this code. 

Gesture directions for APDS9960are as follows:

•	UP: from USB connector towards antenna

•	DOWN: from antenna towards USB connector

•	LEFT: from analog pins side towards digital pins side

•	RIGHT: from digital pins side towards analog pins side

Therefore, I concluded that following implementation of the APDS9960 and the USBHID does not work.



#include "mbed.h"
#include "USBKeyboard.h"
#include <Arduino_APDS9960.h>
USBKeyboard key;
void setup() {
   pinMode(13,OUTPUT);
   key.printf("Hello World\r\n");
   if (!APDS.begin()) {
     digitalWrite(13,HIGH);
  }
}
void loop() {
 if (APDS.gestureAvailable()) {
   // a gesture was detected, read and print to serial monitor
   int gesture = APDS.readGesture();
   switch (gesture) {
     case GESTURE_UP:
       key.printf("UP\r\n");
       break;
     case GESTURE_DOWN:
       key.printf("DOWN\r\n");
       break;
     case GESTURE_LEFT:
       key.printf("LEFT\r\n");
       break;
     case GESTURE_RIGHT:
       key.printf("RIGHT\r\n");
       break;
     default:
       // ignore
       break;
   }
 }
}
int main(void)
{
 delay(100); // for the keyboard to have time to be recognized
 setup();
 while(1) loop();
}


Possible Solutions

•	I assume this is because of the fact that the Arduino core actually has a hidden main function that internally calls the setup() and loop(), by redefining the main() a few initializations which might be important for the working of the APDS9960 might get skipped which is causing the sensor to not work.

•	I dug into the Arduino Core files to see what the hidden main function calls, and replaced the main() I defined with the hidden main() function.


Results

The internal APDS9960 Gesture Sensor to read gesture data and use the HID features to play/pause, mute/unmute, and change music tracks playing on your laptop. 
![image](https://user-images.githubusercontent.com/102630199/219315299-5d62929f-6d54-4eef-ba52-56fbaba63b23.png)


Conclusion

Further development of this project can be used in piloting of drones. An Arduino Nano 33 BLE Sense can be used to control a Tello Drone using gestures to fly, land, and move left and right


References

[1] https://create.arduino.cc/projecthub/textzip/gesture-based-media-control-using-nano-33-ble-sense-1420fd?ref=part&ref_id=108462&offset=0

[2] https://github.com/arduino/ArduinoCore-mbed/tree/master/libraries/USBHID

[3] https://forum.arduino.cc/t/arduino-nano-33-ble-sense-hid/675014?_gl=1*1se8b2k*_ga*MTU4MDU1NzYyOC4xNjY1NTcxNjkx*_ga_NEXN8H46L5*MTY2NzQ2NDk5Mi41NC4xLjE2Njc0NjY3MzkuMC4wLjA.
