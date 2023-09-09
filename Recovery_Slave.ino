/********************************************************/
/* Slave Code for I2C                                   */
/* Master : "zigbee_reveive"                            */
/* Slave : Receive data from MPU9250 and send to Master */
/*                                                      */
/* MPU9250 : Analog 4, 5                                */
/********************************************************/

#include "MPU9250.h"
#include <Wire.h> 

int data[3]; // List of mpu data
MPU9250 mpu; // start mpu9250

void setup() {
  Wire.begin(1); // Slave address                
  Wire.onRequest(requestEvent); // excute requestEvent if request exists
  Serial.begin(9600);

  // mpu setup
  if (!mpu.setup(0x68)) {
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }           
}

void loop() {
  if (mpu.update()) {
    static uint32_t prev_ms = millis();
    // Save data
    data[0] = mpu.getYaw();
    data[1] = mpu.getPitch();
    data[2] = mpu.getRoll();
 }
}

void requestEvent() { // execute when request exists
  // Send data // Request function : wire.read()
  Wire.write(data[0]);   
  Wire.write(data[1]);
  Wire.write(data[2]);

}
