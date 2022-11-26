#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  pinMode(0, OUTPUT);
  //SPI.begin();
  LoRa.setPins(5, 16, 4);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  if (Serial.available() > 0) {
    digitalWrite(0, HIGH);
    LoRa.beginPacket();
    LoRa.print(Serial.readString());
    LoRa.endPacket();
    delay(50);
    digitalWrite(0, LOW);
  }
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    digitalWrite(0, HIGH);
    // received a packet
    Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    delay(50);
    digitalWrite(0, LOW);
  }

}
