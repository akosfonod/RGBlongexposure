#include <FastLED.h>
#define VERSION V0.0.0
#define BAUD        115200
#define STATUS_LED  13 //B00100000  MSB----LSB
#define DATA_PIN    11 //B00001000
#define NUM_LEDS    10 //number of used leds
#pragma message (__DATE__##__TIME__##VERSION)

//####----variable declarations----####//
CRGB leds[NUM_LEDS];    // Define the array of leds
uint8_t i =0;
bool newcolor = false;

//####----function declarations----####//
void setup();
void loop();
void serialEvent();

//####----function definitions----####//
void setup(){
    pinMode(STATUS_LED,OUTPUT);
    pinMode(DATA_PIN,OUTPUT);
    //DDRB = DDRB | B00101000 //setting PIN 13 and PIN 11 as an output with one line.
    digitalWrite(STATUS_LED,HIGH);
    Serial.begin(BAUD);
    While(!Serial){;}
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
    digitalWrite(STATUS_LED,LOW);
}

void loop()
    if(newcolor){
        FastLED.show();
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