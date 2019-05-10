#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        6
#define NUMPIXELS 27

#define DELAYVAL 10

int PIR = 3;

void setup() {
  Serial.begin(9600);
  pinMode(PIR, INPUT);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin();
}

void loop() {
  pixels.clear();

  if (digitalRead(PIR) == HIGH) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 150, 250));
      pixels.show();
      delay(DELAYVAL);
    }
    Serial.println("SENSOR ON");
  } else {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAYVAL);
    }
    Serial.println("SENSOR OFF");
  }
  delay(10);
}
