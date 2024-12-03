#pragma once

#include <Arduino.h>
#include <Servo.h>

int upSpeed[] = {30, 35, 30, 30, 30, 30};
int downSpeed[] = {40, 10, 30, 30, 30, 30};
int forwardSpeed[] = {30, 30, 10, 10, 10, 10};
int backwardSpeed[] = {30, 10, 10, 10, 10, 10};

int upDelay[] = {300, 400, 200, 300, 300, 300};
int downDelay[] = {300, 300, 200, 200, 200, 200};
int forwardDelay[] = {300, 200, 200, 200, 200, 200};
int backwardDelay[] = {300, 200, 200, 200, 200, 200};

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

    void pushForward(int speed = 10);

    void pushBackward(int speed = 10);

    void stopUpperServo();

    void left();

    void right();

    void stop();

    bool isLeft();
};


class Hexapod{
private:
    Leg* legs;

public:
    Hexapod();

    void attach(Leg* legs);

    void moveForward();

    void moveBackward();

    void moveLeft();

    void moveRight();

    void stop();
};
