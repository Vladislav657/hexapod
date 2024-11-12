#include "hexapod.h"


Leg::Leg(int upperPin, int middlePin, int lowerPin) {
    this->upperServo.attach(upperPin);
    this->middleServo.attach(middlePin);
    this->lowerServo.attach(lowerPin);
}

void Leg::up() {
    this->middleServo.write(125);
    this->lowerServo.write(180);
    delay(500);
    this->middleServo.write(90);
}

void Leg::forward() {
    this->upperServo.write(80);
    delay(200);
    this->upperServo.write(90);
}

void Leg::down() {
    this->lowerServo.write(0);
    this->middleServo.write(65);
    delay(250);
    this->middleServo.write(90);
}

void Leg::backward() {
    this->upperServo.write(100);
    delay(200);
    this->upperServo.write(90);
}

LegGroup::LegGroup(Leg forwardLeg, Leg middleLeg, Leg backwardLeg) {
    this->forwardLeg = forwardLeg;
    this->middleLeg = middleLeg;
    this->backwardLeg = backwardLeg;
}

Hexapod::Hexapod(LegGroup A, LegGroup B) {
    this->A = A;
    this->B = B;
}
