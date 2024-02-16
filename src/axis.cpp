#include <axis.h>

Axis::Axis() {
    this->reversed = false;
    this->previousState = midPoint;
    this->deadzone = 100;
    this->timer = 0;
    this->slowModeSpeed = 200;
    this->turbo = false;
}

Axis::Axis(int dirPin, int stepPin, int deadzone, int slowModeSpeed, bool turboAxis) {
    this->dir = dirPin;
    this->step = stepPin;
    this->deadzone = deadzone;
    this->range = float(midPoint - deadzone);
    this->reversed = false;
    this->previousState = midPoint;
    this->timer = 0;
    this->slowModeSpeed = slowModeSpeed;
    this->turbo = turboAxis;
}

void Axis::reverse() {
    this->reversed = !this->reversed;
}

void Axis::setDirection(bool newDirection) {
    this->reversed = newDirection;
}

void Axis::update(int input, bool mode) {
    unsigned long currentTime = millis();
    if(this->stepping){
        if(currentTime - this->timer == 0){ return; }
        else {
            this->stepping = false;
            this->timer = currentTime;
            digitalWrite(this->step, LOW);
            return;
        }
    }
    int amount = input - midPoint;
    if(abs(amount) > this->deadzone){
        if(this->reversed){ amount = -amount; }
        float scaler = amount / (midPoint - this->deadzone);
        scaler = scaler > 0.0 ? 1.0 - scaler : -(1.0 + scaler);
        unsigned int stepLength = 1;
        if(mode){
            stepLength = this->slowModeSpeed + (int)(abs(scaler) * 1000.0);
        }
        else{
            stepLength = 1 + (int)(abs(scaler) * (float)slowModeSpeed);
        }
        if(this->turbo && !mode){
            digitalWrite(this->dir, amount > 0 ? HIGH : LOW);
            digitalWrite(this->step, HIGH);
            delayMicroseconds(100);
            digitalWrite(this->step, LOW);
        }
        if(currentTime - this->timer > stepLength){
            digitalWrite(this->dir, amount > 0 ? HIGH : LOW);
            digitalWrite(this->step, HIGH);
            this->timer = currentTime;
            this->stepping = true;
        }
    }
    else { return; }
}