
#include <Servo.h>

// ---------------- SERVO OBJECTS ----------------

Servo motor1;  // Gripper
Servo motor2;  // Base
Servo motor3;  // Elbow
Servo motor4;  // Shoulder


// ---------------- GRIPPER CONFIGURATION ----------------

// Change these values to adjust claw movement
int motor_open = 0;
int motor_close = 15;  // ADJUST THIS ANGLE


// ---------------- MOTOR ANGLES ----------------

int rotate = 0;
int elbow_angle = 180;
int shoulder_angle = 180;


// ---------------- MOTOR LIMITS ----------------

// Base
int base_min = 0;
int base_max = 180;
int base_step = 45;

// Elbow
int elbow_min = 130;
int elbow_max = 180;
int elbow_step = 20;

// Shoulder
int shoulder_min = 160;
int shoulder_max = 180;
int shoulder_step = 5;


// ---------------- JOYSTICK CONFIGURATION ----------------

const int joystick_high = 900;
const int joystick_low = 100;

// Prevent repeated commands while joystick is held
bool grip_ready = true;
bool base_ready = true;
bool elbow_ready = true;
bool shoulder_ready = true;


// ---------------- SERVO MOVEMENT FUNCTION ----------------

void moveServo(Servo &motor, int pin, int angle, int waitTime) {

  motor.attach(pin);

  motor.write(angle);

  delay(waitTime);

  motor.detach();
}


// ---------------- GRIPPER ----------------

void hand_open() {

  Serial.println("GRIPPER: OPENING");

  moveServo(motor1, 5, motor_open, 500);
}


void hand_close() {

  Serial.println("GRIPPER: CLOSING");

  moveServo(motor1, 5, motor_close, 700);
}


// ---------------- BASE ROTATION ----------------

void rotate_motor_increase() {

  rotate += base_step;

  if (rotate > base_max) {
    rotate = base_min;
  }

  rotate = constrain(rotate, base_min, base_max);

  Serial.print("Base Angle: ");
  Serial.println(rotate);

  moveServo(motor2, 3, rotate, 500);
}


void rotate_motor_decrease() {

  rotate -= base_step;

  if (rotate < base_min) {
    rotate = base_max;
  }

  rotate = constrain(rotate, base_min, base_max);

  Serial.print("Base Angle: ");
  Serial.println(rotate);

  moveServo(motor2, 3, rotate, 500);
}


// ---------------- ELBOW ----------------

void elbow_increase() {

  elbow_angle -= elbow_step;

  elbow_angle = constrain(
    elbow_angle,
    elbow_min,
    elbow_max
  );

  Serial.print("Elbow Angle: ");
  Serial.println(elbow_angle);

  moveServo(motor3, 4, elbow_angle, 500);
}


void elbow_decrease() {

  elbow_angle += elbow_step;

  elbow_angle = constrain(
    elbow_angle,
    elbow_min,
    elbow_max
  );

  Serial.print("Elbow Angle: ");
  Serial.println(elbow_angle);

  moveServo(motor3, 4, elbow_angle, 500);
}


// ---------------- SHOULDER ----------------

void shoulder_motor_increase() {

  shoulder_angle -= shoulder_step;

  shoulder_angle = constrain(
    shoulder_angle,
    shoulder_min,
    shoulder_max
  );

  Serial.print("Shoulder Angle: ");
  Serial.println(shoulder_angle);

  moveServo(motor4, 2, shoulder_angle, 500);
}


void shoulder_motor_decrease() {

  shoulder_angle += shoulder_step;

  shoulder_angle = constrain(
    shoulder_angle,
    shoulder_min,
    shoulder_max
  );

  Serial.print("Shoulder Angle: ");
  Serial.println(shoulder_angle);

  moveServo(motor4, 2, shoulder_angle, 500);
}


// ---------------- JOYSTICK PRINT ----------------

void joystickprint() {

  Serial.print("J1 X: ");
  Serial.print(analogRead(A4));

  Serial.print(" | J1 Y: ");
  Serial.print(analogRead(A5));

  Serial.print(" | J2 X: ");
  Serial.print(analogRead(A2));

  Serial.print(" | J2 Y: ");
  Serial.println(analogRead(A3));
}


// ---------------- SETUP ----------------

void setup() {

  Serial.begin(9600);

  Serial.println("ROBOTIC ARM INITIALIZED");
  Serial.println("Joystick Control Ready");
}


// ---------------- MAIN LOOP ----------------

void loop() {

  // Read all joystick axes
  int joystick1x = analogRead(A4);
  int joystick1y = analogRead(A5);

  int joystick2x = analogRead(A2);
  int joystick2y = analogRead(A3);


  // Print readings without blocking for 500 ms
  // Uncomment when debugging:
  // joystickprint();


  // ---------------- GRIPPER ----------------

  if (joystick1x > joystick_high && grip_ready) {

    hand_open();
    grip_ready = false;

  }
  else if (joystick1x < joystick_low && grip_ready) {

    hand_close();
    grip_ready = false;
  }

  // Reset when joystick returns to center
  if (joystick1x > joystick_low &&
      joystick1x < joystick_high) {

    grip_ready = true;
  }


  // ---------------- ELBOW ----------------

  if (joystick2x > joystick_high && elbow_ready) {

    elbow_decrease();
    elbow_ready = false;

  }
  else if (joystick2x < joystick_low && elbow_ready) {

    elbow_increase();
    elbow_ready = false;
  }

  if (joystick2x > joystick_low &&
      joystick2x < joystick_high) {

    elbow_ready = true;
  }


  // ---------------- SHOULDER ----------------

  if (joystick2y > joystick_high && shoulder_ready) {

    shoulder_motor_decrease();
    shoulder_ready = false;

  }
  else if (joystick2y < joystick_low && shoulder_ready) {

    shoulder_motor_increase();
    shoulder_ready = false;
  }

  if (joystick2y > joystick_low &&
      joystick2y < joystick_high) {

    shoulder_ready = true;
  }


  // ---------------- BASE ROTATION ----------------

  if (joystick1y > joystick_high && base_ready) {

    rotate_motor_increase();
    base_ready = false;

  }
  else if (joystick1y < joystick_low && base_ready) {

    rotate_motor_decrease();
    base_ready = false;
  }

  if (joystick1y > joystick_low &&
      joystick1y < joystick_high) {

    base_ready = true;
  }

}
