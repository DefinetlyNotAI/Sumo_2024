#include <SoftwareSerial.h>
SoftwareSerial BT(2, 3); // RX TX pins

const int sensorPin1 = 2;
const int sensorPin2 = 3;

void setup() {
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  BT.begin(9600);
}

void loop() {
  int val1 = digitalRead(sensorPin1);
  int val2 = digitalRead(sensorPin2);

  if (val1 == HIGH || val2 == HIGH) {
    String output = "IR Sensor Value detected: 1: " + String(val1) + " - 2: " + String(val2);
    Serial.println(output);
    BT.print(output);
  }
  
  if (val1 == HIGH && val2 == HIGH) {
    String output = "Both IR Sensor Value: TRUE";
    Serial.println(output);
    BT.print(output);
  }

  delay(100); // Debounce delay
}
