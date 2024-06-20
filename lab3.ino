int btns[] = { 4, 0, 2, 15 };
// int btns[] = { 34, 35, 36, 39 };

// int sevenSeg[] = { 7, 5, 13, 12, 14, 2, 0 };
int leds[] = { 32, 33, 27, 14 };
int isWinner[] = { 13, 12 };
int buzzer = 26;

// byte sevenSegDigits[10][7] = { {
//                                  0,
//                                  0,
//                                  0,
//                                  0,
//                                  0,
//                                  0,
//                                  1,
//                                },                          //0
//                                { 1, 0, 0, 1, 1, 1, 1 },    //1
//                                { 0, 0, 1, 0, 0, 1, 0 },    //2
//                                { 0, 0, 0, 0, 1, 1, 0 },    //3
//                                { 1, 0, 0, 1, 1, 0, 0 },    //4
//                                { 0, 1, 0, 0, 1, 0, 0 },    //5
//                                { 0, 1, 0, 0, 0, 0, 0 },    //6
//                                { 0, 0, 0, 1, 1, 1, 1 },    //7
//                                { 0, 0, 0, 0, 0, 0, 0 },    //8
//                                { 0, 0, 0, 1, 1, 0, 0 } };  //9
//	{{1, 1, 1, 1, 1, 1, 0},0
//	{0, 1, 1, 0, 0, 0, 0},1
//	{1, 1, 0, 1, 1, 0, 1},2
//   {1, 1, 1, 1, 0, 0, 1},3
//   {0, 1, 1, 0, 0, 1, 1},4
//   {1, 0, 1, 1, 0, 1, 1},5
//   {1, 0, 1, 1, 1, 1, 1},6
//  {1, 1, 1, 0, 0, 0, 0},7
//   {1, 1, 1, 1, 1, 1, 1},8
//   {1, 1, 1, 0, 0, 1, 1}};9

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
  // display_digit(score);
}

void generateSequence() {
  // Generate a random sequence of 4 numbers
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

  //loop four times because four digits in the sequence needs four input
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
  delay(2000);
  Serial.println(score);
}

// void display_digit(int N){
//   if (N > 9 || N < 0){
//     for(int i = 0; i < 7; i++){
//     digitalWrite(sevenSeg[i], 0); //off all when score is not sigle digit
//   	}
//   }else{
//     for(int i = 0; i < 7; i++){
//     digitalWrite(sevenSeg[i], sevenSegDigits[N % 10][i]);
//   	}
//   }
// }

bool evaluateGame() {
  return (score < 0 || score > 9);
}
