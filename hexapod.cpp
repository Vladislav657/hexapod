#include "hexapod.h"

int upSpeed[] = {30, 35, 30, 30, 30, 30};
int downSpeed[] = {30, 30, 30, 30, 30, 25};
int forwardSpeed[] = {30, 30, 30, 10, 30, 20};
int backwardSpeed[] = {30, 40, 30, 10, 30, 35};

int upDelay[] = {300, 300, 300, 300, 300, 350};
int downDelay[] = {400, 300, 300, 300, 300, 300};
int forwardDelay[] = {300, 200, 200, 200, 300, 300};
int backwardDelay[] = {300, 200, 200, 200, 300, 300};

Leg::Leg() {}

void Leg::attach(int upperPin, int middlePin, int lowerPin, enum type t) {
    this->upperServo.attach(upperPin);
    this->middleServo.attach(middlePin);
    this->lowerServo.attach(lowerPin);
    this->t = t;
    if (this->t == r)
        this->lowerServo.write(20);
    else
        this->lowerServo.write(160);
    this->h = back;
    this->v = lower;
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

void Leg::pushDown(int speed) {
    speed *= this->isLeft() ? 1 : -1;
    this->middleServo.write(90 + speed);
}

void Leg::stopUpperServo() {
    this->upperServo.write(90);
}

void Leg::stop(){
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
    int a = this->legs[0].isLeft() ? 0 : 1;

    for (int i = 0; i < 3; ++i) {
        this->legs[i].pushDown();
    }

    for (int i = 0; i < 3; ++i){
        this->legs[i].up(upDelay[2 * i + a], upSpeed[2 * i + a]);
        this->legs[i].forward(forwardDelay[2 * i + a], forwardSpeed[2 * i + a]);
        this->legs[i].down(downDelay[2 * i + a], downSpeed[2 * i + a]);
        this->legs[i].pushDown();
    }

    for (int i = 0; i < 3; ++i)
        this->legs[i].pushBackward(backwardSpeed[2 * i + a]);

    delay(300);

    for (int i = 0; i < 3; ++i)
        this->legs[i].stopUpperServo();
}

void Module::moveBackward() {
    int a = this->legs[0].isLeft() ? 0 : 1;

    for (int i = 0; i < 3; ++i) {
        this->legs[i].pushDown();
    }

    for (int i = 0; i < 3; ++i){
        this->legs[i].up(upDelay[2 * i + a], upSpeed[2 * i + a]);
        this->legs[i].backward(forwardDelay[2 * i + a], forwardSpeed[2 * i + a]);
        this->legs[i].down(downDelay[2 * i + a], downSpeed[2 * i + a]);
        this->legs[i].pushDown();
    }

    for (int i = 0; i < 3; ++i)
        this->legs[i].pushForward(backwardSpeed[2 * i + a]);

    delay(300);

    for (int i = 0; i < 3; ++i)
        this->legs[i].stopUpperServo();
}

void Module::stop() {
    for (int i = 0; i < 3; ++i)
        this->legs[i].stop();
}
