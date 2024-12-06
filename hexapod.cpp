#include "hexapod.h"
#include <Wire.h>

int upSpeed[] = {30, 35, 30, 30, 30, 30};
int downSpeed[] = {40, 10, 30, 30, 30, 30};
int forwardSpeed[] = {30, 30, 10, 10, 10, 10};
int backwardSpeed[] = {30, 10, 10, 10, 10, 10};

int upDelay[] = {300, 400, 200, 300, 300, 300};
int downDelay[] = {300, 300, 200, 200, 200, 200};
int forwardDelay[] = {300, 200, 200, 200, 200, 200};
int backwardDelay[] = {300, 200, 200, 200, 200, 200};

Leg::Leg() {}

void Leg::attach(int upperPin, int middlePin, int lowerPin, enum type t) {
    this->upperServo.attach(upperPin);
    this->middleServo.attach(middlePin);
    this->lowerServo.attach(lowerPin);
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
    for (int i = 0; i < 3; ++i){
        this->legs[i].up(upDelay[i], upSpeed[i]);
        this->legs[i].forward(forwardDelay[i], forwardSpeed[i]);
        this->legs[i].down(downDelay[i], downSpeed[i]);
    }

    for (int i = 0; i < 3; ++i)
        this->legs[i].pushBackward(backwardSpeed[i]);

    delay(200);

    for (int i = 0; i < 3; ++i)
        this->legs[i].stopUpperServo();
}

void Module::moveBackward() {
    for (int i = 0; i < 3; ++i){
        this->legs[i].up(upDelay[i], upSpeed[i]);
        this->legs[i].backward(forwardDelay[i], forwardSpeed[i]);
        this->legs[i].down(downDelay[i], downSpeed[i]);
    }

    for (int i = 0; i < 3; ++i)
        this->legs[i].pushForward(backwardSpeed[i]);

    delay(200);

    for (int i = 0; i < 3; ++i)
        this->legs[i].stopUpperServo();
}

void Module::stop() {
    for (int i = 0; i < 3; ++i)
        this->legs[i].stop();
}
