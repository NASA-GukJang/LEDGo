#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <MsTimer2.h>
#include <Wire.h>
#include "RTClib.h"
#include "game.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define PIN            3

#define PIXELS_ROWS 32
#define PIXELS_COLUMNS 48
#define PIXELS_COUNT 768*2 //PIXELS_ROWS * PIXELS_COLUMNS

#define ROW 7
#define COL 6

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXELS_COUNT, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(96, 8, PIN,
  NEO_MATRIX_TOP    + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_SEQUENCE,
  NEO_GRB            + NEO_KHZ800);

const uint16_t Mcolors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)};

int WinCheckField[ROW][COL];

extern volatile unsigned long timer0_millis; //타이머변수
unsigned long timeVal = 0;
unsigned long previousVal = 0;

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);

  mySoftwareSerial.begin(9600);
  
  pixels.begin();
  pixels.setBrightness(10);

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin5, INPUT_PULLUP);
  pinMode(buttonPin6, INPUT_PULLUP);
  pinMode(buttonPin7, INPUT_PULLUP);

  pinMode(btnLedPin1, OUTPUT);
  pinMode(btnLedPin2, OUTPUT);
  pinMode(btnLedPin3, OUTPUT);
  pinMode(btnLedPin4, OUTPUT);
  pinMode(btnLedPin5, OUTPUT);
  pinMode(btnLedPin6, OUTPUT);
  pinMode(btnLedPin7, OUTPUT);

  pinMode(ch1, INPUT_PULLUP);
  pinMode(ch2, INPUT_PULLUP);
  pinMode(ch3, INPUT_PULLUP);

  rtcFunc();

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
      Serial.println(F("Unable to begin:"));
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
      while(true);
    }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30

  MsTimer2::set(100, enterAnyKey);

  pixelarrayInit();
}

void loop() {
  if(gamestatus == -1){
    MsTimer2::start();
    rtcLed();
  }
  else if(gamestatus == 0) StartingScreen();
  
  else if(gamestatus == 1){
    display_Player();
    enterPlayer();
  }
  if(gamestatus == 2){
    displaySelectColor();
    selectColorPlayer1();
  }
  else if(gamestatus == 3){
    selectColorPlayer2();
  }
  else if(gamestatus == 4){
    display_Score();
    blockBlink();
    WinCheckField_Data();
    select_sw();
  }
  else if(gamestatus == 5){
    display_WinLose();
    endGame();
    initializeGame();
  }
}
