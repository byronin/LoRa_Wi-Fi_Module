import os
import time
import subprocess
from flask import Flask, render_template, request

app = Flask(__name__)

# İnternet bağlantısını kontrol eden fonksiyon
def check_internet():
    try:
        subprocess.check_output(["ping", "-c", "1", "8.8.8.8"], stderr=subprocess.STDOUT)
        return True
    except subprocess.CalledProcessError:
        return False

# Hotspot'u başlatan fonksiyon
def start_hotspot():
    os.system("sudo systemctl stop hostapd dnsmasq")
    os.system("sudo systemctl start hostapd dnsmasq")
    print("Hotspot başlatıldı. SSID: RaspberryPi, Şifre: 12345678")

# Hotspot'u durdurup Wi-Fi ağına bağlanan fonksiyon
def connect_to_wifi(ssid, password):
    with open("/etc/wpa_supplicant/wpa_supplicant.conf", "a") as wifi_config:
        wifi_config.write(f'\nnetwork={{\n    ssid="{ssid}"\n    psk="{password}"\n}}\n')
    os.system("sudo systemctl stop hostapd dnsmasq")
    os.system("sudo wpa_cli -i wlan0 reconfigure")
    print(f"Wi-Fi ağına bağlanılıyor: {ssid}")

# Flask arayüzü
@app.route("/")
def index():
    return render_template("index.html")

@app.route("/connect", methods=["POST"])
def connect():
    ssid = request.form["ssid"]
    password = request.form["password"]
    connect_to_wifi(ssid, password)
    return "Bağlanılıyor... Raspberry Pi yeniden başlatılıyor."

# Ana işlem
if __name__ == "__main__":
    time.sleep(60)  # Raspi açıldıktan sonra 1 dakika bekle
    if not check_internet():
        print("İnternet bağlantısı bulunamadı. Hotspot başlatılıyor...")
        start_hotspot()
        # Flask uygulamasını başlat
        app.run(host="0.0.0.0", port=80)
    else:
        print("İnternet bağlantısı mevcut.")
