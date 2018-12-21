//TODO: http://www.elecrom.com/avr-tutorial-2-avr-input-output/
//TODO: Instead of STATE_LED is used, we can use the LED strip to act as a status led, eg: only the 1st led.
//TODO: SERIAL PARSER:  https://github.com/scogswell/ArduinoSerialCommand/blob/master/SerialCommand.cpp
//                      https://joshondesign.com/2014/07/31/slug


#include <FastLED.h>
#define VERSION "V0.0.0"
#define BAUD        115200
#define STATUS_LED  13  //PINB5
#define DATA_PIN    11  //PINB3
#define HW_BUTTON   2   //PIND2
#define NUM_LEDS    5   //number of used leds
#pragma message ("TIME: "__DATE__ " " __TIME__ " Version: " VERSION)

//####----variable declarations----####//
CRGB leds[NUM_LEDS];    // Define the array of leds
uint8_t i =0;
bool newcolor = false;
bool hw_button_pressed = false;

//####----function declarations----####//
void setup();
void loop();
void serialEvent();
#ifdef HW_BUTTON
    void hw_button_handler();
#endif
int freeRam();

//####----function definitions----####//
void setup(){
    pinMode(STATUS_LED,OUTPUT);             //DDRB |= PINB5;
    pinMode(DATA_PIN,OUTPUT);               //DDRB |= PINB3;
                                            //DDRB |= B00101000; //setting PIN 13 and PIN 11 as an output with one line.
#ifdef HW_BUTTON
    pinMode(HW_BUTTON,INPUT_PULLUP);        //DDRD &= ~(PIND2); PORTD |= PIND2;
#endif
    digitalWrite(STATUS_LED,HIGH);          //PORTB |= B00100000;
    Serial.begin(BAUD);
    while(!Serial){;}
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(60);
    FastLED.clear(false);
    FastLED.show();
#ifdef HW_BUTTON
    attachInterrupt(digitalPinToInterrupt(HW_BUTTON), hw_button_handler ,FALLING);
#endif
    digitalWrite(STATUS_LED,LOW);           //PORTB &= ~(PINB5);
}

void loop(){
    if(newcolor){
        FastLED.show();
        Serial.println(freeRam());
        newcolor = false;
    }
}

void serialEvent(){
    if(Serial.available()>2){
        if(i == NUM_LEDS)
            i = 0;
        Serial.readBytes( (char*)(&leds[i]), 3); // read three bytes: r, g, and b.
        i++;
        newcolor = true;
    }
}

#ifdef HW_BUTTON
    void hw_button_handler(){
        if(!hw_button_pressed)
            hw_button_pressed = true;
    }
#endif

// Private function: from http://arduino.cc/playground/Code/AvailableMemory  
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}