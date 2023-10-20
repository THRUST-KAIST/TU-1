/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMP280 Breakout
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial Serialgps(4,3);

File myFile;

#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp; // I2C



void setup() {
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  Serialgps.begin(9600);

  // Serial.println(F("BMP280 test"));
  // unsigned status;
  // status = bmp.begin(BMP280_ADDRESS);
  // Serial.println("?");
  // if (!status) {
  //   Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
  //                     "try a different address!"));
  //   Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
  //   Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
  //   Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
  //   Serial.print("        ID of 0x60 represents a BME 280.\n");
  //   Serial.print("        ID of 0x61 represents a BME 680.\n");
  //   while (1) delay(10);
  // }

  // /* Default settings from datasheet. */
  // bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
  //                 Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
  //                 Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
  //                 Adafruit_BMP280::FILTER_X16,      /* Filtering. */
  //                 Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  // if (!SD.begin(5)) {
  //   Serial.println("initialization failed!");
  //   while (1);
  // }
  // Serial.println("SD initialization done.");
  // myFile = SD.open("bmpc2.txt", FILE_WRITE);
  // myFile.close();
}

 void loop() {
    // Serial.print(F("Temperature = "));
    // Serial.print(bmp.readTemperature());
    // Serial.println(" *C");

    // Serial.print(F("Pressure = "));
    // Serial.print(bmp.readPressure());
    // Serial.println(" Pa");

    // Serial.print(F("Approx altitude = "));
    // Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    // Serial.println(" m");

    // if (Serialgps.available() > 0){
    //   int c = Serialgps.read();
    //   if(gps.encode(c)){
    //     Serial.print("Latitude: ");
    //     Serial.println(gps.location.lat(), 6);
    //     Serial.print("Longitude: ");
    //     Serial.println(gps.location.lng(), 6);
    //     Serial.print("Altitude: ");
    //     Serial.println(gps.altitude.meters());
    //   }
    // }else{
    //   Serial.println("gps sensor error");
    // }

    if (Serialgps.available() > 0){
      int c = Serialgps.read();
      if(gps.encode(c)){
        Serial.print(gps.time.hour());
        Serial.print(" : ");
        Serial.print(gps.time.minute());
        Serial.print(" : ");
        Serial.println(gps.time.second());
      }
    }


    // myFile = SD.open("bmpc2.txt", FILE_WRITE);
    // if(myFile){
    //   myFile.print(F("Temperature = "));
    //   myFile.print(bmp.readTemperature());
    //   myFile.println(" *C");
    //   myFile.print(F("Pressure = "));
    //   myFile.print(bmp.readPressure());
    //   myFile.println(" Pa");
    //   myFile.print(F("Approx altitude = "));
    //   myFile.print(bmp.readAltitude(1013.25));
    //   myFile.println(" m");
    //   myFile.close();
    // }else{
    //   Serial.println("sd card error");
    // }

    // myFile = SD.open("gps.txt", FILE_WRITE);
    // if(myFile){
    //   int c = Serialgps.read();
    //   myFile.println(c);
    //   myFile.close();
    // }else{
    //   Serial.println("sd card error2");
    // }
    
     Serial.println();
     delay(1000);
}