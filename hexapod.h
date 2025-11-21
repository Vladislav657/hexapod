#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//enum vertical {upper, lower};
//enum horizontal {front, back};
enum type {l, r};

class Leg{
public:
//    enum vertical v;
//    enum horizontal h;
    enum type t;

    Adafruit_PWMServoDriver* pwm;

    // Пины на PCA9685
    int horizontalPin;
    int verticalPin;
//    int lowerPin;
    int buttonPin;
    int lastButtonState;

public:
    Leg();

    void attach(Adafruit_PWMServoDriver* driver, int horizontalPin, int verticalPin, int buttonPin, enum type t);

    void up();

    void down();

    void forward();

    void backward();

    void pushForward();

    void pushBackward();

//    void pushDown(int speed);

//    void pushLowerServo(int speed);

    void stopHorizontal();

    void stop();

    bool isLeft();
};


class Hexapod{
private:
    Adafruit_PWMServoDriver* left, *right;
    Leg legs[50];
    int count;

public:
    Hexapod(Adafruit_PWMServoDriver* left, Adafruit_PWMServoDriver* right);

    void attachLeg(int horizontalPin, int verticalPin, enum type t);

    void moveForward();

    void moveBackward();

//    void turnLeft();

//    void turnRight();

    void stop();
};
