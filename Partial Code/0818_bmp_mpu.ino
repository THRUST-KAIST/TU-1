#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <MPU9250.h>
#include <SoftwareSerial.h>

#define BMP280_ADDRESS 0x76
#define MPU9250_IMU_ADDRESS 0x68
#define MAGNETIC_DECLINATION 1.63 // To be defined by user
Adafruit_BMP280 bmp; // I2C

MPU9250 mpu;

SoftwareSerial xbeeSerial(2,3); //RX, TX

unsigned long time;
unsigned long lasttime = 0;
unsigned long timeinterval;

void setup() {
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  Wire.begin();
  delay(2000);

  bmpsetup();
  mpusetup();
  //mpucallibrate();
}
int input = 0;
char ch;
void loop() {

    //Serial.print("Timeinterval: ");
    time = millis();
    Serial.println(time-lasttime);
    lasttime = time;
    
    bmpprint();
    if (mpu.update()){mpuprint();}

    // input ++;
    // xbeeSerial.write(input);
    // Serial.print(input);
    // ch = Serial.read();
    // Serial.write(ch);
}

void bmpsetup(){
  Serial.println(F("BMP280 test"));
  unsigned status;
  status = bmp.begin(BMP280_ADDRESS);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void bmpprint(){
  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
  Serial.println(" m");
}

void mpusetup(){
  MPU9250Setting setting;
 
  // Sample rate must be at least 2x DLPF rate
  setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
  setting.gyro_fs_sel = GYRO_FS_SEL::G1000DPS;
  setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
  setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_250HZ;
  setting.gyro_fchoice = 0x03;
  setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_20HZ;
  setting.accel_fchoice = 0x01;
  setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;
 
  mpu.setup(MPU9250_IMU_ADDRESS, setting);
 
  mpu.setMagneticDeclination(MAGNETIC_DECLINATION);
  mpu.selectFilter(QuatFilterSel::MADGWICK);
  mpu.setFilterIterations(15);
 
}

void mpucallibrate(){
 
  Serial.println("Calibration will start in 5sec.");
  Serial.println("Please leave the device still on the flat plane.");
  delay(5000);
 
  Serial.println("Calibrating...");
  mpu.calibrateAccelGyro();
 
  Serial.println("Magnetometer calibration will start in 5sec.");
  Serial.println("Please Wave device in a figure eight until done.");
  delay(5000);
 
  Serial.println("Calibrating...");
  mpu.calibrateMag();
 
  Serial.println("Ready!");
}

void mpuprint(){
  if (mpu.update()){
    Serial.print(mpu.getAccX(), 6);
    Serial.print("\t");
    Serial.print(mpu.getAccY(), 6);
    Serial.print("\t");
    Serial.print(mpu.getAccZ(), 6);
    Serial.print("\t");
    Serial.print(F("Yaw: ")); Serial.print(mpu.getYaw()); 
    Serial.print(F(", Pitch: ")); Serial.print(mpu.getPitch()); 
    Serial.print(F(", Roll: ")); Serial.print(mpu.getRoll()); 
    Serial.print("\n\n");
  }
  
}