#include <TinyGPS++.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SoftwareSerial.h>

#define BMP_SCK  (8)
#define BMP_MOSI (9)
#define BMP_CS   (10)
#define BMP_MISO (11)
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
Servo myservo;
TinyGPSPlus gps;
SoftwareSerial Serialgps(4,3);

int trigger = 0;
int pos = 0;
int i;
int cnt = 0;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serialgps.begin(9600);
  
  for (pos = 110; pos >= 90; pos -= 10) { 
    myservo.write(pos);            
    delay(10);                       
  }
  
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1){ 
      delay(10);
    }
   }
  
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                 
  Serial.println("Start Conversation"); 
}

void loop(){ 
  if (Serial.available()){ 
    Wire.requestFrom(1, 3);
    double Yaw = Wire.read(); 
    double Pitch = Wire.read();
    double Roll = Wire.read();
    
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25));
    Serial.println(" m");
    /*
    if (Serialgps.available() > 0){
      int c = Serialgps.read();
      if(gps.encode(c)){
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
        Serial.print("Altitude: ");
        Serial.println(gps.altitude.meters());
      }
    }
    */
  }
  /*
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
  */
  Serial.println(analogRead(A1));
}
