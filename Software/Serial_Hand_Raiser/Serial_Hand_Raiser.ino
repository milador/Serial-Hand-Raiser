/** ************************************************************************
 * File Name: Serial_Hand_Raiser.ino 
 * Title: Serial Hand Raiser Software
 * Developed by: Milador
 * Version Number: 1.0 (05/9/2022)
 * Github Link: https://github.com/milador/Serial-Hand-Raiser
 ***************************************************************************/

#include <Adafruit_NeoPixel.h>

#define NUM_PIXELS        1                       // Number of LEDs
#define BLINK_DELAY       1000                    // Blink action delay in ms 

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
char inputColor[7];
char inputAction[2];
uint32_t color;
bool performAction=false;
int currentAction;                                // OFF = 0, ON = 1, BLINK = 2

//*** SETUP ***//
void setup() {
  Serial.begin(9600);

#if defined(NEOPIXEL_POWER)
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif

  pixels.begin();                                           // Initialize NeoPixel
  pixels.setBrightness(20); 
  color = 0x00ff00;
  currentAction = 0;
  delay(2000);
}

//*** LOOP ***//
void loop() {
  if (Serial.available() > 0) {
  String serialString = Serial.readString();
    // Color data
    if(serialString.length()==7){
     serialString.toCharArray(inputColor, 8);

     if(inputColor[0] == '#') {
        uint8_t red   = 16*char2num(inputColor[1]) + char2num(inputColor[2]);
        uint8_t green = 16*char2num(inputColor[3]) + char2num(inputColor[4]);
        uint8_t blue  = 16*char2num(inputColor[5]) + char2num(inputColor[6]);
      
        color = (red << 16) | (green << 8) | blue;  
        updateLed(color);
        Serial.print("LED color changed:");
        Serial.println(serialString);      
      }
    } // Action data
    else if(serialString.length()==2){  
        serialString.toCharArray(inputAction, 3);
        if(inputAction[0] == '@'){
          performAction=true;
          currentAction = inputAction[1];
        }
        else {
          performAction=false;
        }
        currentAction = inputAction[1] - '0';  // char to int
        if(performAction == true && currentAction == 2) {
          Serial.println("LED blinking");
          blinkLed(BLINK_DELAY);
        } else if(performAction == true && currentAction == 1) {
          Serial.println("LED On");
          turnLedOn();
        } else if(performAction == true && currentAction == 0) {
          Serial.println("LED Off");
          turnLedOff();
        }
        performAction=false;
    }
  }
  if(currentAction == 2){
    blinkLed(BLINK_DELAY);
  }
}

//*** CONVERT CHAR TO NUM ***//
uint8_t char2num(char c)
{ // convert a hex character to number
  if (c >= 'a') return c - 'a' + 10;
  if (c >= 'A') return c - 'A' + 10;
  return c - '0';  
}

//*** LED BLINK ***//
void blinkLed(long mstime){
  // turn on
  Serial.print("LED blinking:");
  Serial.println(color);
  pixels.fill(color);
  pixels.show();
  delay(mstime);
  pixels.fill(0x000000);
  pixels.show(); 
  delay(mstime);
}

//*** TURN LED ON ***//
void turnLedOn(){
  // turn on
  pixels.fill(color);
  pixels.show();
}

//*** TURN LED OFF ***//
void turnLedOff(){
  // turn off
  pixels.fill(0x000000);
  pixels.show();
}

//*** UPDATE LED COLOR ***//
void updateLed(uint32_t color){
  // update color
  pixels.fill(color);
  pixels.show();
}
