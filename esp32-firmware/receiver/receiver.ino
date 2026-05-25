#include <Wire.h>
#include <SPI.h>

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Wire.begin(0x50);
  
  // Trigger function for I2C
  Wire.onReceive(veriGelince);
  
  pinMode(5, INPUT_PULLUP);
  Serial.println("System ready.");
}

void veriGelince(int gelenByte) {
  Serial.print("I2C: ");
  while(Wire.available()) {
    Serial.print((char)Wire.read());
  }
  Serial.println();
}

void loop() {
  // Listening for UART
  if (Serial2.available()) {
    Serial.print("UART: ");
    Serial.println(Serial2.readString());
  }

  // Listening for SPI (Check CS pin)
  if (digitalRead(5) == LOW) {
    Serial.println("SPI: Up");
    delay(500);
  }
}
