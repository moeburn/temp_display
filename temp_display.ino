#include "SH1106Wire.h"

SH1106Wire display(0x3c, SDA, SCL, GEOMETRY_128_64, I2C_ONE, 400000); 

#include <OneWire.h>  //All Dallas code has been temporarily removed, to be replaced by Dallas EEPROM calibrator
#include <DallasTemperature.h>
#include <NonBlockingDallas.h>
#include <FastLED.h>
#include <SteinhartHart.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;


SteinhartHart thermistor(2930,7248,14800, 328.59, 305.71, 289.65);
int16_t adc0;

#define LED_PIN 1  
#define ONE_WIRE_BUS 2
#define TIME_INTERVAL 1500    

#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallasTemp(&oneWire);
NonBlockingDallas sensorDs18b20(&dallasTemp);

#define every(interval) \        
  static uint32_t __every__##interval = millis(); \
  if (millis() - __every__##interval >= interval && (__every__##interval = millis()))

float tempC, tempTherm;
	uint8_t red = 0;
	uint8_t green = 000;
	uint8_t blue = 0;
	double hue, saturation, lighting, value;

void handleTemperatureChange(int deviceIndex, int32_t temperatureRAW)
{

  tempC = sensorDs18b20.rawToCelsius(temperatureRAW);

}

long mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void setup() {
  //Serial.begin(9600);
  ads.setGain(GAIN_ONE); 
  ads.begin();
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  sensorDs18b20.begin(NonBlockingDallas::resolution_12, TIME_INTERVAL); //use non-blocking DS18b20 library
  sensorDs18b20.onTemperatureChange(handleTemperatureChange);  //only notice when DS18B20 changes
  display.init();
  display.setFont(SansSerif_plain_30);
 // display.setFont(ArialMT_Plain_16);

  //display.flipScreenVertically();
  display.setBrightness(255); 

}

void loop() {
  delay(10);
  //sensorDs18b20.update();
  display.clear();
  //String tempstring = String(tempC) + "°C"; 

  /*long adsread;
  for (int i = 0; i < 50; i++) {
      adsread += ads.readADC_SingleEnded(0);
  }*/
  adc0 = ads.readADC_SingleEnded(0);
  //String thermstring = String(tempTherm) + "°C"; 
  float V = ads.computeVolts(adc0);
  float  R2 = (-V * 10000) / (V - 3.3);    
  tempTherm = thermistor.resistanceToTemperature(R2) - 273.15;
  blue = mapf(tempTherm, 15, 40, 255, 0);
  red = mapf(tempTherm, 15, 40, 0, 255);
  green = mapf(tempTherm, 40, 60, 0, 255);
  if (tempTherm < 40) {green = 0;}
  if (tempTherm > 40) {red = 255; blue = 0;}
  if (tempTherm > 60) {green = 255;}
  String thermstring = String(tempTherm, 2)+ "°C"; 
  //String rgbstring = String(red) + "," + String(green) + "," + String(blue);
  //display.drawString(0, 0, tempstring);
    //display.setFont(SansSerif_plain_30);

  display.drawString(0, 16, thermstring);
   // display.setFont(ArialMT_Plain_16);
  //display.drawString(0, 32, rgbstring);
  display.display();
  //Serial.println(tempTherm);


  /*if (tempTherm < 40) {hue = map(tempTherm, 10,40,243,360);}
  if (tempTherm >= 40) {hue = map(tempTherm, 40,60,0,60);}
  hue = map(hue, 0, 360, 0, 255);*/

  leds[0] = CRGB(red, green, blue);
  FastLED.show();

  
}
