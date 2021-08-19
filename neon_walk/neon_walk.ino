#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN              5
#define LED_TYPE        WS2812B
#define COLOR_ORDER         GRB
#define NUM_POSTS            12 //the number of  posts in the neon walk
#define NUM_LEDS (NUM_POSTS * 4) //4 LEDs per grouping
#define BRIGHTNESS           96
#define FRAMES_PER_SECOND    8

CRGB leds[NUM_LEDS];
uint8_t offset = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void setup()
{
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  //rotator();

  walkAlong(5);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

  EVERY_N_MILLISECONDS( 2 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}

void walkAlong(int timeDelay)
{
  fadeToBlackBy(leds, NUM_LEDS, 100);

  int pos = offset * 4;

  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  leds[pos+1] += CHSV( gHue + random8(64), 200, 255);
  leds[pos+2] +=  CHSV( gHue + random8(64), 200, 255);
  leds[pos+3] +=  CHSV( gHue + random8(64), 200, 255);

  EVERY_N_MILLISECONDS(timeDelay) { offset = (offset + 1) % 12; }
}

void rotator()
{
  fadeToBlackBy(leds, NUM_LEDS, 1000);
  
  for (int i = 0; i < NUM_POSTS; i++)
  {
  
    leds[i*4+offset].red = random8(255);
    leds[i*4+offset].green = random8(255);
    leds[i*4+offset].blue = random8(255);
  }

  offset = (offset + 1) % 4;
}
