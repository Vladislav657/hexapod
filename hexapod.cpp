#include "hexapod.h"

//int upSpeed[] = {45, 60, -10, 60, 45, 70};
//int forwardSpeed[] = {50, 40, 30, 30, 60, 60};
//int downSpeed[] = {60, 20, 40, 20, 70, 70};
//int backwardSpeed[] = {50, 40, 35, 30, 60, 50};
//
//int upDelay[] = {500, 400, 200, 400, 300, 400};
//int forwardDelay[] = {500, 400, 150, 400, 400, 400};
//int downDelay[] = {500, 400, 600, 400, 400, 400};
//int backwardDelay[] = {500, 400, 200, 400, 400, 400};

#define SERVOMIN  200 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  400 // This is the 'maximum' pulse length count (out of 4096)
#define SERVOSTOP 300

#define UPANGLE 120
#define LOWANGLE 60


Leg::Leg() {}

void Leg::attach(Adafruit_PWMServoDriver* driver, int horizontalPin, int verticalPin, int buttonPin, type t) {
    this->pwm = driver;
    this->horizontalPin = horizontalPin;
    this->verticalPin = verticalPin;
//    this->lowerPin = lowerPin;
    this->t = t;
//    this->h = back;
//    this->v = lower;
    this->buttonPin = buttonPin;
    this->lastButtonState = HIGH;
}

void Leg::up() { // 0 - 225
    int angle = (this->isLeft() ? UPANGLE : LOWANGLE);
    int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);

    // Устанавливаем ШИМ
    this->pwm.setPWM(this->verticalPin, 0, pulse);
}

void Leg::down() {
    int angle = (this->isLeft() ? LOWANGLE : UPANGLE);
    int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);

    // Устанавливаем ШИМ
    this->pwm.setPWM(this->verticalPin, 0, pulse);
}

void Leg::forward() {
//    if (this->h == front) return;
//    this->h = front;
    int speed = (this->isLeft() ? SERVOMAX : SERVOMIN);
    int currentButtonState = digitalRead(this->buttonPin);

    while (!(this->lastButtonState == HIGH && currentButtonState == LOW)) {
        this->lastButtonState = currentButtonState;
        this->pwm->setPWM(this->horizontalPin, 0, speed);
        delay(10);
        int currentButtonState = digitalRead(this->buttonPin);
    }

    this->pwm->setPWM(horizontalPin, 0, SERVOSTOP);
}

void Leg::backward() {
//    if (this->h == back) return;
//    this->h = back;
    int speed *= (this->isLeft() ? SERVOMIN : SERVOMAX);
    int currentButtonState = digitalRead(this->buttonPin);

    while (!(this->lastButtonState == HIGH && currentButtonState == LOW)) {
        this->lastButtonState = currentButtonState;
        this->pwm->setPWM(this->horizontalPin, 0, speed);
        delay(10);
        int currentButtonState = digitalRead(this->buttonPin);
    }

    this->pwm->setPWM(horizontalPin, 0, SERVOSTOP);
}

// Методы для непрерывного управления без задержки
void Leg::pushForward() {
//    if (this->h == front) return;
//    this->h = front;
//    int speed *= (this->isLeft() ? SERVOMAX : SERVOMIN);
    this->pwm->setPWM(this->horizontalPin, 0, (this->isLeft() ? SERVOMAX : SERVOMIN));
    delay(10);
    this->lastButtonState = digitalRead(this->buttonPin);
}

void Leg::pushBackward() {
//    if (this->h == back) return;
//    this->h = back;
//    int speed *= (this->isLeft() ? SERVOMIN : SERVOMAX);
    this->pwm->setPWM(this->horizontalPin, 0, (this->isLeft() ? SERVOMIN : SERVOMAX));
    delay(10);
    this->lastButtonState = digitalRead(this->buttonPin);
}

//void Leg::pushDown(int speed) {
//    speed *= (this->isLeft() ? -1 : 1);
//    this->pwm->setPWM(middlePin, 0, 300 + speed);
//}

//void Leg::pushLowerServo(int speed) {
//    this->pwm->setPWM(lowerPin, 0, speed);  // Правая нога
//}

void Leg::stopHorizontal() {
    this->pwm->setPWM(this->horizontalPin, 0, SERVOSTOP);
}

void Leg::stop() {
    this->stopHorizontal();
    this->pwm->setPWM(this->verticalPin, 0, SERVOSTOP);
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

void Hexapod::attachLeg(int horizontalPin, int verticalPin, enum type t) {
//    this->legs[this->count] = Leg();
    this->legs[this->count].attach(t == l ? this->left : this->right, horizontalPin, verticalPin, t);
    this->count++;
}

void Hexapod::moveForward() {
//    for (int i = 0; i < this->count; ++i) {
//        this->legs[i].pushLowerServo(550);
//    }
//
//    for (int i = 0; i < this->count; ++i) {
//        this->legs[i].pushDown(downSpeed[i] + 10);
//    }
//    delay(duration);

    for (int i = 0; i < this->count; ++i){
        this->legs[i].up();
        this->legs[i].forward();
        this->legs[i].down();
//        this->legs[i].pushDown(downSpeed[i] + 10);
    }

    int allMoving = this->count;
    while (allMoving) {
        for (int i = 0; i < this->count; ++i) {
            if (this->legs[i].lastButtonState == HIGH)
                this->legs[i].pushBackward();
            else {
                this->legs[i].stopHorizontal();
                allMoving--;
            }
        }
    }
}

void Hexapod::moveBackward() {
//    for (int i = 0; i < this->count; ++i) {
//        this->legs[i].pushLowerServo(600);
//    }

//    for (int i = 0; i < this->count; ++i) {
//        this->legs[i].pushDown(downSpeed[i] + 10);
//    }

//    delay(duration);

    for (int i = 0; i < this->count; ++i){
        this->legs[i].up();
        this->legs[i].backward();
        this->legs[i].down();
//        this->legs[i].pushDown();
    }

    int allMoving = this->count;
    while (allMoving) {
        for (int i = 0; i < this->count; ++i) {
            if (this->legs[i].lastButtonState == HIGH)
                this->legs[i].pushForward();
            else {
                this->legs[i].stopHorizontal();
                allMoving--;
            }
        }
    }
}

void Hexapod::stop() {
    for (int i = 0; i < this->count; ++i)
        this->legs[i].stop();
}
