#include "hexapod.h"


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

//LegGroup::LegGroup() {}
//
//void LegGroup::attach(Leg forwardLeg, Leg middleLeg, Leg backwardLeg) {
//    this->forwardLeg = forwardLeg;
//    this->middleLeg = middleLeg;
//    this->backwardLeg = backwardLeg;
//}
//
//void LegGroup::up() {
//    if (this->forwardLeg.isLeft()) {
//        this->forwardLeg.up();
//        this->middleLeg.up();
//        this->backwardLeg.up();
//    } else {
//        this->forwardLeg.up(400, 35);
//        this->middleLeg.up(200, 30);
//        this->backwardLeg.up();
//    }
//}
//
//void LegGroup::forward() {
//    if (this->forwardLeg.isLeft()) {
//        this->forwardLeg.forward(300, 30);
//        this->middleLeg.forward();
//        this->backwardLeg.forward();
//    } else {
//        this->forwardLeg.forward(200, 30);
//        this->middleLeg.forward();
//        this->backwardLeg.forward();
//    }
//}
//
//void LegGroup::down() {
//    if (this->forwardLeg.isLeft()) {
//        this->forwardLeg.down(300, 40);
//        this->middleLeg.down();
//        this->backwardLeg.down();
//    } else {
//        this->forwardLeg.down(300, 10);
//        this->middleLeg.down();
//        this->backwardLeg.down();
//    }
//}
//
//void LegGroup::backward() {
//    if (this->forwardLeg.isLeft()) {
//        this->forwardLeg.backward(300, 30);
//        this->middleLeg.backward();
//        this->backwardLeg.backward();
//    } else {
//        this->forwardLeg.backward();
//        this->middleLeg.backward();
//        this->backwardLeg.backward();
//    }
//}
//
//void LegGroup::stop() {
//    this->forwardLeg.stop();
//    this->middleLeg.stop();
//    this->backwardLeg.stop();
//}

// ---------------------------------------------------------------------------------------------------------------------

Hexapod::Hexapod() {}

void Hexapod::attach(Leg* legs) {
    this->legs = legs;
}

void Hexapod::moveForward() {
    for (int i = 0; i < 6; ++i){
        this->legs[i].up(upDelay[i], upSpeed[i]);
        this->legs[i].forward(forwardDelay[i], forwardSpeed[i]);
        this->legs[i].down(downDelay[i], downSpeed[i]);
    }

    for (int i = 0; i < 6; ++i)
        this->legs->pushBackward(backwardSpeed[i]);

    delay(200);

    for (int i = 0; i < 6; ++i)
        this->legs[i].stopUpperServo();
}

void Hexapod::moveBackward() {
    for (int i = 0; i < 6; ++i){
        this->legs[i].up(upDelay[i], upSpeed[i]);
        this->legs[i].backward(forwardDelay[i], forwardSpeed[i]);
        this->legs[i].down(downDelay[i], downSpeed[i]);
    }

    for (int i = 0; i < 6; ++i)
        this->legs->pushForward(backwardSpeed[i]);

    delay(200);

    for (int i = 0; i < 6; ++i)
        this->legs[i].stopUpperServo();
}

void Hexapod::stop() {
    for (int i = 0; i < 6; ++i)
        this->legs[i].stop();
}
