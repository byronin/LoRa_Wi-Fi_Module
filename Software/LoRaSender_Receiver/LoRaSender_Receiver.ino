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
import paho.mqtt.client as mqtt
import serial
import time
import socket  # Import socket for internet connection check

# Function to check internet connection
def check_internet_connection(host="8.8.8.8", port=53, timeout=3):
    try:
        socket.setdefaulttimeout(timeout)
        socket.socket(socket.AF_INET, socket.SOCK_STREAM).connect((host, port))
        return True
    except socket.error as ex:
        print("Waiting for internet connection...")
        return False

# Wait for internet connection before proceeding
while not check_internet_connection():
    time.sleep(5)

print("Internet connection established. Continuing with the program...")

# MQTT Settings
broker_address = "broker.hivemq.com"
port = 1883
topic = "raspi/robot"

# Serial port settings (Adjust these settings according to your system)
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
ser.flush()

# Function to be called when a message is received from MQTT
def on_message(client, userdata, message):
    msg = str(message.payload.decode("utf-8"))
    print(f"Message received from MQTT: {msg}")
    ser.write(msg.encode('utf-8'))  # Write to serial port

# Read data from serial and publish to MQTT
def read_from_serial_and_publish(client):
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        print(f"Data received from serial: {line}")
        client.publish(topic, line)

# Set up MQTT client
client = mqtt.Client("Raspi_Robot_Client")
client.on_message = on_message

# Connect to the broker
client.connect(broker_address, port=port)
client.subscribe(topic)

# Start listening for messages asynchronously
client.loop_start()

try:
    while True:
        read_from_serial_and_publish(client)
        time.sleep(1)
except KeyboardInterrupt:
    print("Program terminated")

# Cleanup
client.loop_stop()
client.disconnect()
ser.close()


[Unit]
Description=MQTT to Serial Python Script
After=network.target

[Service]
ExecStart=/usr/bin/python3 /home/ronin/robot/mqtt_serial.py
WorkingDirectory=/home/ronin/robot
StandardOutput=inherit
StandardError=inherit
Restart=always
User=ronin

[Install]
WantedBy=multi-user.target
