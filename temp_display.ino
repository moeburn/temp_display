#include "SH1106Wire.h"

SH1106Wire display(0x3c, SDA, SCL, GEOMETRY_128_64, I2C_ONE, 400000); 


#include <FastLED.h>
#include <SteinhartHart.h>


//SteinhartHart thermistor(2930,7248,14800, 328.59, 305.71, 289.65);
SteinhartHart thermistor(905,1900,3133, 338.65, 310.75, 288.55);
int16_t adc0;

#define LED_PIN 1  
#define ADC_PIN 0
#define TIME_INTERVAL 1500    

#define NUM_LEDS 1
CRGB leds[NUM_LEDS];



#define every(interval) \        
  static uint32_t __every__##interval = millis(); \
  if (millis() - __every__##interval >= interval && (__every__##interval = millis()))

float tempC, tempTherm;
	uint8_t red = 0;
	uint8_t green = 000;
	uint8_t blue = 0;
	double hue, saturation, lighting, value;



long mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void setup() {
  Serial.begin(9600);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  display.init();
  display.setFont(SansSerif_plain_30);
  display.flipScreenVertically();
  //display.setFont(ArialMT_Plain_16);
  display.setBrightness(128); 

}

void loop() {
  delay(10);

  display.clear();

  adc0 = analogRead(ADC_PIN);

  tempTherm = thermistor.resistanceToTemperature(adc0) - 273.15;
  //ADC0: 2800, V: 2.26, R2: 21621.63, Temp: -94.89
  blue = mapf(tempTherm, 15, 40, 255, 0);
  red = mapf(tempTherm, 15, 40, 0, 255);
  green = mapf(tempTherm, 40, 60, 0, 255);
  if (tempTherm < 15) {green = 0; red = 0; blue = 255;}
  if (tempTherm < 40) {green = 0;}
  if (tempTherm > 40) {red = 255; blue = 0;}
  if (tempTherm > 60) {green = 255;}
  every(100){
  String thermstring = String(tempTherm, 1)+ "°C"; 
  //String adcstring = String(adc0); 
  //display.setFont(SansSerif_plain_30);
  display.drawString(0, 16, thermstring);
  //display.setFont(ArialMT_Plain_16);
  //display.drawString(0, 48, adcstring);
  display.display();
  }
  leds[0] = CRGB(red, green, blue);
  FastLED.show();
}
