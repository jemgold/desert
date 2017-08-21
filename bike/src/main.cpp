#include "Arduino.h"
#include <FastLED.h>
// #include <OneButton.h>
// #include <TimerOne.h>
#include <ClickEncoder.h>

#define PIN_ENCODER_A 5
#define PIN_SWITCH 4
#define PIN_ENCODER_B 3

#define PIN_FRONT_L 14
#define PIN_FRONT_R 15
#define PIN_MID_L 16
#define PIN_MID_R 17
#define PIN_REAR 18

#define COLOR_ORDER GRB
#define CHIPSET WS2812B

// 60 (x2) + 60 (x2) + 60
#define NUM_LEDS 180

#define MAX_MA 1500
uint8_t BRIGHTNESS = 125;
#define SATURATION 255
#define STEPS 2
#define FRAMES_PER_SECOND 60

ClickEncoder *encoder;
int16_t last, value;
// OneButton button(PIN_SWITCH, true);

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette( PartyColors_p );
TBlendType currentBlending;

// Animations
void rainbowBeat() {
  uint8_t beatA = beatsin8(17, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(13, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);            // Use FastLED's fill_rainbow routine.
}

void bounceColor() {
  lead_dot = beatsin8(20, 0, NUM_LEDS);
  leds[lead_dot] = CRGB::HotPink;
  EVERY_N_MILLISECONDS(1000 / 30) {
    fadeToBlackBy(leds, NUM_LEDS, 64);
    FastLED.show();
  }
}


void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  for (int i=0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    colorIndex += STEPS;
  }
  FastLED.show();
}

void resetLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

// ClickHandlers
void singleClick() {
  // nextPattern();
}

void doubleClick() {
  // nextPattern();
}

void longPress() {
  // nextPattern();
}

// RotaryHandlers
void rotaryClockwise() {
  FastLED.setBrightness(BRIGHTNESS += 10);
}

void rotaryAnticlockwise() {
  FastLED.setBrightness(BRIGHTNESS -= 10);
}

// Main
void timerIsr() {
  encoder->service();
}

void setup() {
  // button.attachClick(singleClick);
  // button.attachDoubleClick(doubleClick);
  // button.attachLongPressStop(longPress);
  // encoder = new ClickEncoder(PIN_ENCODER_A, PIN_ENCODER_B, PIN_SWITCH);
  
  Serial.begin(9600);

  // add front LEDs
  FastLED.addLeds<CHIPSET, PIN_FRONT_L, COLOR_ORDER>(leds, 60).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<CHIPSET, PIN_FRONT_R, COLOR_ORDER>(leds, 60).setCorrection(TypicalLEDStrip);

  // add main LEDs
  FastLED.addLeds<CHIPSET, PIN_MID_L, COLOR_ORDER>(leds, 60, 60).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<CHIPSET, PIN_MID_R, COLOR_ORDER>(leds, 60, 60).setCorrection(TypicalLEDStrip);

  // add sissy-bar LEDs
  FastLED.addLeds<CHIPSET, PIN_REAR, COLOR_ORDER>(leds, 120, 60).setCorrection(TypicalLEDStrip);;

  FastLED.setBrightness(BRIGHTNESS);
  currentBlending = LINEARBLEND;
  currentPalette = RainbowStripesColors_p;

  set_max_power_in_volts_and_milliamps(5, MAX_MA);

  resetLEDs();

  // Timer1.initialize(1000);
  // Timer1.attachInterrupt(timerIsr);
}

void loop() {
  rainbowBeat();
  bounceColor();
  FastLED.show();
}

// void loop() {
//   int i = 0;
//   EVERY_N_MILLISECONDS(60) {
//     FillLEDsFromPaletteColors(i);
//     i++;
//   }
  
// }

int lead_dot = 0;

int angle = 0;

// void loop() {
//   EVERY_N_MILLISECONDS(1000 / FRAMES_PER_SECOND) {
//     // uint8_t lead_dot = map(cubicwave8(angle), 0, 255, 0, NUM_LEDS - 1);
//     angle = angle + 1;
//     leds[lead_dot] = ColorFromPalette(currentPalette, lead_dot, BRIGHTNESS, currentBlending);

//     // fadeToBlackBy(leds, NUM_LEDS, 10);
//   }

//   FastLED.show();
// }
