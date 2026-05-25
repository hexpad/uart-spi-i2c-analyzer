#include <Wire.h>
#include <SPI.h>

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Wire.begin(0x50);
  
  // I2C icin tetikleyici fonksiyon
  Wire.onReceive(veriGelince);
  
  pinMode(5, INPUT_PULLUP);
  Serial.println("Sistem hazir.");
}

void veriGelince(int gelenByte) {
  Serial.print("I2C: ");
  while(Wire.available()) {
    Serial.print((char)Wire.read());
  }
  Serial.println();
}

void loop() {
  // UART dinleme
  if (Serial2.available()) {
    Serial.print("UART: ");
    Serial.println(Serial2.readString());
  }

  // SPI dinleme (CS pinini kontrol et)
  if (digitalRead(5) == LOW) {
    Serial.println("SPI: Up");
    delay(500);
  }
}