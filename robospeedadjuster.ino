
#include <Stepper.h>
#include <Wire.h>
#include "ITG3200.h"
#include <Arduino.h>
#include <math.h>

 
#define GRAV 9.81
#define MIU 0.74
#define WHEEL_DIAMETER 0.04 // 4 cm
 
#define V0 0.5
 
ITG3200 gyro = ITG3200();

 
Stepper myStepper(255, 3, 5);
int MOTOR2_PIN1 = 3;
int MOTOR2_PIN2 = 5;
 
int MOTOR1_PIN1 = 9;
int MOTOR1_PIN2 = 6;
 
char val;
int x=0;
 
void setup()
{
  float   gyroX0;
  float   gyroY0;
  float   gyroZ0;
 
 // gyro = new ITG3200();
  myStepper.setSpeed(200);
 
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);
  Serial.begin(9600);
   Wire.begin();
 
  Serial.println("Initializing I2C devices...");
gyro.init(ITG3200_ADDR_AD0_HIGH);
  
gyro.zeroCalibrate(2500, 2);

  Serial.print("zeroCalibrating..."); 
gyro.readGyro(&gyroX0,&gyroY0,&gyroZ0);
 Serial.println("done.");

Serial.print(gyroY0);
 
}
 
void loop() {
  rulareTraseu();
  float gyroX;
  float gyroY;
  float gyroZ;
  float time = 0;
  float rate;
  float angle;
  float a, v;
 
 gyro.readGyro(&gyroX,&gyroY,&gyroZ); 
 
  Serial.print("X: "); Serial.print(gyroX);   Serial.print(" ");
  Serial.print("Y: "); Serial.print(gyroY);   Serial.print(" ");
  Serial.print("Z: "); Serial.println(gyroZ); Serial.print(" ");
 
  delay(100);
  time += 0.1;
 
  rate = (gyroY - 1) / 14.375;
  angle = angle + (rate * time);
 
  // Set acceleration  
  if (rate > 0)
    a = GRAV * (MIU * cos(angle * PI / 180) + sin(angle * PI / 180));
  else
    a = - GRAV * (sin(angle * PI / 180) - MIU * cos(angle * PI / 180));
 
  // Set velocity (in m/s)
  v = V0 + a * time;
 
  // Set velocity (in RPM)
  float rpm = v * (60 / (PI * WHEEL_DIAMETER));
 
  myStepper.setSpeed(rpm);
 
}
int readDirection(){
  if( Serial.available() ){
    val = Serial.read();
  }
  if( val == 'U'){
    return 1;
  }
  if( val == 'D'){
    return 2;
  }
  if( val == 'L'){
    return 3;
  }
  if( val == 'R'){
    return 4;
  }
  if( val == 'C'){
    return 0;
  }
  //rulareTraseu();
}
 
void rulareTraseu()
{
  while (x==1){
    //x=0;
    masinaFata();
    delay(500);
    //masinaStop();
    x=readDirection();
  }
  while(x==2){
    //x=0;
    masinaSpate();
    delay(500);
   // masinaStop();
    x=readDirection();
  }
  while(x==3){
    //x=0;
    mergestanga();
    delay(100);
    //masinaStop();
    x=readDirection();
  }
  while(x==4){
    x=0;
    mergedreapta();
    delay(100);
    masinaStop();
    x=readDirection();
  }
    if(x==0){
    masinaStop();
    delay(100);
    x=readDirection();
  }
}
 
void dreaptafata()
{
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
}
void dreaptaspate()
{
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);
}
void dreaptastop()
{
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, LOW);
}
 
void stangafata()
{
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
}
void stangaspate()
{
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
}
void stangastop()
{
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
}
void mergedreapta()
{
  dreaptastop();
  stangafata();
}
void mergestanga()
{
  stangastop();
  dreaptafata();
}
void vireazadreapta()
{
  dreaptaspate();
  stangafata();
}
void vireazastanga()
{
  stangaspate();
  dreaptafata();
}
void masinaStop()
{
  dreaptastop();
  stangastop();
}
void masinaFata()
{
  dreaptafata();
  stangafata();
}
void masinaSpate()
{
  stangaspate();
  dreaptaspate();
}
