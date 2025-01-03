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

    void attach(int upperPin, int middlePin, int lowerPin, enum type t);

    void up(int d = 300, int speed = 30);

    void down(int d = 200, int speed = 30);

    void forward(int d = 200, int speed = 10);

    void backward(int d = 200, int speed = 10);

    void pushForward(int speed = 10);

    void pushBackward(int speed = 10);

    void pushDown(int speed = 10);

    void stopUpperServo();

    void stop();

    bool isLeft();
};


class Module{
private:
    Leg* legs;

public:
    Module();

    void attach(Leg* legs);

    void moveForward();

    void moveBackward();

    void stop();
};
