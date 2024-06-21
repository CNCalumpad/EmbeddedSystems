#include <TM1637Display.h>

#define CLK 22
#define DIO 23

TM1637Display display = TM1637Display(CLK, DIO);

int btns[] = { 4, 0, 2, 15 };

int leds[] = { 32, 33, 27, 14 };
int isWinner[] = { 13, 12 };
int buzzer = 26;

const uint8_t done[] = {
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           
  SEG_C | SEG_E | SEG_G,                          
};

int score = 0;
int sequence[4] = { 0, 0, 0, 0 };
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    Serial.println("initializing");
    pinMode(btns[i], INPUT_PULLUP);
  }

  for (int k = 0; k < 4; k++) {
    Serial.println("initializing");
    pinMode(leds[k], OUTPUT);
  }

  for (int l = 0; l < 2; l++) {
    Serial.println("initializing");
    pinMode(isWinner[l], OUTPUT);
  }

  pinMode(buzzer, OUTPUT);

  display.clear();
  display.setBrightness(7);
  display_digit(score);
}

void loop() {
  if (evaluateGame()) {
    if (score > 9) {
      digitalWrite(isWinner[1], HIGH);
      Serial.println("win");
    }

    if (score < 0) {
      digitalWrite(isWinner[0], HIGH);
      Serial.println("loose");
    }
  } else {
    generateSequence();
    displaySequence();
    play();
    Serial.println("is game over");
    Serial.println(evaluateGame());
  }
}

void generateSequence() {
  for (int k = 0; k < 4; k++) {
    int randomNumber = random(4);
    sequence[k] = randomNumber;
  }
}

void displaySequence() {
  for (int k = 0; k < 4; k++) {
    digitalWrite(leds[sequence[k]], HIGH);
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);
    digitalWrite(leds[sequence[k]], LOW);
    delay(500);
  }
}

int getInput() {
  if (digitalRead(btns[0]) == LOW) {
    return 0;
  } else if (digitalRead(btns[1]) == LOW) {
    return 1;
  } else if (digitalRead(btns[2]) == LOW) {
    return 2;
  } else if (digitalRead(btns[3]) == LOW) {
    return 3;
  } else {
    return 4;
  }
}

void play() {
  Serial.println("playing");
  bool correct = false;
  for (int k = 0; k < 4; k++) {
    Serial.println("to guess:");
    Serial.println(sequence[k]);
    correct = false;
    int userInput = 4;
    //wait for user input
    while (userInput >= 4) {
      userInput = getInput();
      if (userInput < 4) {
        digitalWrite(leds[userInput], HIGH);
        tone(buzzer, 1000);
        Serial.println("user input:");
        Serial.println(userInput);

        if (userInput == sequence[k]) {
          Serial.println("korik");
          correct = true;
        }
        delay(500);
        noTone(buzzer);
        digitalWrite(leds[userInput], LOW);
      }
    }
    if (correct == false) {
      score -= 1;
      Serial.println("rong");
      break;
    }
  }
  if (correct == true) {
    score += 1;
  }
  display_digit(score);

  delay(2000);
  Serial.println(score);
}

void display_digit(int N) {
  if (N > 9 || N < 0) {
    display.setSegments(done);
  } else {
    display.showNumberDec(N);
  }
}

bool evaluateGame() {
  return (score < 0 || score > 9);
}
