//fai fflush
#include <avr/sleep.h>
#include <EnableInterrupt.h>

#define NUM_LEDS 4
#define Ls 11
#define POT A0

#define L1 8
#define L2 9
#define L3 7
#define L4 6

#define T1 2
#define T2 3
#define T3 13
#define T4 12

int penalties = 0;
int score = 0;
int F = 1;
int difficulty = 0;
int time1 = 2000;
int time2 = 3000;
int time3 = 5000;

int ledPins[NUM_LEDS] = {L1,L2,L3,L4};
int buttonPins[NUM_LEDS] = {T1,T2,T3,T4};
int pressed[NUM_LEDS] = {0,0,0,0};
int pattern[NUM_LEDS] = {0,0,0,0};
int time0 = 0;
bool started = false;

void setup()
{
  pinMode(Ls, OUTPUT);
  pinMode(POT, INPUT);
  
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  
  pinMode(T1, INPUT);
  pinMode(T2, INPUT);
  pinMode(T3, INPUT);
  pinMode(T4, INPUT);
  enableInterrupt(T1, T1Pressed, RISING);
  enableInterrupt(T2, T2Pressed, RISING);
  enableInterrupt(T3, T3Pressed, RISING);
  enableInterrupt(T4, T4Pressed, RISING);
  
  Serial.begin(9600);
}

void loop()
{
  if(!started){
    pulseAndWait();
  }else{
    setDifficulty();
    startGame();
    delay(10000);
    started = false;
  }
}

void T1Pressed(){
  pressed[0] = 1;
  if(started == false){
    time0 = millis();
  }else{
    digitalWrite(L1, HIGH);
    delay(1000);
    digitalWrite(L1, LOW);
  }
  Serial.print("Premuto T");
  Serial.println("1");
}

void T2Pressed(){
  pressed[1] = 1;
  if(started == false){
    time0 = millis();
  }else{
    digitalWrite(L2, HIGH);
    delay(1000);
    digitalWrite(L2, LOW);
  }
  Serial.print("Premuto T");
  Serial.println("2");
}

void T3Pressed(){
  pressed[2] = 1;
  if(started == false){
    time0 = millis();
  }else{
    digitalWrite(L3, HIGH);
    delay(1000);
    digitalWrite(L3, LOW);
  }
  Serial.print("Premuto T");
  Serial.println("3");
}

void T4Pressed(){
  pressed[3] = 1;
  if(started == false){
    time0 = millis();
  }else{
    digitalWrite(L4, HIGH);
    delay(1000);
    digitalWrite(L4, LOW);
  }
  Serial.print("Premuto T");
  Serial.println("4");
}

void pulseAndWait(){
  Serial.println("Welcome to the Catch the Led Pattern Game. Press Key T1 to Start");
  int intensity = 0;
  int fade = 5;
  
  while(started == false){
    time0 = millis();
    clearInputs();
    while((millis() - time0) < 10000 && started == false){
      if(pressed[0] == 1){
        started = true;
        analogWrite(Ls, 0);
      }
      analogWrite(Ls, intensity);
      intensity = intensity + fade;
      if (intensity == 0 || intensity == 255) {
        fade = -fade ;
      }
      delay(20);
    }
    analogWrite(Ls, 0);
    if(started == false){
      Serial.println("AAAAAAAAAA");
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_enable();
      sleep_mode();
      sleep_disable();
    }
  }
}

void setDifficulty(){
  int newValue = analogRead(POT);
  if(newValue < 255){
    F = 1;
  } else if(newValue < 512){
    F = 2;
  } else if(newValue < 768){
    F = 3;
  } else{
    F = 4;
  }
  Serial.print("Diff: ");
  Serial.println(F);
  }

void startGame(){
  Serial.println("Go!");
  score = 0;
  penalties = 0;
  bool error = false;
  while(penalties < 3){
    delay(time1-difficulty);
    generatePattern();
    //Turns on leds and waits
    for(int i = 0; i < NUM_LEDS; i++){
      if(pattern[i] == 1){
      digitalWrite(ledPins[i], HIGH);
      }
    }
    //checks that no buttons were pressed while leds were still ON
    if(checkWrongInputs() == true){
      penalty();
    }
    //Turns off leds
    for(int i = 0; i < NUM_LEDS; i++){
      digitalWrite(ledPins[i], LOW);
    }
    //getPattern();
    delay(time3-difficulty);
    //Checks the input
    for(int i = 0; i < NUM_LEDS; i++){
      if(pattern[i] != pressed[i]){
        error = true;
      }
    }
    clearInputs();
    if(error){
      penalty();
      error = false;
    } else{
      score++;
    }
    increaseDifficulty();
  }
  Serial.print("Game Over. Final Score: ");
  Serial.println(score);
}

void generatePattern(){
  for(int i = 0; i < NUM_LEDS; i++){
        pattern[i] = random(0, 2);
        Serial.print("p: ");
        Serial.println(pattern[i]);
    }
  }

  void penalty(){
  penalties ++;
  Serial.println("Penalty!");
  Serial.println(penalties);
  digitalWrite(Ls, HIGH);
  delay(1000);
  digitalWrite(Ls, LOW);
}

bool checkWrongInputs(){
  time0=millis();
  while((millis()-time0) < (time2-difficulty)){
    for(int i = 0; i < NUM_LEDS; i++){
      if(pressed[i] == 1){
        return true;
      }
    }
  }
  return false;
}

void increaseDifficulty(){
  difficulty -= (F*500);
}

void clearInputs(){
  for(int i = 0; i < NUM_LEDS; i++){
    pressed[i] = 0;
    }
  }
