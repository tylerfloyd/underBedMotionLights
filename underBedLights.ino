#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//LED strip pin
const int LED_PIN = 6;
//LED strip's number of lights
const int NUMBER_OF_PIXELS = 150;
//PIR sensor pin
const int NUMBER_OF_PIRS = 1;
const int MOTION_PINS[NUMBER_OF_PIRS] = {2};
const int TURN_ON_DELAY = 5;
//for testing only
const boolean DEBUG_ON  = true;

//Configure LED strip library
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMBER_OF_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

boolean LIGHTS_ON = false;
int TIMER = 0;
int MAX_TIMER = 80;
int BUFFER = 0;
int MAX_BUFFER = 10;
int MOTION_DETECTED = false;

void setup() {
  pixels.begin();
  pixels.setBrightness(0);
  pixels.show();
  
  for(int i=0; i < sizeof(MOTION_PINS); i++){
    pinMode(MOTION_PINS[i],INPUT);    
  }
  if(DEBUG_ON){
    Serial.begin(9600);
  }
}

void loop() {
  delay(500);
  MOTION_DETECTED = false;
  
  for(int i=0; i < sizeof(MOTION_PINS); i++){
    if(digitalRead(MOTION_PINS[i]) == 1){
      MOTION_DETECTED = true;
    }
  }
  
  if(DEBUG_ON){
    turnOnDebug();
  }
  
  if(MOTION_DETECTED && !LIGHTS_ON && BUFFER == 0){
    toggleLightStatus();
    turnLightsOn();
  }else if(TIMER >= MAX_TIMER){
    turnLightsOff();
  }else if(LIGHTS_ON && BUFFER == 0){
    TIMER++;
  }else if(BUFFER > 0){
    BUFFER--;
  }
}

void turnLightsOn(){
  pixels.setBrightness(64);
  for(int i=0; i < NUMBER_OF_PIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0,150,0));
    pixels.show();
    delay(TURN_ON_DELAY);
  }
}

void turnLightsOff(){
  for(int i=NUMBER_OF_PIXELS; i >= 0; i--){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
    delay(TURN_ON_DELAY);
  }
  clocksReset();
  toggleLightStatus();
}

void clocksReset(){
  BUFFER = MAX_BUFFER;
  TIMER = 0;
}

void toggleLightStatus(){
  LIGHTS_ON = !LIGHTS_ON;
}

void turnOnDebug(){  
  Serial.print("Motion:");
  Serial.print(MOTION_DETECTED);
  Serial.print(" | ");
  Serial.print("LIGHTS_ON:");
  Serial.print(LIGHTS_ON);
  Serial.print(" | ");
  Serial.print("BUFFER:");
  Serial.print(BUFFER);
  Serial.print(" | ");
  Serial.print("TIMER:");
  Serial.println(TIMER);
}
