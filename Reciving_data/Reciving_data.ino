#include <Servo.h>
#define Signal_Pin 10
Servo motor_1;
const byte sensorPin = A0;
float const val = radians(10);//set point
int sensorValue = 0;
float angle = 0;
const int intial_value = 1035;
float error = 0;
float previous_error = 0;
int Signal = 0;
float k_P = 0;
float k_I = 0;
float k_D = 0;
float now = 0 , PreviousTime = 0 , dt = 0 ; //for dt measurement
float sum_error = 0;
float x, y, z;
boolean check = true;
const int numReadings = 100;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
void setup() {
  Serial.begin(9600);// start serial with computer
  motor_1.attach (Signal_Pin);
  delay(1);
  pinMode(sensorPin, INPUT); // put your setup code here, to run once:
  motor_1.writeMicroseconds (intial_value);
  Serial.flush();
  PreviousTime = millis();
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

}

void loop() {
  if (check)
  {
    while(dt < .03){//const sample time
      now = millis();
    dt = (now - PreviousTime) / 1000 ;
    }
    total = total - readings[readIndex];
    readings[readIndex] = analogRead(sensorPin); // put your main code here, to run repeatedly:
    total = total + readings[readIndex];
    readIndex = readIndex + 1;
    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }
    sensorValue = total / numReadings;
    angle = -0.0002 * pow(sensorValue, 2) + 0.5385 * sensorValue - 197.33;
    angle = radians(angle);
    error = val - angle;
    sum_error += error ;
    x = k_P * error;
    y = k_I * sum_error * dt;
    z = k_D * (error - previous_error) / dt;
    Signal = x + y + z + intial_value ;
    if (int(angle) > 50) {//safity
      check = false;
    }
    motor_1.writeMicroseconds( Signal);
    previous_error = error ;
    PreviousTime = now;

    Serial.print("\t");
    Serial.print(dt);
    Serial.print("\t");
    Serial.print(Signal);
    Serial.print(" \t ");
    Serial.print(millis() / 1000.0);
    Serial.print(" \t");
    /*
      Serial.print(" 180");
      Serial.print(" \t");
      Serial.print(" -180");
      Serial.print(" \t");
    */
    Serial.println(degrees(angle));
    delay(1);
  }
  motor_1.writeMicroseconds(0);
}
