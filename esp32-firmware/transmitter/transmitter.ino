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
  // Send via UART
  Serial2.print("Wake");

  // Send via SPI
  digitalWrite(5, LOW);
  SPI.transfer('U');
  SPI.transfer('p');
  digitalWrite(5, HIGH);

  // Send via I2C
  Wire.beginTransmission(0x50);
  Wire.print("Neo");
  Wire.endTransmission();

  delay(5000);
}
