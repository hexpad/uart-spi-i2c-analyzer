#include <SPI.h>
#include <Wire.h>

void setup() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Wire.begin(21, 22);
  SPI.begin(18, 19, 23, 5);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
}

void loop() {
  // UART ile gonder
  Serial2.print("Wake");

  // SPI ile gonder
  digitalWrite(5, LOW);
  SPI.transfer('U');
  SPI.transfer('p');
  digitalWrite(5, HIGH);

  // I2C ile gonder
  Wire.beginTransmission(0x50);
  Wire.print("Neo");
  Wire.endTransmission();

  delay(5000);
}