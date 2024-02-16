#include <Arduino.h>
#include <axis.h>

void getInputs();
void modeInterrupt();

const bool zAxisDirection = false;
const int axisDeadzone = 60;
int midPoint = 512;

int xInPin = A6;
int yInPin = A5;
int zUpPin = 8;
int zDownPin = 9;
int xInvertPin = 6;
int yInvertPin = 7;
int modePin = 2;

int xDirOut = A1;
int xStepOut = A0;
int yDirOut = A4;
int yStepOut = A3;
int zDirOut = 11;
int zStepOut = 12;

int xCurrentState = 0;
int yCurrentState = 0;
int zCurrentState = 0;
volatile bool currentMode = false;

unsigned long xTimer = 0;
unsigned long yTimer = 0;
unsigned long zTimer = 0;
unsigned long continuousTimer = 0;
volatile unsigned long modeTimer = 0;

bool zUpPressed = false;
bool zDownPressed = false;
bool xUsed = false;
bool yUsed = false;
bool xInverted = false;
bool yInverted = false;

Axis* xAxis;
Axis* yAxis;
Axis* zAxis;


void setup() {
    pinMode(xInPin, INPUT);
    pinMode(yInPin, INPUT);
    pinMode(zUpPin, INPUT_PULLUP);
    pinMode(zDownPin, INPUT_PULLUP);
    pinMode(xInvertPin, INPUT_PULLUP);
    pinMode(yInvertPin, INPUT_PULLUP);
    pinMode(modePin, INPUT_PULLUP);

    pinMode(xDirOut, OUTPUT);
    pinMode(xStepOut, OUTPUT);
    pinMode(yDirOut, OUTPUT);
    pinMode(yStepOut, OUTPUT);
    pinMode(zDirOut, OUTPUT);
    pinMode(zStepOut, OUTPUT);

    xAxis = new Axis(xDirOut, xStepOut, axisDeadzone, 100, false);
    yAxis = new Axis(yDirOut, yStepOut, axisDeadzone, 100, false);
    zAxis = new Axis(zDirOut, zStepOut, axisDeadzone, 200, true);
    zAxis->setDirection(zAxisDirection);

    attachInterrupt(digitalPinToInterrupt(modePin), modeInterrupt, FALLING);

    interrupts();
}

void loop() {
    while(true){
        getInputs();
        xAxis->update(xCurrentState, currentMode);
        yAxis->update(yCurrentState, currentMode);
        zAxis->update(zCurrentState, currentMode);
    }
}

void getInputs(){
    xCurrentState = analogRead(xInPin);
    yCurrentState = analogRead(yInPin);
    zCurrentState = digitalRead(zUpPin) == LOW ? 1024 : digitalRead(zDownPin) == LOW ? 0 : 512;
    bool xInvertedIn = digitalRead(xInvertPin) == LOW ? true : false;
    if(xInvertedIn != xInverted){
        xInverted = xInvertedIn;
        xAxis->reverse();
    }
    bool yInvertedIn = digitalRead(yInvertPin) == LOW ? true : false;
    if(yInvertedIn != yInverted){
        yInverted = yInvertedIn;
        yAxis->reverse();
    }
}

void modeInterrupt(){
    unsigned long currentTime = millis();
    if(currentTime - modeTimer > 100){
        currentMode = !currentMode;
        modeTimer = currentTime;
    }
}