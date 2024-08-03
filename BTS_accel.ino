#include <Ramp.h>

ramp LM_R;  // Left motor R_PWM
ramp LM_L;  // Left motor L_PWM
ramp RM_R;  // Right motor R_PWM
ramp RM_L;  // Right motor L_PWM

// ========== 5 second delay ==========
int b_pin = 13;
bool delayed;

// ========== IR ==========
const int IR_pins[3] = {2, 3, 4};
bool IR[3];

const bool _clear[3] = {false, false, false};
const bool front[3] = {true, false, false};
const bool right[3] = {false, true, false};
const bool left[3] = {false, false, true};
const bool frontRight[3] = {true, true, false};
const bool frontLeft[3] = {true, false, true};

// ========== ML2 ==========
const int ml2_right_p = 7;
int ml2_right;
const int ml2_left_p = 8;
int ml2_left;
const int ml2_back_p = 9;
int ml2_back;

// ========== BTS ==========
int EN = 12;
int R_PWM = 11;
int L_PWM = 10;

int R_PWM2 = 5;
int L_PWM2 = 6;

unsigned long currentMillis;
unsigned long prevMillis;
int stepFlag;

void setup() {
  // Sensors
  for (int i = 0; i < 3; i++) pinMode(IR_pins[i], INPUT);
  pinMode(b_pin, INPUT_PULLUP);
  pinMode(ml2_right_p, INPUT);
  pinMode(ml2_left_p, INPUT);
  pinMode(ml2_back_p, INPUT);

  // BTS
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  Serial.begin(9600);

  Serial.println("End of setup");
}

void loop() {
  currentMillis = millis();

  // Sensor readings
  for (int i = 0; i < 3; i++) IR[i] = digitalRead(IR_pins[i]);
  ml2_right = digitalRead(ml2_right_p);
  ml2_left = digitalRead(ml2_left_p);
  ml2_back = digitalRead(ml2_back_p);

  // 5 second delay
  //while (digitalRead(b_pin)) Serial.println("Waiting for button");
  if (!delayed && currentMillis - prevMillis >= 1000) {
    Serial.println(currentMillis);
    stepFlag = 1;
    prevMillis = currentMillis;
    delayed = true;
  }
  else if (delayed) Loop();
} // End of main loop

int mapSpeed(int _s) {
  int s = map(_s, 0, 100, 0, 255);
  return s;
}

void Loop() {
  while (true) {
    currentMillis = millis();
    
    if (stepFlag == 1) {
      Serial.println("STEP 1");
      LM_R.go(mapSpeed(100), 1000);

      prevMillis = currentMillis;
      stepFlag = 2;
    }
    if (stepFlag == 2 && currentMillis - prevMillis >=  3000) {
      Serial.println("STEP 2");
      LM_R.go(mapSpeed(0), 1000);

      prevMillis = currentMillis;
      stepFlag = 3;
    }
    Serial.print("SPEED: ");
    Serial.print(LM_R.update());
    Serial.println();
    Serial.println(currentMillis);

    if (IR != _clear) {
      if (IR == front) {
        Serial.println("FRONT");
      }
      else if (IR == right) {
        Serial.println("RIGHT");
      }
      else if (IR == left) {
        Serial.println("LEFT");
      }
      else if (IR == frontRight) {
        Serial.println("FRONT RIGHT");
      }
      else if (IR == frontLeft) {
        Serial.println("FRONT LEFT");
      }
    }
    else {
      Serial.println("CLEAR");
    }
    /*
        if (ml2_right && ml2_left) {
          Serial.println("ML2 RIGHT LEFT");
        }
        if (ml2_right) {
          Serial.println("ML2 RIGHT");
        }
        if (ml2_left) {
          Serial.println("ML2 Left");
        }
        if (ml2_back) {
          Serial.println("ML2 Back");
        }*/

  }
}

void Forward(int s) {
  analogWrite(R_PWM, s);
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM2, s);
  analogWrite(L_PWM2, 0);
}

void Backward(int s) {
  analogWrite(R_PWM, 0);
  analogWrite(L_PWM, s);
  analogWrite(R_PWM2, 0);
  analogWrite(L_PWM2, s);
}

void Right(int s) {
  analogWrite(R_PWM, s);
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM2, 0);
  analogWrite(L_PWM2, s);
}

void Left(int s) {
  analogWrite(R_PWM, 0);
  analogWrite(L_PWM, s);
  analogWrite(R_PWM2, s);
  analogWrite(L_PWM2, 0);
}
