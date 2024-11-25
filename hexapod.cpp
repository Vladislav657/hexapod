#include "hexapod.h"

Leg::Leg() {
    Servo upperServo, middleServo, lowerServo;
    this->h = back;
    this->v = lower;
}

void Leg::attach(int upperPin, int middlePin, int lowerPin) {
    this->upperServo.attach(upperPin);
    this->middleServo.attach(middlePin);
    this->lowerServo.attach(lowerPin);
}

void Leg::up() {
    if (this->v == upper)
        return;
    this->v = upper;

    this->middleServo.write(130);
    this->lowerServo.write(180);
    delay(300);
    this->middleServo.write(90);
}

void Leg::forward() {
    if (this->h == front)
        return;
    this->h = front;

    this->upperServo.write(60);
    delay(300);
    this->upperServo.write(90);
}

void Leg::down() {
    if (this->v == lower)
        return;
    this->v = lower;

    this->lowerServo.write(0);
    this->middleServo.write(60);
    delay(300);
    this->middleServo.write(90);
}

void Leg::backward() {
    if (this->h == back)
        return;
    this->h = back;

    this->upperServo.write(120);
    delay(300);
    this->upperServo.write(90);
}

void Leg::stop() {
    if (this->h == front)
        this->backward();
    if (this->v == upper)
        this->down();
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

void LegGroup::stop() {
    this->forwardLeg.stop();
    this->middleLeg.stop();
    this->backwardLeg.stop();
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

void Hexapod::stop() {
    this->A.stop();
    this->B.stop();
}
