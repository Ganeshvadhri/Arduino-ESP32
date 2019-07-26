#include <Wire.h>                                                                          // Wire library for communicating over I2C
#define BQ34Z100 0xAA                                                                      // This is the I2C address of the BQ34Z100

unsigned int voltage;
void readVoltage()
{
  Wire.beginTransmission(BQ34Z100);
  Wire.write(0x08);
  Wire.endTransmission();

  Wire.requestFrom(BQ34Z100, 1);

  unsigned int low = Wire.read();

  Wire.beginTransmission(BQ34Z100);
  Wire.write(0x09);
  Wire.endTransmission();

  Wire.requestFrom(BQ34Z100, 1);

  unsigned int high = Wire.read();

  unsigned int high1 = high << 8;

  voltage = high1 + low;

}



void setup()
{
  Serial.begin(9600);

  Wire.begin();


}
void loop()
{


 readVoltage();
  Serial.print("Battery Pack Voltage: ");
  Serial.print(voltage);
  Serial.println(" mV");

delay(500);
}
