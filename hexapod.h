#pragma once

#include <Arduino.h>
#include <Servo.h>

class Leg{
private:
    Servo upperServo, middleServo, lowerServo;

public:
    Leg();

    Leg(int upperPin, int middlePin, int lowerPin);

    void up();

    void down();

    void forward();

    void backward();

    void left();

    void right();

    void setDefault();
};

class LegGroup{
private:
    Leg forwardLeg, middleLeg, backwardLeg;

public:
    LegGroup();

    LegGroup(Leg forwardLeg, Leg middleLeg, Leg backwardLeg);

    void up();

    void down();

    void forward();

    void backward();

    void left();

    void right();

    void setDefault();
};

class Hexapod{
private:
    LegGroup A, B;

public:
    Hexapod(LegGroup A, LegGroup B);

    void moveForward();

    void moveBackward();

    void moveLeft();

    void moveRight();
};
