#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

enum vertical {upper, lower};
enum horizontal {front, back};
enum type {l, r};

class Leg{
public:
    enum vertical v;
    enum horizontal h;
    enum type t;

    Adafruit_PWMServoDriver* pwm;

    // Пины на PCA9685
    int upperPin;
    int middlePin;
    int lowerPin;

public:
    Leg();

    void attach(Adafruit_PWMServoDriver* driver, int upperPin, int middlePin, int lowerPin, enum type t);

    void up(int duration, int speed);

    void down(int duration, int speed);

    void forward(int duration, int speed);

    void backward(int duration, int speed);

    void pushForward(int speed);

    void pushBackward(int speed);

    void pushDown(int speed);

    void stopUpperServo();

    void stop();

    bool isLeft();
};


class Hexapod{
private:
    Adafruit_PWMServoDriver* pwm;
    Leg legs[50];
    int count;

public:
    Hexapod(Adafruit_PWMServoDriver* driver);

    void attachLeg(int upperPin, int middlePin, int lowerPin, enum type t);

    void moveForward(int duration, int speed);

    void moveBackward(int duration, int speed);

//    void turnLeft();

//    void turnRight();

    void stop();
};
