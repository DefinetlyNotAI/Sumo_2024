/* 

TODO

1) Put strats 1 2 3 and Attack
2) Make check_Sensors always run, and pauses the strats if needed
3) Run code

*/

// Motor Connections (Both must use PWM pins)
const int Backward_PWM_Right = 3; // Right H-Bridge, Backward Control
const int Forward_PWM_Right = 5; // Right H-Bridge, Forward Control
const int Backward_PWM_Left = 9; // Left H-Bridge, Backward Control
const int Forward_PWM_Left = 10; // Left H-Bridge, Forward Control

// IR Sensor Connections
const int Front_View_IR = 4; // 200cm range
const int Right_View_IR = 8; // 200cm range
const int Left_View_IR = 7; // 200cm range
const int Left_SideView_IR = A0; // 40cm range
const int Right_SideView_IR = A2; // 40cm range
const int Back_View_IR = 2; // 40cm range

// Color Sensor Connections
const int Left_CS = A1; // Direction only based on the blade viewpoint
const int Right_CS = A3; // Direction only based on the blade viewpoint
int Left_CS_value = 0; // Variable to store the value read from the color sensor
int Right_CS_value = 0; // Variable to store the value read from the color sensor

// Potentiometer Connection
const int Pot_Pin = A5; // Analog input pin connected to the potentiometer
int Pot_Value = 0; // Variable to store the value read from the potentiometer

// Wheel calculations (Leave this alone)
float wheelRotationPerMM = 360 / (PI * 49); // Rotations per mm traveled

void setup() {
  // Set motor connections as outputs
  pinMode(Forward_PWM_Right, OUTPUT);
  pinMode(Backward_PWM_Right, OUTPUT);
  pinMode(Forward_PWM_Left, OUTPUT);
  pinMode(Backward_PWM_Left, OUTPUT);

  // Set IR sensor pins as inputs
  pinMode(Front_View_IR, INPUT);
  pinMode(Right_View_IR, INPUT);
  pinMode(Left_View_IR, INPUT);
  pinMode(Left_SideView_IR, INPUT);
  pinMode(Right_SideView_IR, INPUT);
  pinMode(Back_View_IR, INPUT);

  // Set color sensor
  pinMode(Left_CS, INPUT);
  pinMode(Right_CS, INPUT);

  // Set potentiometer
  pinMode(Pot_Pin, INPUT);
  Pot_Value = analogRead(Pot_Pin); // Read the value from the sensor

  // Wait 5 seconds per rules as well as stop motor movements
  stop_Movement()
  delay(5000);  
}

void Strat_1() {
  attack();
}

void Strat_2() {
  // Placeholder
}

void Strat_3() {
  // Placeholder
}

void move_forward(int speed, int distanceInDegrees) {
  float rotationDistance = distanceInDegrees / wheelRotationPerMM; // Distance in mm
  float rotationDuration = rotationDistance / speed;
  int power = 255; // Full power for movement

  analogWrite(Forward_PWM_Right, power);
  analogWrite(Backward_PWM_Right, 0);
  analogWrite(Forward_PWM_Left, power);
  analogWrite(Backward_PWM_Left, 0);
  
  delay(rotationDuration * 1000); // Convert duration from seconds to milliseconds
}

void move_backward(int speed, int distanceInDegrees) {
  float rotationDistance = distanceInDegrees / wheelRotationPerMM; // Distance in mm
  float rotationDuration = rotationDistance / speed;
  int power = 255; // Full power for movement

  analogWrite(Backward_PWM_Right, power);
  analogWrite(Forward_PWM_Right, 0);
  analogWrite(Backward_PWM_Left, power);
  analogWrite(Forward_PWM_Left, 0);
  delay(rotationDuration * 1000); // Convert duration from seconds to milliseconds
}

void rotate_clockwise(int speed, int angle) {
  float rotationDistance = angle / wheelRotationPerMM; // Distance in mm
  float rotationDuration = rotationDistance / speed;
  int power = 255; // Full power for rotation

  analogWrite(Forward_PWM_Right, power);
  analogWrite(Backward_PWM_Right, 0);
  analogWrite(Backward_PWM_Left, power);
  analogWrite(Forward_PWM_Left, 0);
  delay(rotationDuration * 1000); // Convert duration from seconds to milliseconds
}

void rotate_anticlockwise(int speed, int angle) {
  float rotationDistance = angle / wheelRotationPerMM; // Distance in mm
  float rotationDuration = rotationDistance / speed;
  int power = 255; // Full power for rotation

  analogWrite(Backward_PWM_Right, power);
  analogWrite(Forward_PWM_Right, 0);
  analogWrite(Forward_PWM_Left, power);
  analogWrite(Backward_PWM_Left, 0);
  delay(rotationDuration * 1000); // Convert duration from seconds to milliseconds
}

void accelerate_forward(int start_power, int final_power) {
  for (int i = start_power; i <= final_power; i++) {
    analogWrite(Forward_PWM_Right, i);
    analogWrite(Forward_PWM_Left, i);
    delay(20);
  }
}

void decelerate_forward(int start_power, int final_power) {
  for (int i = start_power; i >= final_power; i--) {
    analogWrite(Forward_PWM_Right, i);
    analogWrite(Forward_PWM_Left, i);
    delay(20);
  }
}

void accelerate_backward(int start_power, int final_power) {
  for (int i = start_power; i <= final_power; i++) {
    analogWrite(Backward_PWM_Right, i);
    analogWrite(Backward_PWM_Left, i);
    delay(20);
  }
}

void decelerate_backward(int start_power, int final_power) {
  for (int i = start_power; i >= final_power; i--) {
    analogWrite(Backward_PWM_Right, i);
    analogWrite(Backward_PWM_Left, i);
    delay(20);
  }
}

void check_Sensors() {
  // Read values from each sensor
  int frontSensorValue = digitalRead(Front_View_IR);
  int rightSensorValue = digitalRead(Right_View_IR);
  int leftSensorValue = digitalRead(Left_View_IR);
  int leftSideViewSensorValue = digitalRead(Left_SideView_IR);
  int rightSideViewSensorValue = digitalRead(Right_SideView_IR);
  int backSensorValue = digitalRead(Back_View_IR);

  Left_CS_value = digitalRead(Left_CS);
  Right_CS_value = digitalRead(Right_CS);

  // Check each sensor individually
  if (rightSensorValue != 0) {
    stop_Movement();
    while (frontSensorValue == 0) {
      rotate_clockwise(60, 5);
    }
    attack();
  } else if (leftSensorValue != 0) {
    stop_Movement();
    while (frontSensorValue == 0) {
      rotate_anticlockwise(60, 5);
    }
    attack();
  } else if (leftSideViewSensorValue != 0) {
    stop_Movement();
    while (frontSensorValue == 0) {
      rotate_anticlockwise(100, 5);
    }
    attack();
  } else if (rightSideViewSensorValue != 0) {
    stop_Movement();
    while (frontSensorValue == 0) {
      rotate_clockwise(100, 5);
    }
    attack();
  } else if (backSensorValue != 0) {
    stop_Movement();
    while (frontSensorValue == 0) {
      rotate_clockwise(100, 5);
    }
    attack();
  }

  // Color Sensor
  if (Left_CS_value != 0) {
    stop_Movement();
    rotate_clockwise(40, 30);
    move_forward(40, 360);
  } else if (Right_CS_value != 0) {
    stop_Movement();
    rotate_anticlockwise(40, 30);
    move_forward(40, 360);
  }
}

void stop_Movement() {
  // Stop all motors
  analogWrite(Forward_PWM_Right, 0);
  analogWrite(Backward_PWM_Right, 0);
  analogWrite(Forward_PWM_Left, 0);
  analogWrite(Backward_PWM_Left, 0);
}

void attack() {
  bool stop = false;
  Left_CS_value = digitalRead(Left_CS);
  Right_CS_value = digitalRead(Right_CS);

  while (stop != true) {
    move_forward(255, 1);
    if (Left_CS_value != 0 || Right_CS_value != 0) {
      stop = true;
    }
  }
}

void loop() {
  /* TODO Uncomment after finishing test case
  if (Pot_Value <= 341) { // Approximately 1/3 of 1023
    Strat_1(); // Strategy 1
  } else if (Pot_Value <= 682) { // Approximately 2/3 of 1023
    Strat_2(); // Strategy 2
  } else {
    Strat_3(); // Strategy 3
  }
  */

  // Test case starts here
  check_Sensors(); // Function to check and act based on the sensors

  move_forward(50, 90); // Move forward 90 degrees
  delay(1000);

  move_backward(50, 180); // Move backward 180 degrees
  delay(1000);

  rotate_clockwise(50, 90); // Rotate 90 degrees clockwise
  delay(1000);

  rotate_anticlockwise(50, 90); // Rotate 90 degrees anticlockwise
  delay(1000);

  accelerate_forward(30, 255); // Accelerate forward
  delay(1000);

  decelerate_forward(255, 0); // Decelerate forward
  delay(500);

  accelerate_backward(0, 255); // Accelerate reverse
  delay(1000); 

  decelerate_backward(255, 0); // Decelerate reverse
  delay(500);

  stop_Movement(); // Stop all movement
  delay(500);

  while (true) {
    check_Sensors();
  }
}
