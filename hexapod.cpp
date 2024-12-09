#include "hexapod.h"
#include <Wire.h>

int upSpeed[] = {30, 30, 30, 20, 30, 35};
int downSpeed[] = {40, 20, 30, 20, 30, 30};
int forwardSpeed[] = {30, 30, 10, 10, 10, 20};
int backwardSpeed[] = {30, 40, 10, 10, 10, 35};

int upDelay[] = {300, 400, 200, 300, 300, 300};
int downDelay[] = {300, 300, 200, 300, 200, 300};
int forwardDelay[] = {300, 200, 200, 200, 300, 300};
int backwardDelay[] = {300, 200, 200, 200, 300, 300};

Leg::Leg() {}

void Leg::attach(int upperPin, int middlePin, int lowerPin, enum type t) {
    this->upperServo.attach(upperPin);
    this->middleServo.attach(middlePin);
    this->lowerServo.attach(lowerPin);
    if (t == r)
        this->lowerServo.write(0);
    else
        this->lowerServo.write(180);
    this->h = back;
    this->v = lower;
    this->t = t;
}

void Leg::up(int d, int speed) {
    if (this->v == upper)
        return;
    this->v = upper;
    speed *= this->isLeft() ? -1 : 1;

    this->middleServo.write(90 + speed);
    delay(d);
    this->middleServo.write(90);
}

void Leg::down(int d, int speed) {
    if (this->v == lower)
        return;
    this->v = lower;
    speed *= this->isLeft() ? 1 : -1;
//    if (this->t == r) {
//        this->lowerServo.write(0);
//    else {
//        this->lowerServo.write(180);
    this->middleServo.write(90 + speed);
    delay(d);
    this->middleServo.write(90);
}

void Leg::forward(int d, int speed) {
    if (this->h == front)
        return;
    this->h = front;
    speed *= this->isLeft() ? 1 : -1;

    this->upperServo.write(90 + speed);
    delay(d);
    this->upperServo.write(90);
}

void Leg::backward(int d, int speed) {
    if (this->h == back)
        return;
    this->h = back;
    speed *= this->isLeft() ? -1 : 1;

    this->upperServo.write(90 + speed);
    delay(d);
    this->upperServo.write(90);
}

void Leg::pushForward(int speed) {
    if (this->h == front)
        return;
    this->h = front;
    speed *= this->isLeft() ? 1 : -1;
    this->upperServo.write(90 + speed);
}

void Leg::pushBackward(int speed) {
    if (this->h == back)
        return;
    this->h = back;
    speed *= this->isLeft() ? -1 : 1;
    this->upperServo.write(90 + speed);
}

void Leg::stopUpperServo() {
    this->upperServo.write(90);
}

void Leg::stop() {
    this->up();
    this->backward();
    this->down();
}

bool Leg::isLeft() {
    return this->t == l;
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

Module::Module() {}

void Module::attach(Leg* legs) {
    this->legs = legs;
}

void Module::moveForward() {
    int a = 1;
    if (this->legs[0].isLeft())
        a--;

    for (int i = 0; i < 3; ++i){
        this->legs[i].up(upDelay[2 * i + a], upSpeed[2 * i + a]);
        this->legs[i].forward(forwardDelay[2 * i + a], forwardSpeed[2 * i + a]);
        this->legs[i].down(downDelay[2 * i + a], downSpeed[2 * i + a]);
    }

    for (int i = 0; i < 3; ++i)
        this->legs[i].pushBackward(backwardSpeed[2 * i + a]);

    delay(200);

    for (int i = 0; i < 3; ++i)
        this->legs[i].stopUpperServo();
}

void Module::moveBackward() {
    int a = 1;
    if (this->legs[0].isLeft())
        a--;

    for (int i = 0; i < 3; ++i){
        this->legs[i].up(upDelay[2 * i + a], upSpeed[2 * i + a]);
        this->legs[i].backward(forwardDelay[2 * i + a], forwardSpeed[2 * i + a]);
        this->legs[i].down(downDelay[2 * i + a], downSpeed[2 * i + a]);
    }

    for (int i = 0; i < 3; ++i)
        this->legs[i].pushForward(backwardSpeed[2 * i + a]);

    delay(200);

    for (int i = 0; i < 3; ++i)
        this->legs[i].stopUpperServo();
}

void Module::stop() {
    for (int i = 0; i < 3; ++i)
        this->legs[i].stop();
}
