#include "hexapod.h"

int upSpeed[] = {45, 60, -10, 60, 45, 70};
int forwardSpeed[] = {50, 40, 30, 30, 60, 60};
int downSpeed[] = {60, 20, 40, 20, 70, 70};
int backwardSpeed[] = {50, 40, 35, 30, 60, 50};

int upDelay[] = {500, 400, 200, 400, 300, 400};
int forwardDelay[] = {500, 400, 150, 400, 400, 400};
int downDelay[] = {500, 400, 600, 400, 400, 400};
int backwardDelay[] = {500, 400, 200, 400, 400, 400};

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

    speed *= (this->isLeft() ? 1 : -1);

    this->pwm->setPWM(upperPin, 0, 300 + speed);
    delay(duration);
    this->pwm->setPWM(upperPin, 0, 300);
}

void Leg::backward(int duration, int speed) {
    if (this->h == back) return;
    this->h = back;

    speed *= (this->isLeft() ? -1 : 1);

    this->pwm->setPWM(upperPin, 0, 300 + speed);
    delay(duration);
    this->pwm->setPWM(upperPin, 0, 300);
}

// Методы для непрерывного управления без задержки
void Leg::pushForward(int speed) {
    if (this->h == front) return;
    this->h = front;

    speed *= (this->isLeft() ? 1 : -1);

    this->pwm->setPWM(upperPin, 0, 300 + speed);
}

void Leg::pushBackward(int speed) {
    if (this->h == back) return;
    this->h = back;

    speed *= (this->isLeft() ? -1 : 1);

    this->pwm->setPWM(upperPin, 0, 300 + speed);
}

void Leg::pushDown(int speed) {
    speed *= (this->isLeft() ? -1 : 1);
    this->pwm->setPWM(middlePin, 0, 300 + speed);
}

void Leg::pushLowerServo(int speed) {
    this->pwm->setPWM(lowerPin, 0, speed);  // Правая нога
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

Hexapod::Hexapod(Adafruit_PWMServoDriver* left, Adafruit_PWMServoDriver* right){
    this->left = left;
    this->right = right;
    this->count = 0;
}

void Hexapod::attachLeg(int upperPin, int middlePin, int lowerPin, enum type t) {
//    this->legs[this->count] = Leg();
    this->legs[this->count].attach(t == l ? this->left : this->right, upperPin, middlePin, lowerPin, t);
    this->count++;
}

void Hexapod::moveForward(int duration) {
//    for (int i = 0; i < this->count; ++i) {
//        this->legs[i].pushLowerServo(550);
//    }

    for (int i = 0; i < this->count; ++i) {
        this->legs[i].pushDown(downSpeed[i] + 10);
    }
    delay(duration);

    for (int i = 0; i < this->count; ++i){
        this->legs[i].up(upDelay[i], upSpeed[i]);
        this->legs[i].forward(forwardDelay[i], forwardSpeed[i]);
        this->legs[i].down(downDelay[i], downSpeed[i]);
        this->legs[i].pushDown(downSpeed[i] + 10);
    }

    for (int i = 0; i < this->count; ++i)
        this->legs[i].pushBackward(backwardSpeed[i]);

    delay(400);

    for (int i = 0; i < this->count; ++i)
        this->legs[i].stopUpperServo();
}

void Hexapod::moveBackward(int duration) {
//    for (int i = 0; i < this->count; ++i) {
//        this->legs[i].pushLowerServo(600);
//    }

    for (int i = 0; i < this->count; ++i) {
        this->legs[i].pushDown(downSpeed[i] + 10);
    }

    delay(duration);

    for (int i = 0; i < this->count; ++i){
        this->legs[i].up(upDelay[i], upSpeed[i]);
        this->legs[i].backward(backwardDelay[i], backwardSpeed[i]);
        this->legs[i].down(downDelay[i], downSpeed[i]);
        this->legs[i].pushDown(downSpeed[i] + 10);
    }

    for (int i = 0; i < this->count; ++i)
        this->legs[i].pushForward(forwardSpeed[i]);

    delay(400);

    for (int i = 0; i < this->count; ++i)
        this->legs[i].stopUpperServo();
}

void Hexapod::stop() {
    for (int i = 0; i < this->count; ++i)
        this->legs[i].stop();
}
