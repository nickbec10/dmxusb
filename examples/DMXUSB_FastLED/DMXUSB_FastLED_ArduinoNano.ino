/* DMXUSB_FastLED.ino
 * Originally created 11/25/2017 by Perry Naseck (DaAwesomeP)
 * This is an example sketch for the DMXUSB Arduino/Teensy library to drive adressable LEDs with the FastLED library.
 * https://github.com/DaAwesomeP/dmxusb/
 *
 * Copyright 2017-present Perry Naseck (DaAwesomeP)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// The example uses code from the FastLED Blink example here:
// https://github.com/FastLED/FastLED/blob/master/examples/Blink/Blink.ino
// More information about FastLED here: https://github.com/FastLED/FastLED
// #define FASTLED_ALLOW_byteERRUPTS 0
#include "DMXUSB.h"
#include <FastLED.h>

// DMXUSB should receive and transmit data at the highest, most reliable speed possible
// Recommended Arduino baud rate: 115200
// Recommended Teensy 3 baud rate: 2000000 (2 Mb/s)
// DMX baud rate: 250000
// MIDI baud rate: 31250
#define DMXUSB_BAUDRATE 115200

// Number of LEDs on the strip
const byte NUM_LEDS = 16;
const byte NUM_BLOCKS = 16;
//const byte LED_TOTAL = 288;

// For LED chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For LED chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3
// #define CLOCK_PIN 13
// #define FRAMES_PER_SECOND  120

// Define the array of LEDs
// CRGB leds[NUM_LEDS * 3];
const int LED_TOTAL = NUM_BLOCKS * NUM_LEDS;
CRGB leds[LED_TOTAL];

// receive a DMX transmission
void showLEDs(byte universe, char buffer[512]) {
 
  for (byte index=0; index < 48; index++) { // for each channel, universe starts at 0
    byte value = buffer[index]; // DMX value 0 to 255
    byte LEDchannel = (universe) + index; // Find the LED number; can't fit a 3-channel fixture on the remaining two channels
    if (LEDchannel <= (NUM_LEDS * 3) - 1) { // If DMX channel (LEDchannel starts at 0) is in range of LEDs (3 channels per LED for RGB)
      byte colorChannel = LEDchannel % 3; // Find the color channel of the LED addressed
      byte ledID = (LEDchannel - colorChannel) / 3; // Find the FastLED index of the LED addressed
      byte ledIDBase = NUM_BLOCKS * ledID;
      byte ledIDUse = ledIDBase - 0;
      for (byte block=0; block < NUM_BLOCKS; block++) {
        if (colorChannel == 1) leds[ledIDUse + block].r = value; // If the channel is red, write the red value of the LED
        if (colorChannel == 0) leds[ledIDUse + block].g = value; // If the channel is green, write the green value of the LED
        if (colorChannel == 2) leds[ledIDUse + block].b = value; // If the channel is blue, write the blue value of the LED
      }      
    }

/*    for (byte count=1; count < NUM_LEDS; count++){
      byte ledMulti = count * NUM_BLOCKS - NUM_BLOCKS+1;
      byte ledToCopy = count * NUM_BLOCKS;
      for (byte i=0; i<NUM_BLOCKS-1; i++){
        leds[ledMulti] =  leds[ledToCopy];
        ledMulti++;
      }
    }
*/
  }

  FastLED.show(); // Display the frame after processing all channels
  // FastLED.delay(1000/FRAMES_PER_SECOND); 
}

DMXUSB DMXPC(
  // Stream serial,
  Serial,
  // byte baudrate,
  DMXUSB_BAUDRATE,
  // byte mode,
  // With mode==1, the library processes two universes for a total of 1024 DMX channels
  0,
  // void (*dmxInCallback)(byte universe, unsigned byte index, char buffer[512])
  showLEDs
);

void setup() {
  Serial.begin(DMXUSB_BAUDRATE);

  // Uncomment/edit one of the following lines for your leds arrangement.
  // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, LED_TOTAL);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDSSTRING);
  // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
  DMXPC.listen();
}
