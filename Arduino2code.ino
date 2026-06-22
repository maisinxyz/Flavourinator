#include <Stepper.h>

const int STEPS_PER_REV = 1700; // 2048 = Full revolution for 28BYJ-48, change this depending on how much angles it takes to dispense 1 tsp

// --- stepper motor 2 (Motor 2) ---
Stepper myStepper2(STEPS_PER_REV, 2, 4, 3, 5);

// --- Sstepper motor 3 (Motor 3) ---
Stepper myStepper3(STEPS_PER_REV, 6, 8, 7, 9);

// Communication pins (match Arduino 1)
#define MOTOR_2_PIN A0
#define MOTOR_3_PIN A1

void setup() {
// Set motor speeds
myStepper2.setSpeed(10);
myStepper3.setSpeed(10);

// Set up communication pins as inputs (recieve signals from arduino 1)
pinMode(MOTOR_2_PIN, INPUT);
pinMode(MOTOR_3_PIN, INPUT);
}

void loop() {
// Motor 2
if (digitalRead(MOTOR_2_PIN) == HIGH) {
myStepper2.step(STEPS_PER_REV); // Spin 1 full revolution


// Wait for pulse to finish
while(digitalRead(MOTOR_2_PIN) == HIGH) {
  delay(1);
}


}

// Motor 3
if (digitalRead(MOTOR_3_PIN) == HIGH) {
myStepper3.step(STEPS_PER_REV); // Spin 1 full revolution


// Wait for pulse to finish
while(digitalRead(MOTOR_3_PIN) == HIGH) {
  delay(1);
}

}
}
