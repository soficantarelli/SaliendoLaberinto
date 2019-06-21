#include <AFMotor.h>
AF_DCMotor motor1(1, MOTOR12_64KHZ);
AF_DCMotor motor2(2, MOTOR12_64KHZ);
void setup() {
  Serial.begin(9600);
  motor1.setSpeed(150);
  motor2.setSpeed(130);
}
void loop() {
  Serial.print("tick"); 
  motor1.run(R); 
  motor2.run(FORWARD); 
  delay(1000); 
 
}
  

