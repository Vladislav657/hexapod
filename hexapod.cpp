#include "hexapod.h"

int upSpeed[] = {30, 35, 30, 30, 30, 30};
int downSpeed[] = {30, 30, 30, 30, 30, 25};
int forwardSpeed[] = {30, 30, 30, 10, 30, 20};
int backwardSpeed[] = {30, 40, 30, 10, 30, 35};

int upDelay[] = {300, 300, 300, 300, 300, 350};
int downDelay[] = {400, 300, 300, 300, 300, 300};
int forwardDelay[] = {300, 200, 200, 200, 300, 300};
int backwardDelay[] = {300, 200, 200, 200, 300, 300};

static const uint16_t SERVO_STOP = 307;    // ~1500μs (12-bit для 50Hz)
static const uint16_t SERVO_MIN = 266;     // ~1300μs
static const uint16_t SERVO_MAX = 348;     // ~1700μs
static const uint16_t SERVO_LOWER_MIN = 102;  // ~500μs (0°)
static const uint16_t SERVO_LOWER_MAX = 512;  // ~2500μs (180°)


int angleToPulse(int angle) {
    angle = constrain(angle, 0, 180);
    return map(angle, 0, 180, SERVO_LOWER_MIN, SERVO_LOWER_MAX);
}


//Leg::Leg() {}
//
//void Leg::attach(int upperPin, int middlePin, int lowerPin, enum type t) {
//    this->upperServo.attach(upperPin);
//    this->middleServo.attach(middlePin);
//    this->lowerServo.attach(lowerPin);
//    this->t = t;
//    if (this->t == r)
//        this->lowerServo.write(20);
//    else
//        this->lowerServo.write(160);
//    this->h = back;
//    this->v = lower;
//}
//
//void Leg::up(int d, int speed) {
//    if (this->v == upper)
//        return;
//    this->v = upper;
//    speed *= this->isLeft() ? -1 : 1;
//
//    this->middleServo.write(90 + speed);
//    delay(d);
//    this->middleServo.write(90);
//}
//
//void Leg::down(int d, int speed) {
//    if (this->v == lower)
//        return;
//    this->v = lower;
//    speed *= this->isLeft() ? 1 : -1;
//
//    this->middleServo.write(90 + speed);
//    delay(d);
//    this->middleServo.write(90);
//}
//
//void Leg::forward(int d, int speed) {
//    if (this->h == front)
//        return;
//    this->h = front;
//    speed *= this->isLeft() ? 1 : -1;
//
//    this->upperServo.write(90 + speed);
//    delay(d);
//    this->upperServo.write(90);
//}
//
//void Leg::backward(int d, int speed) {
//    if (this->h == back)
//        return;
//    this->h = back;
//    speed *= this->isLeft() ? -1 : 1;
//
//    this->upperServo.write(90 + speed);
//    delay(d);
//    this->upperServo.write(90);
//}
//
//void Leg::pushForward(int speed) {
//    if (this->h == front)
//        return;
//    this->h = front;
//    speed *= this->isLeft() ? 1 : -1;
//    this->upperServo.write(90 + speed);
//}
//
//void Leg::pushBackward(int speed) {
//    if (this->h == back)
//        return;
//    this->h = back;
//    speed *= this->isLeft() ? -1 : 1;
//    this->upperServo.write(90 + speed);
//}
//
//void Leg::pushDown(int speed) {
//    speed *= this->isLeft() ? 1 : -1;
//    this->middleServo.write(90 + speed);
//}
//
//void Leg::stopUpperServo() {
//    this->upperServo.write(90);
//}
//
//void Leg::stop(){
//    this->backward();
//    this->down();
//}
//
//bool Leg::isLeft() {
//    return this->t == l;
//}

Leg::Leg() {}

void Leg::attach(Adafruit_PWMServoDriver& driver, int upperPin, int middlePin, int lowerPin, type t) {
    this->pwm = driver;
    this->upperPin = upperPin;
    this->middlePin = middlePin;
    this->lowerPin = lowerPin;
    this->t = t;

    // Инициализация нижнего сервопривода (0-180°)
    if (this->t == r) {
        this->pwm.setPWM(lowerPin, 0, angleToPulse(20));  // Правая нога
    } else {
        this->pwm.setPWM(lowerPin, 0, angleToPulse(160)); // Левая нога
    }

    this->h = back;
    this->v = lower;

    // Остановка сервоприводов постоянного вращения
    this->pwm.setPWM(upperPin, 0, SERVO_STOP);
    this->pwm.setPWM(middlePin, 0, SERVO_STOP);
}

void Leg::up(int duration, int speed) {
    if (this->v == upper) return;
    this->v = upper;

    speed = constrain(speed, 0, 100);
    speed *= (this->isLeft() ? -1 : 1);

    int pulse = map(speed, -100, 100, SERVO_MIN, SERVO_MAX);
    this->pwm.setPWM(middlePin, 0, pulse);
    delay(duration);
    this->pwm.setPWM(middlePin, 0, SERVO_STOP);
}

void Leg::down(int duration, int speed) {
    if (this->v == lower) return;
    this->v = lower;

    speed = constrain(speed, 0, 100);
    speed *= (this->isLeft() ? 1 : -1);

    int pulse = map(speed, -100, 100, SERVO_MIN, SERVO_MAX);
    this->pwm.setPWM(middlePin, 0, pulse);
    delay(duration);
    this->pwm.setPWM(middlePin, 0, SERVO_STOP);
}

void Leg::forward(int duration, int speed) {
    if (this->h == front) return;
    this->h = front;

    speed = constrain(speed, 0, 100);
    speed *= (this->isLeft() ? 1 : -1);

    int pulse = map(speed, -100, 100, SERVO_MIN, SERVO_MAX);
    this->pwm.setPWM(upperPin, 0, pulse);
    delay(duration);
    this->pwm.setPWM(upperPin, 0, SERVO_STOP);
}

void Leg::backward(int duration, int speed) {
    if (this->h == back) return;
    this->h = back;

    speed = constrain(speed, 0, 100);
    speed *= (this->isLeft() ? -1 : 1);

    int pulse = map(speed, -100, 100, SERVO_MIN, SERVO_MAX);
    this->pwm.setPWM(upperPin, 0, pulse);
    delay(duration);
    this->pwm.setPWM(upperPin, 0, SERVO_STOP);
}

// Методы для непрерывного управления без задержки
void Leg::pushForward(int speed) {
    if (this->h == front) return;
    this->h = front;

    speed = constrain(speed, 0, 100);
    speed *= (this->isLeft() ? 1 : -1);

    this->pwm.setPWM(upperPin, 0, map(speed, -100, 100, SERVO_MIN, SERVO_MAX));
}

void Leg::pushBackward(int speed) {
    if (this->h == back) return;
    this->h = back;

    speed = constrain(speed, 0, 100);
    speed *= (this->isLeft() ? -1 : 1);

    this->pwm.setPWM(upperPin, 0, map(speed, -100, 100, SERVO_MIN, SERVO_MAX));
}

void Leg::pushDown(int speed) {
    speed = constrain(speed, 0, 100);
    speed *= (this->isLeft() ? 1 : -1);

    this->pwm.setPWM(middlePin, 0, map(speed, -100, 100, SERVO_MIN, SERVO_MAX));
}

void Leg::stopUpperServo() {
    this->pwm.setPWM(upperPin, 0, SERVO_STOP);
}

void Leg::stop() {
    this->stopUpperServo();
    this->pwm.setPWM(middlePin, 0, SERVO_STOP);
}

bool Leg::isLeft() {
    return this->t == l;
}


// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

Hexapod::Hexapod(Adafruit_PWMServoDriver& driver){
    this->pwm = driver;
    this->count = 0;
}

void Hexapod::attachLeg(int upperPin, int middlePin, int lowerPin, enum type t) {
    this->legs[this->count].attach(this->pwm, upperPin, middlePin, lowerPin, t);
    this->count++;
}

void Hexapod::moveForward() {
    for (int i = 0; i < this->count; ++i) {
        this->legs[i].pushDown();
    }

    for (int i = 0; i < this->count; ++i){
        this->legs[i].up(upDelay[i], upSpeed[i]);
        this->legs[i].forward(forwardDelay[i], forwardSpeed[i]);
        this->legs[i].down(downDelay[i], downSpeed[i]);
        this->legs[i].pushDown();
    }

    for (int i = 0; i < this->count; ++i)
        this->legs[i].pushBackward(backwardSpeed[i]);

    delay(300);

    for (int i = 0; i < this->count; ++i)
        this->legs[i].stopUpperServo();
}

void Hexapod::moveBackward() {
    for (int i = 0; i < this->count; ++i) {
        this->legs[i].pushDown();
    }

    for (int i = 0; i < this->count; ++i){
        this->legs[i].up(upDelay[i], upSpeed[i]);
        this->legs[i].backward(forwardDelay[i], forwardSpeed[i]);
        this->legs[i].down(downDelay[i], downSpeed[i]);
        this->legs[i].pushDown();
    }

    for (int i = 0; i < this->count; ++i)
        this->legs[i].pushForward(backwardSpeed[i]);

    delay(300);

    for (int i = 0; i < this->count; ++i)
        this->legs[i].stopUpperServo();
}

void Hexapod::stop() {
    for (int i = 0; i < this->count; ++i)
        this->legs[i].stop();
}
