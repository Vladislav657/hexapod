#pragma once

#include <Arduino.h>
#include <Servo.h>

enum vertical {upper, lower};
enum horizontal {front, back};

class Leg{
private:
    Servo upperServo, middleServo, lowerServo;
    enum vertical v;
    enum horizontal h;

public:
    Leg();

    void attach(int upperPin, int middlePin, int lowerPin);

    void up();

    void down();

    void forward();

    void backward();

    void left();

    void right();

    void stop();
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

    void stop();
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

    void stop();
};
