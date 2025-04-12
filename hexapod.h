#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

enum vertical {upper, lower};
enum horizontal {front, back};
enum type {l, r};

class Leg{
private:
    enum vertical v;
    enum horizontal h;
    enum type t;

    Adafruit_PWMServoDriver& pwm;

    // Пины на PCA9685
    int upperPin;
    int middlePin;
    int lowerPin;

public:
    Leg();

    void attach(Adafruit_PWMServoDriver& driver, int upperPin, int middlePin, int lowerPin, enum type t);

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


class Hexapod{
private:
    Adafruit_PWMServoDriver& pwm;
    Leg legs[50];
    int count;

public:
    Hexapod(Adafruit_PWMServoDriver& driver);

    void attachLeg(int upperPin, int middlePin, int lowerPin, enum type t);

    void moveForward();

    void moveBackward();

//    void turnLeft();

//    void turnRight();

    void stop();
};
