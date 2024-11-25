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
111111
import paho.mqtt.client as mqtt
import serial
import threading

# MQTT Broker ayarları
mqtt_broker = "broker.hivemq.com"
mqtt_port = 1883
mqtt_topic_publish = "topic/serial1"
mqtt_topic_subscribe = "topic/serial2"

# Seri port ayarları
seri_port = '/dev/serial0'  # Raspberry Pi'nin seri portu
baud_rate = 115200

# Seri portu aç
ser = serial.Serial(seri_port, baud_rate)

# MQTT mesajı geldiğinde çağrılacak fonksiyon
def on_message(client, userdata, msg):
    message = msg.payload
    ser.write(message)  # Seri porta yaz

# MQTT'ye bağlanma
def on_connect(client, userdata, flags, rc):
    client.subscribe(mqtt_topic_subscribe)

# Seri porttan veri okuma
def read_from_serial():
    while True:
        if ser.in_waiting > 0:
            incoming_data = ser.readline()
            mqtt_client.publish(mqtt_topic_publish, incoming_data)

# MQTT Client'ı ayarla
mqtt_client = mqtt.Client()
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message
mqtt_client.connect(mqtt_broker, mqtt_port, 60)

# Seri porttan okuma için ayrı bir thread başlat
thread = threading.Thread(target=read_from_serial)
thread.daemon = True
thread.start()

# MQTT loop başlat
mqtt_client.loop_forever()

******************
import paho.mqtt.client as mqtt
import serial
import threading
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

# MQTT Broker settings
mqtt_broker = "broker.hivemq.com"
mqtt_port = 1883
mqtt_topic_publish = "topic/serial1"
mqtt_topic_subscribe = "topic/serial2"

# Serial port settings
seri_port = '/dev/serial0'  # Adjust this according to your system
baud_rate = 115200

# Open the serial port
ser = serial.Serial(seri_port, baud_rate)

# Function to be called when a message is received from MQTT
def on_message(client, userdata, msg):
    message = msg.payload
    ser.write(message)  # Write to the serial port

# Function to read data from serial and publish to MQTT
def read_from_serial_and_publish(client):
    while True:
        if ser.in_waiting > 0:
            incoming_data = ser.readline().decode('utf-8').rstrip()
            print(f"Data received from serial: {incoming_data}")
            client.publish(mqtt_topic_publish, incoming_data)
            time.sleep(1)

# Set up MQTT client
mqtt_client = mqtt.Client()
mqtt_client.on_connect = lambda client, userdata, flags, rc: client.subscribe(mqtt_topic_subscribe)
mqtt_client.on_message = on_message
mqtt_client.connect(mqtt_broker, mqtt_port, 60)

# Start the MQTT loop in a separate thread
mqtt_thread = threading.Thread(target=mqtt_client.loop_forever)
mqtt_thread.daemon = True
mqtt_thread.start()

# Start the serial reading in a separate thread
serial_thread = threading.Thread(target=read_from_serial_and_publish, args=(mqtt_client,))
serial_thread.daemon = True
serial_thread.start()

try:
    while True:
        time.sleep(10)  # Main thread stays alive
except KeyboardInterrupt:
    print("Program terminated")

# Cleanup
mqtt_client.loop_stop()
mqtt_client.disconnect()
ser.close()

++++++++++++++++++++++++++++++++++++++++++++++++
from paho.mqtt import client as mqtt_client
import Jetson.GPIO as GPIO
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

#print("Internet connection established. Continuing with the program...")

# MQTT Settings
broker_address = "broker.emqx.io"
port = 1883
topic = "raspi/robot"
topic2 = "raspi/robot2"

# Serial port settings (Adjust these settings according to your system)
ser = serial.Serial('/dev/ttyTHS1', 115200, timeout=1)
#ser.flush()

# Function to be called when a message is received from MQTT
def on_message(client, userdata, message):
    msg = str(message.payload.decode("utf-8"))
    print(f"Message received from MQTT: {msg}")
    ser.write(msg.encode('utf-8'))  # Write to serial port
    ser.write(b'\n')
# Read data from serial and publish to MQTT
def read_from_serial_and_publish(client):
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        print(f"Data received from serial: {line}")
        client.publish(topic2, line)

# Set up MQTT client
client = mqtt_client.Client('raspibot')
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
    print("Program sonlandırıldı.")
    client.loop_stop()
    client.disconnect()
    ser.close()
    # Burada programınızın düzgün bir şekilde kapanması için gerekli olan diğer temizlik işlemlerini yapabilirsiniz.
from flask import Flask, render_template, request
import os

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/connect", methods=["POST"])
def connect():
    ssid = request.form["ssid"]
    password = request.form["password"]
    with open("/etc/wpa_supplicant/wpa_supplicant.conf", "a") as f:
        f.write(f'\nnetwork={{\n ssid="{ssid}"\n psk="{password}"\n}}')
    os.system("wpa_cli -i wlan0 reconfigure")
    return "Wi-Fi'ye bağlanılıyor..."

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80)


<form method="POST" action="/connect">
    <label for="ssid">SSID:</label>
    <input type="text" id="ssid" name="ssid" required><br>
    <label for="password">Password:</label>
    <input type="password" id="password" name="password" required><br>
    <button type="submit">Bağlan</button>
</form>

