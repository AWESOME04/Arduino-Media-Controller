/* 
Summary
The following sketch is for the Arduino Nano 33 BLE SENSE board, it uses the internal gesture sensor(APDS99690) for identifying gestures and then sends appropriate keyboard commands using the USBHID features of the board to control music.
Serial cannot be used for debugging, hence the LED_BUILTIN is being used, if the code runs successfully you should see the LED_BUILTIN stay lit up.
*/
#include <Arduino.h>
#include "mbed.h"
#include "USBKeyboard.h"
#include <Arduino_APDS9960.h>
USBKeyboard key;
// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) {
 return 0;
}
// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }
void setupUSB() __attribute__((weak));
void setupUSB() { }
void setup() {
 pinMode(13, OUTPUT);
 digitalWrite(13, HIGH);
 if (!APDS.begin()) { 
   digitalWrite(13, LOW);
 }
}
void loop() {
 if (APDS.gestureAvailable()) {
   int gesture = APDS.readGesture();
   switch (gesture) {
     case GESTURE_UP:
       key.media_control(KEY_PLAY_PAUSE);
       break;
     case GESTURE_DOWN:
       key.media_control(KEY_MUTE);
       break;
     case GESTURE_LEFT:
       key.media_control(KEY_PREVIOUS_TRACK);
       break;
     case GESTURE_RIGHT:
       key.media_control(KEY_NEXT_TRACK);
       break;
     default:
       break;
   }
 }
}
int main(void)
{
 init();
 initVariant();
#if defined(USBCON)
 USBDevice.attach();
#endif
 setup();
 for (;;) {
   loop();
   if (serialEventRun) serialEventRun();
 }
 return 0;
}