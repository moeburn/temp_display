#include "SH1106Wire.h"

SH1106Wire display(0x3c, SDA, SCL, GEOMETRY_128_64, I2C_ONE, 400000); 

#include <OneWire.h>  //All Dallas code has been temporarily removed, to be replaced by Dallas EEPROM calibrator
#include <DallasTemperature.h>
#include <NonBlockingDallas.h>
#include <FastLED.h>

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

float tempC;
	uint8_t red = 50;
	uint8_t green = 100;
	uint8_t blue = 150;
	double hue, saturation, lighting, value;

void handleTemperatureChange(int deviceIndex, int32_t temperatureRAW)
{

  tempC = sensorDs18b20.rawToCelsius(temperatureRAW);

}


void setup() {
  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  sensorDs18b20.begin(NonBlockingDallas::resolution_12, TIME_INTERVAL); //use non-blocking DS18b20 library
  sensorDs18b20.onTemperatureChange(handleTemperatureChange);  //only notice when DS18B20 changes
  display.init();
  display.setFont(SansSerif_plain_30);
  //display.flipScreenVertically();
  display.setBrightness(200); 
    leds[0] = CRGB(100, 100, 0);
    FastLED.show();
}

void loop() {
  sensorDs18b20.update();
  display.clear();
  String tempstring = String(tempC) + "°C"; 
  display.drawString(0, 0, tempstring);
  display.display();

  if (tempC < 40) {hue = map(tempC, 10,40,243,360);}
  if (tempC >= 40) {hue = map(tempC, 40,60,0,60);}
  hue = map(hue, 0, 360, 0, 255);
  
  leds[0] = CHSV(hue, 255, 255);
  FastLED.show();
  
}
