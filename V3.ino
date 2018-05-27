#include <Adafruit_NeoPixel.h>
#include <NexVariable.h>
#include <NexPage.h>
#include<Nextion.h>

const int player1Button = 2;
const int player2Button = 3;
const int player3Button = 4;
const int player4Button = 5;
const int player1Relay = 6;
const int player2Relay = 7;
const int player3Relay = 8;
const int player4Relay = 9;
const int LED = 10;
const int regulator = 11;

int Voltages [6] = {0, 0, 15, 30, 45, 254};
int32_t playerNumber = 0;
int32_t power = 0;
int32_t rounds = 0;
int allPressed = 0;
int whoPressed[5];
int winner = 0;
int loser = 0;
int play = 0;
int PP1 = 0;
int PP2 = 0;
int PP3 = 0;
int PP4 = 0;


bool player1ButtonState = false;
bool player2ButtonState = false;
bool player3ButtonState = false;
bool player4ButtonState = false;
bool player1Done = false;
bool player2Done = false;
bool player3Done = false;
bool player4Done = false;
bool pressed = false;

NexVariable NplayerNumber = NexVariable(1, 5, "playerCount1");
NexVariable Nrounds = NexVariable(1, 7, "pattern1");
NexVariable Npower = NexVariable(1, 6, "volt1");
NexButton START = NexButton(1, 2, "bt0");
NexButton FINISH = NexButton(4, 5, "b0");
NexNumber P1 = NexNumber(2, 14, "n1");
NexNumber P2 = NexNumber(2, 15, "n2");
NexNumber P3 = NexNumber(2, 16, "n3");
NexNumber P4 = NexNumber(2, 17, "n4");
NexNumber Rounds = NexNumber(2, 21, "n5");

NexTouch *nex_listen_list[] =
{
  &START,
  &FINISH,
  NULL  // String terminated
};

void STARTPushCallback(void *ptr) {
  play = 1;
}

void FINISHPushCallback(void *ptr) {
  delay(500);
  SettingsChange();
}

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, LED, NEO_GRBW + NEO_KHZ800);

void setup() {
  pinMode(regulator, OUTPUT);
  analogWrite(regulator, 0);

  pinMode(player1Button, INPUT);
  pinMode(player2Button, INPUT);
  pinMode(player3Button, INPUT);
  pinMode(player4Button, INPUT);

  pinMode(player1Relay, OUTPUT);
  digitalWrite(player1Relay, HIGH);
  pinMode(player2Relay, OUTPUT);
  digitalWrite(player2Relay, HIGH);
  pinMode(player3Relay, OUTPUT);
  digitalWrite(player3Relay, HIGH);
  pinMode(player4Relay, OUTPUT);
  digitalWrite(player4Relay, HIGH);

  randomSeed(analogRead(0));
  Serial.begin(9600);

  strip.setBrightness(100);
  strip.begin();
  strip.show();

  START.attachPush(STARTPushCallback);
  FINISH.attachPush(FINISHPushCallback);
}

void loop() {
  nexLoop(nex_listen_list);
  if (play == 1) { // Pradedamas zaidimas
    delay(5000);
    for (int i = 1; i <= rounds; i++) { // Vygdoma 5 raundus
      Rounds.setValue(i);
      Rounds.setValue(i);
      Regulating();
      Random();
      Shock(loser);
      Results();
      loser = 0;
      winner = 0;
      delay(3000);
    }
    play=0;
    PP1 = 0;
    PP2 = 0;
    PP3 = 0;
    PP4 = 0;
  }
}

void Results() {
  if (winner == 1) {
    PP1++;
    P1.setValue(PP1);
    P1.setValue(PP1);
  } else if (winner == 2) {
    PP2++;
    P2.setValue(PP2);
    P2.setValue(PP2);
  }
  else if (winner == 3) {
    PP3++;
    P3.setValue(PP3);
    P3.setValue(PP3);
  }
  else if (winner == 4) {
    PP4++;
    P4.setValue(PP4);
    P4.setValue(PP4);
  }
}

void SettingsChange() { // Pakeicia nustatymus
  NplayerNumber.getValue(&playerNumber);
  Nrounds.getValue(&rounds);
  Npower.getValue(&power);
  NplayerNumber.getValue(&playerNumber);
  delay(100);
}

void PlayerNull() { // Nunulina zaideju paspaudimo mygtukus
  player1ButtonState = false;
  player2ButtonState = false;
  player3ButtonState = false;
  player4ButtonState = false;
  player1Done = false;
  player2Done = false;
  player3Done = false;
  player4Done = false;
}

void Shock(int who) { // Issiuncia elektros soka
  switch (who) {
    case 1:
      digitalWrite(player1Relay, LOW);
      delay(500);
      digitalWrite(player1Relay, HIGH);
      break;
    case 2:
      digitalWrite(player2Relay, LOW);
      delay(500);
      digitalWrite(player2Relay, HIGH);
      break;
    case 3:
      digitalWrite(player3Relay, LOW);
      delay(500);
      digitalWrite(player3Relay, HIGH);
      break;
    case 4:
      digitalWrite(player4Relay, LOW);
      delay(500);
      digitalWrite(player4Relay, HIGH);
      break;
    default:
      break;
  }
}

void ButtonPressing() { // Mygtuku paspaudimo skaiciavimas
  for (int k = 1; k <= playerNumber;) {
    player1ButtonState = digitalRead(player1Button);
    player2ButtonState = digitalRead(player2Button);
    player3ButtonState = digitalRead(player3Button);
    player4ButtonState = digitalRead(player4Button);
    if (player1ButtonState && !player1Done) {
      whoPressed [k] = 1;
      player1Done = true;
      k++;
    } else if (player2ButtonState && !player2Done) {
      whoPressed [k] = 2;
      player2Done = true;
      k++;
    } else if (player3ButtonState && !player3Done) {
      whoPressed [k] = 3;
      player3Done = true;
      k++;
    } else if (player4ButtonState && !player4Done) {
      whoPressed [k] = 4;
      player4Done = true;
      k++;
    }
  }
  winner = whoPressed[1];
  loser = whoPressed[playerNumber];
  PlayerNull();
}

void Random() { // Atsitiktinis lemputes mirgejimas
  colorWipe(strip.Color(0, 255, 0), 100); // Green
  colorWipe(strip.Color(0, 0, 0), 0); // Off
  int randomNumber = random(0, 1);
  int times = (int)(randomNumber);
  for (int i = 1; i <= times; i++) {
    delay((int)(random(100, 5000)));
    colorWipe(strip.Color(0, 255, 0), 100); // Green
    colorWipe(strip.Color(0, 0, 0), 0); // Off
  }
  delay((int)(random(100, 5000)));
  Fault();
  colorWipe(strip.Color(255, 0, 0), 0); // Red
  ButtonPressing();
  colorWipe(strip.Color(0, 0, 0), 0); // Off
}

void colorWipe(uint32_t c, uint8_t wait) {
  strip.setPixelColor(0, c);
  strip.show();
  delay(wait);
}

void Regulating() {
  analogWrite(regulator, 0);
  delay(1000);
  analogWrite(regulator, Voltages[power]);
}

void Fault() {
  int faults[4];
  player1ButtonState = digitalRead(player1Button);
  player2ButtonState = digitalRead(player2Button);
  player3ButtonState = digitalRead(player3Button);
  player4ButtonState = digitalRead(player4Button);
  if (player1ButtonState) {
    Shock(1);
  }
  if (player2ButtonState) {
    Shock(2);
  }
  if (player3ButtonState) {
    Shock(3);
  }
  if (player4ButtonState) {
    Shock(4);
  }
  PlayerNull();
}


