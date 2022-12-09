# Programmable LoRa Module with ESP8266 Wi-Fi

![](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware/lora0.png)

  There are many lora modules and many boards that contain lora module on the market. Its modules are difficult to use alone, and the boards never fully meet our needs. I designed a programmable lora module so that it would be easy to use with all kinds of boards. The mcu controlling the module is an esp8266 chip. In this way, our module gained Wi-Fi feature. In particular I did a few tests with the Raspberry Pi Pico, which gives the pico board the Wi-Fi feature. You can program the board as you wish with the ESP8266EX method. I just wrote a code that will enable two lora modules to communicate with each other. The rest of the development process is in your hands. Below are two different board designs and test results. I hope this module will help you improve your projects. [Click for pcb production file.](https://www.pcbway.com/project/member/?bmbno=1F77F424-45AC-4F "Click for pcb production file.") Feel free to contact me for any problem.
## Sponsor
[![](https://github.com/byronin/MQTT-DMD/blob/main/Hardware/PCBWay_logo.png)](https://www.pcbway.com/project/shareproject/LoRa_Module_for_Raspberry_Pi_Arduino_ESP8266_Testing_LoRa_under_sea_83c3da00.html)
#### Thanks to PCBWay for handling PCB production.
<a href="https://www.pcbway.com/project/shareproject/LoRa_Module_for_Raspberry_Pi_Arduino_ESP8266_Testing_LoRa_under_sea_83c3da00.html"><img src="https://www.pcbway.com/project/img/images/frompcbway-1220.png" alt="PCB from PCBWay" /></a>

  ## Schematic
[![](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware/Ra-01_Sch.png)](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware/Ra-01_Sch.png)
[![](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware/RFM95_Sch.png)](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware/RFM95_Sch.png)
  

## Hardware & BOM
![PCBs](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware/lora2.jpg "PCBs")
 [![This PCB Desing with Eagle ](https://www.snapeda.com/static/img/eda/eagle.png "PCB Desing with Eagle ")](https://www.autodesk.com/products/eagle/free-download "This PCB Desing with Eagle ") 
  ##### This PCB Desing with Eagle  
  ![PCBs](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware/BOM_map.png "PCBs")

|No| PCB Components  | Package |Piece ||No| PCB Components  | Package |Piece | 
| ------------- | ------------- | ------------- |------------- |---------| ------------- | ------------- |------------- |------------- |
|1| ESP8266EX   | 32-VFQFN | 1 pcs  ||2| xmc flash 250hb0b | Soic-8  | 1 pcs |
|3| nx3225sa-24mhz  | 3225 | 1 pcs ||4| 22pf Cap | 0402 | 2 pcs |
|5| 2450AT45A100E | 2450  | 1 pcs  ||6| 5.6pf Cap | 0402   | 1 pcs |
|7| 12K Resistor | 0402  | 2 pcs ||8| 22nf Cap  | 0402  | 1 pcs  |
|9| 100uf Cap | 1210  | 2 pcs || 10  | 10K Resistor | 0402  | 4 pcs  |
|11| 100nf Cap  | 0402  | 1 pcs ||  12 | 1uf Cap | 0603  | 1 pcs  |
|13| 10uf Cap  | 0805  | 2 pcs  ||14| LED | 0603  | 1 pcs  |
|15| Header 1x4 ML  | 2.54-90  | 1 pcs  ||16| LoRa Module  |   | 1 pcs  |
|17| Button-SMD  | 3X4X2.5MM-4pin  | 2 pcs  | 

## Software & Library

- The project runs via the [Arduino IDE](https://www.arduino.cc/en/software "Arduino IDE"). If it is not installed, install it.
- Install [ESP8266](https://github.com/esp8266/Arduino "ESP8266") to Arduino.
- Install the [LoRa Lib.](https://github.com/sandeepmistry/arduino-LoRa "LoRa Lib.") to Arduino.
- Upload this code to [board](https://github.com/byronin/LoRa_Wi-Fi_Module/tree/main/Software/LoRaSender_Receiver "board").

- You can use it as a LoRa gateway, [check this repository](https://github.com/pulsartronic/LoRaWANGatewaySC "check this repository").


## Test 
[![](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware/lora7.png)](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware)
[![](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware/lora-chart-range.png)](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware)
[![](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware/lora5.jpg)](https://github.com/byronin/LoRa_Wi-Fi_Module/blob/main/Hardware)



