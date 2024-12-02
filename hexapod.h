#pragma once

#include <Arduino.h>
#include <Servo.h>

enum vertical {upper, lower};
enum horizontal {front, back};
enum type {l, r};

class Leg{
private:
    Servo upperServo, middleServo, lowerServo;
    enum vertical v;
    enum horizontal h;
    enum type t;

public:
    Leg();

    void attach(int upperPin, int middlePin, int lowerPin, enum type);

    void up(int d = 300, int speed = 30);

    void down(int d = 200, int speed = 30);

    void forward(int d = 200, int speed = 10);

    void backward(int d = 200, int speed = 10);

    void left();

    void right();

    void stop();

    bool isLeft();
};

class LegGroup{
private:
    Leg forwardLeg, middleLeg, backwardLeg;

public:
    LegGroup();

    void attach(Leg forwardLeg, Leg middleLeg, Leg backwardLeg);

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
    Hexapod();

    void attach(LegGroup A, LegGroup B);

    void moveForward();

    void moveBackward();

    void moveLeft();

    void moveRight();

    void stop();
};
