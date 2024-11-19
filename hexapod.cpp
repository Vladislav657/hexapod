#include "hexapod.h"


Leg::Leg(int upperPin, int middlePin, int lowerPin) {
    this->h = back;
    this->v = lower;
    this->upperServo.attach(upperPin);
    this->middleServo.attach(middlePin);
    this->lowerServo.attach(lowerPin);
}

void Leg::up() {
    if (this->v == upper)
        return;
    this->v = upper;

    this->middleServo.write(125);
    this->lowerServo.write(180);
    delay(500);
    this->middleServo.write(90);
}

void Leg::forward() {
    if (this->h == front)
        return;
    this->h = front;

    this->upperServo.write(80);
    delay(200);
    this->upperServo.write(90);
}

void Leg::down() {
    if (this->v == lower)
        return;
    this->v = lower;

    this->lowerServo.write(0);
    this->middleServo.write(65);
    delay(250);
    this->middleServo.write(90);
}

void Leg::backward() {
    if (this->h == back)
        return;
    this->h = back;

    this->upperServo.write(100);
    delay(200);
    this->upperServo.write(90);
}

// ---------------------------------------------------------------------------------------------------------------------

LegGroup::LegGroup(Leg forwardLeg, Leg middleLeg, Leg backwardLeg) {
    this->forwardLeg = forwardLeg;
    this->middleLeg = middleLeg;
    this->backwardLeg = backwardLeg;
}

void LegGroup::up() {
    this->forwardLeg.up();
    this->middleLeg.up();
    this->backwardLeg.up();
}

void LegGroup::forward() {
    this->forwardLeg.forward();
    this->middleLeg.forward();
    this->backwardLeg.forward();
}

void LegGroup::down() {
    this->forwardLeg.down();
    this->middleLeg.down();
    this->backwardLeg.down();
}

void LegGroup::backward() {
    this->forwardLeg.backward();
    this->middleLeg.backward();
    this->backwardLeg.backward();
}

// ---------------------------------------------------------------------------------------------------------------------

Hexapod::Hexapod(LegGroup A, LegGroup B) {
    this->A = A;
    this->B = B;
}

void Hexapod::moveForward() {
    this->B.down();
    this->A.up();
    this->A.forward();
    this->B.backward();
    this->A.down();
    this->B.up();
    this->B.forward();
    this->A.backward();
}

void Hexapod::moveBackward() {
    this->B.down();
    this->A.up();
    this->A.backward();
    this->B.forward();
    this->A.down();
    this->B.up();
    this->B.backward();
    this->A.forward();
}
