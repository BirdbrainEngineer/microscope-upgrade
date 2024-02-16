#include <Arduino.h>

extern int midPoint;

class Axis{
    public:
        int dir;
        int step;

        Axis();
        Axis(int dirPin, int stepPin, int deadzone, int slowModeSpeed, bool turboAxis);
        void reverse();
        void setDirection(bool newDirection);
        void update(int input, bool mode);

    private:
        int deadzone;
        float range;
        int previousState;
        bool stepping;
        bool reversed;
        int slowModeSpeed;
        unsigned long timer;
        bool turbo;
};