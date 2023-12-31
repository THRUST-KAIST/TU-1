#include <MPU9250_asukiaaa.h>

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

#include <Servo.h>
#include <SPI.h>
#include <SD.h>

MPU9250_asukiaaa mySensor;
Servo myservo;
File myFile;

//MPU9250_asukiaaa mySensor;
int result;
float mDirection, mx, my, mz, ax, ay, az, gx, gy, gz;
float deltat;
float * quatt;

#include <MadgwickAHRS.h>

#include "SensorFusion.h"
SF fusion;

Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;

//float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
double arrx[5];
double arry[5];
int trigger = 0;
int pos = 0;
double x = 0;
double y = 0;
int i =0;
int cnt = 0;

static double init_x = 85;
static double init_y = 110;

// for PT sensor and timer trigger
int ptsensor = 0;
float voltage;
float analoginput;
float bar;
unsigned long launchtime;
int launched = 0;
int deploytime = 8; // need to be changed
int barlimit = 2; //need to be changed
File myptFile;

void setup() {
  myservo.attach(3);
  // Serial.begin(9600);
  
  // for (i = 0; i < 5; i++){
  //   arrx[i] = init_x;
  //   arry[i] = init_y;
  // }
  
  for (pos = 110; pos >= 90; pos -= 10){ // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  
  #ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
  #endif

  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();
  
  if (!SD.begin(6)) {
    // Serial.println("sd card error");
    while(1){}
  }
  // myFile = SD.open("rectest2.txt", FILE_WRITE);
  // SD.remove("rectest2.txt");

  myFile = SD.open("rectest2.txt", FILE_WRITE);
  myFile.println("Time, roll, pitch, yaw");
  myFile.close();

  // myFile = SD.open("ptsensor.txt", FILE_WRITE);
  // SD.remove("ptsensor.txt");

  myFile = SD.open("ptsensor.txt", FILE_WRITE);
  myFile.println("Time, bar");
  myFile.close();

  microsPerReading = 1000000 / 25;
  microsPrevious = micros();

  // wait for mpu to be stablized
  delay(20*1000);

}

void loop() {
  int result;
  int aix, aiy, aiz;
  int gix, giy, giz;
  float roll, pitch, heading, yaw;
  unsigned long microsNow;
  myFile = SD.open("rectest2.txt", FILE_WRITE);
  myptFile = SD.open("ptsensor.txt", FILE_WRITE);
  //Serial.println(myFile);
  //Serial.println(myptFile);
  
  if (myFile && myptFile) { 
    result = mySensor.magUpdate() + mySensor.gyroUpdate() + mySensor.accelUpdate();
    
    if (result != 0) {
      // Serial.println("cannot read mag so call begin again");
      mySensor.beginMag();
      result = mySensor.magUpdate();
      delay(100);
    }
    if (result == 0) {
      ax = mySensor.accelX(); //g
      ay = mySensor.accelY();
      az = mySensor.accelZ();
      
      mx = mySensor.magX();
      my = mySensor.magY();
      mz = mySensor.magZ();

      gx = mySensor.gyroX(); //deg/s
      gy = mySensor.gyroY();
      gz = mySensor.gyroZ();


      deltat = fusion.deltatUpdate();
      //fusion.MahonyUpdate(gx* DEG_TO_RAD, gy* DEG_TO_RAD, gz* DEG_TO_RAD, ax, ay, az, deltat);  //mahony is suggested if there isn't the mag
      fusion.MadgwickUpdate(gx* DEG_TO_RAD, gy* DEG_TO_RAD, gz* DEG_TO_RAD, ax, ay, az, mx, my, mz, deltat);  //else use the magwick

      roll = fusion.getRoll();
      pitch = fusion.getPitch();
      yaw = fusion.getYaw();
      quatt = fusion.getQuat();

      // print the heading, pitch and roll
      // Serial.print("Orientation: ");
      // Serial.print(roll);
      // Serial.print(" ");
      // Serial.print(pitch);
      // Serial.print(" ");
      // Serial.println(yaw);

      myFile.print(String(millis()));
      myFile.print(", ");
      
      myFile.print(String(roll));
      myFile.print(", ");
      myFile.print(String(pitch));
      myFile.print(", ");
      myFile.println(String(yaw));

      analoginput = analogRead(ptsensor);
      voltage = analoginput * 5 / 1023;
      bar = (voltage - 1) * 69 / 4 + 1;
      myptFile.print(String(millis()));
      myptFile.print(", ");
      myptFile.println(String(bar));

      if (trigger == 0){
        // x = 0;
        // y = 0;
        // arrx[cnt] = mX;
        // arry[cnt] = mY;

        // cnt += 1;
        // if(cnt == 5){
        //   cnt = 0; 
        // }
    
        // for (i = 0; i < 5; i++){
        //   x += arrx[i]/5;
        //   y += arry[i]/5;
        // }
        /*if 문 안 조건 if(자이로센서의 y,z 방향으로 45, -45 방향 이상으로 꺾였을때 || 고도가 OOm 이상 || 시간이 O초 지났을때 || <가능하면> 장거리 통신으로 키보드의 O 버튼을 누를때 )*/

        if(pitch > 100 ){
          trigger = 1;
        }

        if (bar > barlimit && launched == 0){ // launch 조건 수정
          launchtime = millis();
          launched = 1;
          myptFile.println("Launched!!!!!!");
          // Serial.println("launched!!!!!!");
        }

        if ((millis() > launchtime + deploytime * 1000) && (launched)){ //launch 후 시간 수정
          trigger = 1;

        }

        // increment previous time, so we keep proper pace
        microsPrevious = microsPrevious + microsPerReading;


      }
      else if(trigger == 1){
        for (pos = 90; pos <= 180; pos += 10) { 
          myservo.write(pos);              
          delay(10);                       
        }
        trigger = 2;
      }
      //Serial.println(x-init_x);
      //Serial.println(y-init_y);
      //Serial.println(analoginput);
      // Serial.println(voltage);
      // Serial.println(bar);
      // Serial.print("trigger : ");
      // Serial.println(trigger);
    }
    //Serial.println("horizontal direction: " + String(mDirection));
  }
  delay(100);
 
  myFile.close();
  myptFile.close();
}
