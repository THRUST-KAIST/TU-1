#include "MPU9250.h"
#include <Servo.h>

MPU9250 mpu;
Servo myservo;

int trigger = 0;
int pos = 0;
int i;
double arry[5];
double arrz[5];
double y = 0;
double z = 0;
int cnt = 0;

void setup() {
    myservo.attach(9);
    Serial.begin(115200);
    Wire.begin();
    

    for (i = 0; i < 5; i++){
      arry[i] = 0;
      arrz[i] = 0;
    }
    
    for (pos = 110; pos >= 90; pos -= 10) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(10);                       // waits 15 ms for the servo to reach the position
    }

    if (!mpu.setup(0x68)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }
}

void loop() {
    if (mpu.update()) {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 25) {
            
            
            if(trigger == 0){
              print_roll_pitch_yaw();
              y = 0;
              z = 0;

              arry[cnt] = mpu.getPitch();
              arrz[cnt] = mpu.getRoll();
    
              cnt += 1;
              if(cnt == 5){
                cnt = 0; 
              }
    
              for (i = 0; i < 5; i++){
                y += arry[i]/5;
                z += arrz[i]/5;
              }
  
      /*if 문 안 조건 if(자이로센서의 y,z 방향으로 45, -45 방향 이상으로 꺾였을때 || 고도가 OOm 이상 || 시간이 O초 지났을때 || <가능하면> 장거리 통신으로 키보드의 O 버튼을 누를때 )*/
              if(y > 45 || y < -45 || z > 45 || z < -45 ){
                trigger = 1;
              }
            }
            else if (trigger == 1){
              for (pos = 90; pos <= 180; pos += 10) { 
                myservo.write(pos);              
                delay(10);                       
              }
              trigger = 2;
            }

            
            prev_ms = millis();
        }
    }
    
    
    
}

void print_roll_pitch_yaw() {
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);
}
