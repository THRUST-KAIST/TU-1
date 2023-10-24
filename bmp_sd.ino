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

File myFile;

#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp; // I2C
int pasttime = 0;

float temperature;
float pressure;
float altitude;


void setup() {
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
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
  }else{
    Serial.println(F("BMP280 test success"));
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  if (!SD.begin(6)) {
    Serial.println("SD initialization failed!");
    while (1);
  }
  Serial.println("SD initialization done.");
  myFile = SD.open("bmp.txt", FILE_WRITE);
  SD.remove("bmp.txt");

  myFile = SD.open("bmp.txt", FILE_WRITE);
  myFile.println("Time, Temperature, Pressure, Altitude");
  myFile.close();
}

void loop() {
  // int nowtime = millis();
    temperature = bmp.readTemperature();
    pressure = bmp.readPressure();
    altitude = bmp.readAltitude();
    Serial.print(F("Temperature = "));
    Serial.print(String(temperature));
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(String(pressure));
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(String(altitude)); /* Adjusted to local forecast! */
    Serial.println(" m");

    myFile = SD.open("bmp.txt", FILE_WRITE);
    if(myFile){

      myFile.print(String(millis()));
      myFile.print(", ");
      myFile.print((String(temperature)));
      myFile.print(", ");
      myFile.print(String(pressure));
      myFile.print(", ");
      myFile.println(String(altitude));

      myFile.close();
    }else{
      Serial.println("sd card error");
    }

    // Serial.println(nowtime - pasttime);
    // pasttime = nowtime;
    // Serial.println();
    
}