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

float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
double arrx[5];
double arry[5];
int trigger = 0;
int pos = 0;
double x = 0;
double y = 0;
int i =0;
int cnt = 0;

static double init_x = 85;
static double init_y = 155;

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
  
  for (i = 0; i < 5; i++){
    arrx[i] = init_x;
    arry[i] = init_y;
  }
  
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
    Serial.println("error");
  }
  myFile = SD.open("rectest2.txt", FILE_WRITE);
  SD.remove("rectest2.txt");

  myFile = SD.open("rectest2.txt", FILE_WRITE);
  myFile.println("Time, magX, magY, magZ");
  myFile.close();
}

void loop() {
  int result;
  myFile = SD.open("rectest2.txt", FILE_WRITE);
  Serial.println(myFile);
  if (myFile) { 
    result = mySensor.magUpdate();
    
    if (result != 0) {
      Serial.println("cannot read mag so call begin again");
      mySensor.beginMag();
      result = mySensor.magUpdate();
    }
    if (result == 0) {
      mX = mySensor.magX();
      mY = mySensor.magY();
      mZ = mySensor.magZ();
      mDirection = mySensor.magHorizDirection();
      myFile.print(String(millis()));
      myFile.print(", ");
      Serial.println("magX: " + String(mX));
      myFile.print(String(mX));
      myFile.print(", ");
      Serial.println("maxY: " + String(mY));
      myFile.print(String(mY));
      myFile.print(", ");
      Serial.println("magZ: " + String(mZ));
      myFile.println(String(mZ));

      if (trigger == 0){
        x = 0;
        y = 0;
        arrx[cnt] = mX;
        arry[cnt] = mY;

        cnt += 1;
        if(cnt == 5){
          cnt = 0; 
        }
    
        for (i = 0; i < 5; i++){
          x += arrx[i]/5;
          y += arry[i]/5;
        }
        /*if 문 안 조건 if(자이로센서의 y,z 방향으로 45, -45 방향 이상으로 꺾였을때 || 고도가 OOm 이상 || 시간이 O초 지났을때 || <가능하면> 장거리 통신으로 키보드의 O 버튼을 누를때 )*/

        if(x-init_x > 45 || x-init_x < -45 || y-init_y > 30 || y-init_y < -30 ){
          trigger = 1;
        }
      }
      else if(trigger == 1){
        for (pos = 90; pos <= 180; pos += 10) { 
          myservo.write(pos);              
          delay(10);                       
        }
        trigger = 2;
      }
      Serial.println(x-init_x);
      Serial.println(y-init_y);
      Serial.print("trigger : ");
      Serial.println(trigger);
    }
    //Serial.println("horizontal direction: " + String(mDirection));
  }
 
  myFile.close();
}
