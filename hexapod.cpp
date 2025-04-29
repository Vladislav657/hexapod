#include "hexapod.h"

//int upSpeed[] = {30, 35, 30, 30, 30, 30};
//int downSpeed[] = {30, 30, 30, 30, 30, 25};
//int forwardSpeed[] = {30, 30, 30, 10, 30, 20};
//int backwardSpeed[] = {30, 40, 30, 10, 30, 35};
//
//int upDelay[] = {300, 300, 300, 300, 300, 350};
//int downDelay[] = {400, 300, 300, 300, 300, 300};
//int forwardDelay[] = {300, 200, 200, 200, 300, 300};
//int backwardDelay[] = {300, 200, 200, 200, 300, 300};

#define SERVOMIN  200 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  400 // This is the 'maximum' pulse length count (out of 4096)
#define SERVOSTOP 300


Leg::Leg() {}

void Leg::attach(Adafruit_PWMServoDriver* driver, int upperPin, int middlePin, int lowerPin, type t) {
    this->pwm = driver;
    this->upperPin = upperPin;
    this->middlePin = middlePin;
    this->lowerPin = lowerPin;
    this->t = t;

    // Инициализация нижнего сервопривода (0-180°)
//    if (this->t == r) {
//        this->pwm.setPWM(lowerPin, 0, 200);  // Правая нога
//    } else {
//        this->pwm.setPWM(lowerPin, 0, 550); // Левая нога
//    }
    this->h = back;
    this->v = lower;

    // Остановка сервоприводов постоянного вращения
//    this->pwm.setPWM(upperPin, 0, SERVOSTOP);
//    this->pwm.setPWM(middlePin, 0, SERVOSTOP);
}

void Leg::up(int duration, int speed) { // 0 - 225
    if (this->v == upper) return;
    this->v = upper;

    speed *= (this->isLeft() ? 1 : -1);

    this->pwm->setPWM(middlePin, 0, 300 + speed);
    delay(duration);
    this->pwm->setPWM(middlePin, 0, 300);
}

void Leg::down(int duration, int speed) {
    if (this->v == lower) return;
    this->v = lower;

    speed *= (this->isLeft() ? -1 : 1);

    this->pwm->setPWM(middlePin, 0, 300 + speed);
    delay(duration);
    this->pwm->setPWM(middlePin, 0, 300);
}

void Leg::forward(int duration, int speed) {
    if (this->h == front) return;
    this->h = front;

    speed *= (this->isLeft() ? -1 : 1);

    this->pwm->setPWM(upperPin, 0, 300 + speed);
    delay(duration);
    this->pwm->setPWM(upperPin, 0, 300);
}

void Leg::backward(int duration, int speed) {
    if (this->h == back) return;
    this->h = back;

    speed *= (this->isLeft() ? 1 : -1);

    this->pwm->setPWM(upperPin, 0, 300 + speed);
    delay(duration);
    this->pwm->setPWM(upperPin, 0, 300);
}

// Методы для непрерывного управления без задержки
void Leg::pushForward(int speed) {
    if (this->h == front) return;
    this->h = front;

    speed *= (this->isLeft() ? -1 : 1);

    this->pwm->setPWM(upperPin, 0, 300 + speed);
}

void Leg::pushBackward(int speed) {
    if (this->h == back) return;
    this->h = back;

    speed *= (this->isLeft() ? 1 : -1);

    this->pwm->setPWM(upperPin, 0, 300 + speed);
}

void Leg::pushDown(int speed) {
    speed *= (this->isLeft() ? -1 : 1);
    this->pwm->setPWM(middlePin, 0, 300 + speed);
}

void Leg::stopUpperServo() {
    this->pwm->setPWM(upperPin, 0, 300);
}

void Leg::stop() {
    this->stopUpperServo();
    this->pwm->setPWM(middlePin, 0, 300);
}

bool Leg::isLeft() {
    return this->t == l;
}


// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

Hexapod::Hexapod(Adafruit_PWMServoDriver* driver){
    this->pwm = driver;
    this->count = 0;
}

void Hexapod::attachLeg(int upperPin, int middlePin, int lowerPin, enum type t) {
    this->legs[this->count].attach(this->pwm, upperPin, middlePin, lowerPin, t);
    this->count++;
}

void Hexapod::moveForward(int duration, int speed) {
    for (int i = 0; i < this->count; ++i) {
        this->legs[i].pushDown(speed);
    }

    for (int i = 0; i < this->count; ++i){
        this->legs[i].up(duration, speed);
        this->legs[i].forward(duration, speed);
        this->legs[i].down(duration, speed);
        this->legs[i].pushDown(speed);
    }

    for (int i = 0; i < this->count; ++i)
        this->legs[i].pushBackward(speed);

    delay(duration);

    for (int i = 0; i < this->count; ++i)
        this->legs[i].stopUpperServo();
}

void Hexapod::moveBackward(int duration, int speed) {
    for (int i = 0; i < this->count; ++i) {
        this->legs[i].pushDown(speed);
    }

    for (int i = 0; i < this->count; ++i){
        this->legs[i].up(duration, speed);
        this->legs[i].backward(duration, speed);
        this->legs[i].down(duration, speed);
        this->legs[i].pushDown(speed);
    }

    for (int i = 0; i < this->count; ++i)
        this->legs[i].pushForward(speed);

    delay(duration);

    for (int i = 0; i < this->count; ++i)
        this->legs[i].stopUpperServo();
}

void Hexapod::stop() {
    for (int i = 0; i < this->count; ++i)
        this->legs[i].stop();
}
