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

int hasPressed() {
  for (int i = 0; i < sizeof(btns) / sizeof(btns[0]); i++) {
    if (digitalRead(btns[i]) == HIGH){
      return i;
    }
  }
  return sizeof(btns) / sizeof(btns[0]);
}

void generateSequence() {
  //ggs na d2 lods
  if (score >= 10) {
    for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++) {
      digitalWrite(leds[i], HIGH);
    }
    while (score >= 10) {
      // hehe forever loop kasi ggs na
    }
  }
  // generate number here
  for (int k = 0; k < sizeof(sequence) / sizeof(sequence[0]); k++) {
    auto randomNumber = random(3);
    sequence[k] = randomNumber;
  }
}

void displaySequence() {
  for (int k = 0; k < sizeof(sequence) / sizeof(sequence[0]); k++) {
    digitalWrite(leds[sequence[k]], HIGH);
    delay(750);
    digitalWrite(leds[sequence[k]], LOW);
    delay(750);
  }
}

void waitForInput() {
  while(!hasPressed()){
    //loop lang habang hindi pa pumipindot
  }
}

