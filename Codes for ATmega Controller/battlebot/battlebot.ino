#include <Wire.h>
#include <Servo.h>

// Pin Definitions
const int in1A = 22;
const int in1B = 24;
const int pwm1 = 2;

const int in2A = 26;
const int in2B = 28;
const int pwm2 = 3;

const int EN1_ENTC = 7;

const int in4A = 34;
const int in4B = 36;
const int pwm4 = 5;

const int in5A = 31;
const int in5B = 33;
const int pwm5 = 6;

class Robot {
public:
  enum class Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STOP
  };

  void init() {
    Serial.begin(9600);

    arm_servo.attach(9);
    arm_servo.write(140);

    digitalWrite(EN1_ENTC, LOW);

    pinMode(in1A, OUTPUT);
    pinMode(in1B, OUTPUT);
    pinMode(pwm1, OUTPUT);
    pinMode(in2A, OUTPUT);
    pinMode(in2B, OUTPUT);
    pinMode(pwm2, OUTPUT);
    pinMode(EN1_ENTC, OUTPUT);
    pinMode(in4A, OUTPUT);
    pinMode(in4B, OUTPUT);
    pinMode(pwm4, OUTPUT);
    pinMode(in5A, OUTPUT);
    pinMode(in5B, OUTPUT);
    pinMode(pwm5, OUTPUT);
  }

  void check() {
    if (Serial.available()) {
      char receivedData = Serial.read();
      switch (receivedData) {
        case 'F':
          move(Direction::FORWARD);
          break;
        case 'B':
          move(Direction::BACKWARD);
          break;
        case 'L':
          move(Direction::LEFT);
          break;
        case 'R':
          move(Direction::RIGHT);
          break;
        case 'X':
          arm_down();
          break;
        case 'U':
          roller_on();
          break;
        case 'I':
          roller_off();
          break;
        case 'S':
          stop_all();
          break;
        default:
          stop_all();
          break;
      }
      delay(200);
    }
  }

private:
  Servo arm_servo;

  // Motor control methods
  void motor_move(int inA, int inB, int pwm, int speed) {
    digitalWrite(inA, HIGH);
    digitalWrite(inB, LOW);
    analogWrite(pwm, speed);
  }

  void motor_stop(int inA, int inB) {
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
  }

  void move(Direction direction) {
    switch (direction) {
      case Direction::FORWARD:
        motor_move(in1A, in1B, pwm1, 255);
        motor_move(in2A, in2B, pwm2, 255);
        break;
      case Direction::BACKWARD:
        motor_move(in1A, in1B, pwm1, 255);
        motor_move(in2A, in2B, pwm2, 255);
        break;
      case Direction::LEFT:
        motor_move(in1A, in1B, pwm1, 180);
        motor_move(in2A, in2B, pwm2, 180);
        break;
      case Direction::RIGHT:
        motor_move(in1A, in1B, pwm1, 180);
        motor_move(in2A, in2B, pwm2, 180);
        break;
      case Direction::STOP:
        motor_stop(in1A, in1B);
        motor_stop(in2A, in2B);
        break;
    }
  }

  // Arm and roller control methods
  void arm_down() {
    for (int i = 140; i >= 87; i--) {
      arm_servo.write(i);
      delay(2);
    }
  }

  void arm_up() {
    for (int i = 87; i <= 140; i++) {
      arm_servo.write(i);
      delay(45);
    }
  }

  void roller_on() {
    digitalWrite(EN1_ENTC, HIGH);
    delay(10);
  }

  void roller_off() {
    digitalWrite(EN1_ENTC, LOW);
    delay(10);
  }

  void stop_all() {
    move(Direction::STOP);
  }
};

Robot battleRobot;

void setup() {
  battleRobot.init();
}

void loop() {
  battleRobot.check();
}
