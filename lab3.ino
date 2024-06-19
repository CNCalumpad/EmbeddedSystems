#include <Arduino.h>

int btns[] = { 16, 17, 18, 19 };
int sevenSeg[] = { 5, 4, 0, 2, 14, 12, 13 };
int leds[] = { 21, 22, 23, 25 };

int score = 0;

int sequence[4] = { 0, 0, 0, 0 };
void setup() {
  for (int i = 0; i < sizeof(btns) / sizeof(btns[0]); i++) {
    pinMode(btns[i], INPUT_PULLUP);
  }

  for (int j = 0; j < sizeof(sevenSeg) / sizeof(sevenSeg[0]); j++) {
    pinMode(sevenSeg[j], OUTPUT);
  }

  for (int k = 0; k < sizeof(leds) / sizeof(leds[0]); k++) {
    pinMode(leds[k], OUTPUT);
  }
}

void loop() {
  generateSequence();
  displaySequence();
  waitForInput();
}

void generateSequence() {
  if (score >= 10) {
    for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++) {
      digitalWrite(leds[i], HIGH);
    }
    while (score >= 10) {
      // hehe
    }
  }

  for (int k = 0; k < sizeof(sequence) / sizeof(sequence[0]); k++) {
      auto randomNumber = random(3);
      sequence[k] = randumNumber;
    }
}

void displaySequence() {
}

void waitForInput() {
}