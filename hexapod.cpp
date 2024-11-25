#include "hexapod.h"

Leg::Leg() {}

void Leg::attach(int upperPin, int middlePin, int lowerPin, enum type t) {
    this->upperServo.attach(upperPin);
    this->middleServo.attach(middlePin);
    this->lowerServo.attach(lowerPin);
    this->h = back;
    this->v = lower;
    this->t = t;
}

void Leg::up() {
    if (this->v == upper)
        return;
    this->v = upper;

    if (this->t == r)
        this->middleServo.write(130);
    else
        this->middleServo.write(50);
    this->lowerServo.write(160);

    delay(200);
    this->middleServo.write(90);
}

void Leg::forward() {
    if (this->h == front)
        return;
    this->h = front;

    if (this->t == r)
        this->upperServo.write(60);
    else
        this->upperServo.write(120);

    delay(200);
    this->upperServo.write(90);
}

void Leg::down() {
    if (this->v == lower)
        return;
    this->v = lower;

    this->lowerServo.write(0);
    if (this->t == r)
        this->middleServo.write(60);
    else
        this->middleServo.write(120);

    delay(200);
    this->middleServo.write(90);
}

void Leg::backward() {
    if (this->h == back)
        return;
    this->h = back;

    if (this->t == r)
        this->upperServo.write(120);
    else
        this->upperServo.write(60);

    delay(200);
    this->upperServo.write(90);
}

void Leg::stop() {
    this->up();
    this->backward();
    this->down();
}

// ---------------------------------------------------------------------------------------------------------------------

LegGroup::LegGroup() {}

void LegGroup::attach(Leg forwardLeg, Leg middleLeg, Leg backwardLeg) {
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

Hexapod::Hexapod() {}

void Hexapod::attach(LegGroup A, LegGroup B) {
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
