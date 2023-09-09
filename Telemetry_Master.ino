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
SoftwareSerial XBee(2, 3);
Servo myservo;

int trigger = 0;
int pos = 0;
int i;
int cnt = 0;

void setup()
{
  Wire.begin();
  myservo.attach(9);
  XBee.begin(9600);
  Serial.begin(9600);

  for (pos = 110; pos >= 90; pos -= 10) { 
    myservo.write(pos);            
    delay(10);                       
  }
  
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }
  
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                 
  Serial.println("Start Conversation"); 
}

void loop()
{ 

  if (1 == 1)//Serial.available())
  { 
    Wire.requestFrom(1, 3);
    double Yaw = Wire.read(); 
    double Pitch = Wire.read();
    double Roll = Wire.read();
    //XBee.println(Serial.read());
    XBee.print("Yaw : ");
    XBee.println(Yaw);
    XBee.print("Pitch : ");
    XBee.println(Pitch);
    XBee.print("Roll : ");
    XBee.println(Roll);
    if (bmp.takeForcedMeasurement()) {
      XBee.print(F("Temperature = "));
      XBee.print(bmp.readTemperature());
      XBee.println(" *C");

      XBee.print(F("Pressure = "));
      XBee.print(bmp.readPressure());
      XBee.println(" Pa");

      XBee.print(F("Approx altitude = "));
      XBee.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
      XBee.println(" m");
    }
  }

  if (XBee.available())
  { 0
    //Serial.write(XBee.read());
    char input = XBee.read();
    Serial.println("Eject");
    for (pos = 90; pos <= 180; pos += 10) { 
      myservo.write(pos);              
      delay(10);                        
    }

    Serial.println(input);
    
  }
}
