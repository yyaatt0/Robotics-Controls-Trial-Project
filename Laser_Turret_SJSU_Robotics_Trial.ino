#include <Servo.h>
#include "Wire.h"

const int MPU_address = 0x68; 
int16_t x_accel, y_accel, z_accel;

size_t x_servo_pin = 8, y_servo_pin = 9;
Servo x_servo, y_servo;

int x_pos, y_pos;

char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {
  Serial.begin(9600);

  // Set up the servos
  x_servo.attach(x_servo_pin);
  y_servo.attach(y_servo_pin);

  // Sets up the MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU_address); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  x_pos = 90;
  y_pos = 90;

  x_servo.write(x_pos);
  y_servo.write(y_pos);
}
void loop() {
  Wire.beginTransmission(MPU_address);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_address, 3*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  x_accel = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  y_accel = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  z_accel = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  
  // print out data
  Serial.print("aX = "); Serial.print(convert_int16_to_str(x_accel));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(y_accel));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(z_accel));
  Serial.println();

  // Calculate the amount to move via current accelerometer positional feedback
  int change_x = x_accel / 2500;
  int change_y = y_accel / 2500;

  if(x_accel > 2500  || x_accel < -2500)
  {
    if(x_pos + change_x > 180)
    {
      x_pos = 180;
      x_servo.write(x_pos);
    }
    else if(x_pos + change_x < 0)
    {
      x_pos = 0;
      x_servo.write(x_pos);
    }
    else
    {
      x_pos += change_x;
      x_servo.write(x_pos);
    }
  }

    if(y_accel > 2500  || y_accel < -2500)
  {
    if(y_pos + change_y > 180)
    {
      y_pos = 180;
      y_servo.write(y_pos);
    }
    else if(y_pos + change_y < 0)
    {
      y_pos = 0;
      y_servo.write(y_pos);
    }
    else
    {
      y_pos += change_y;
      y_servo.write(y_pos);
    }
  }

  delay(15);
}












// const int MPU_address = 0x68

// Servo myservo;  // create Servo object to control a servo
// // twelve Servo objects can be created on most boards

// int pos = 0;    // variable to store the servo position

// void setup() {
//   myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
// }

// void loop() {
//   for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//   }
//   for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//   }
// }