#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN              2
#define LED_TYPE        WS2812B
#define COLOR_ORDER         GRB
#define NUM_LEDS            373 // 2 per top, 164 per side, 40 on bottom step, plus 1 extra to account for a bug
#define BRIGHTNESS           96
#define FRAMES_PER_SECOND   180

CRGB leds[NUM_LEDS];
uint8_t offset = 0;
uint8_t autoHue = 0;        // auto-rotating "base color" used by many of the patterns

uint8_t lastStepLED = 167;
uint8_t curSidePixel = 0;   // the index of the current side pixel (adjusted for each side)

DEFINE_GRADIENT_PALETTE(sides_gp) {
  0, 65, 0, 177,
  48, 187, 9, 90,
  166, 0, 0, 255,
  255, 65, 0, 177
};

CRGBPalette16 sidePalette = sides_gp;

void setup()
{
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setTemperature(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  topBits();
  sides();
  bottomStep();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  EVERY_N_MILLISECONDS( 2 ) {
    autoHue++;  // slowly cycle the "base color" through the rainbow
  }
}

void topBits() {
  uint8_t BeatsPerMinute = 60;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);

  leds[1] = ColorFromPalette(palette, autoHue + (0 * 2), beat - autoHue + (0 * 10));
  leds[2] = ColorFromPalette(palette, autoHue + (1 * 2), beat - autoHue + (1 * 10));
  leds[NUM_LEDS - 2] = ColorFromPalette(palette, autoHue + ((NUM_LEDS - 2) * 2), beat - autoHue + ((NUM_LEDS - 2) * 10));
  leds[NUM_LEDS - 1] = ColorFromPalette(palette, autoHue + ((NUM_LEDS - 1) * 2), beat - autoHue + ((NUM_LEDS - 1) * 10));
}

void sides() {
  uint8_t sideStart = 3;
  uint8_t sideEnd = 166;
  curSidePixel += 3;

  for (int i = sideStart; i <= sideEnd; i++) {
    if (random(10) > 5)
      leds[i] = leds[i].fadeToBlackBy(50);
    if (random(10) > 5) // want them to fade separately
      leds[NUM_LEDS - i].fadeToBlackBy(50);
  }

  for (int j = 0; j < 10; j++) {
    if ((curSidePixel - j <= sideEnd) && (curSidePixel - j >= sideStart)) {
      CRGB color = ColorFromPalette(sidePalette, (curSidePixel - j) * (255 / 163));
      leds[curSidePixel - j] = color;
      leds[NUM_LEDS - (curSidePixel - j)] = color;
    }
  }

  if (curSidePixel >= sideEnd)
    curSidePixel = sideStart;
}

void bottomStep() {
  uint8_t stepStart = 167;
  uint8_t stepEnd = 206;

  for (int i = stepStart; i <= stepEnd; i++) {
    leds[i].fadeToBlackBy(20);
  }
  
  int pos = beatsin16(40, stepStart, stepEnd);
  CRGB color = leds[lastStepLED];
  if (pos == stepStart || pos == stepEnd)
    color = CHSV(autoHue, 255, 255);

  for (int i = 0; i < 5; i++) {
    if ((pos - i <= stepEnd) && (pos - i >= stepStart)) {
      leds[pos - i] = color;
    }
  }

  lastStepLED = pos;
}
