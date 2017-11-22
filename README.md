# version
..* all version upload and debugging by one computer only 
- Twins_v100.ino  Start version for Serial communication between 2 of NodeMCU 1.0
..* Twin Pin  D7==RX   D8==TX
- Twins_v110.ino  Must use with double OLED 0.96 screen for both debug by Serial_OLED
..* OLED Pin  D2==SCL  D3==SDA
..* C-Connected recheck pair
..* D-Connected double-check pair
..* EEPROM 0 to 5 Twin-S start parity
..* EEPROM 6 to 11 Twin-S check Twin parity
- Twins_v120.ino  Start communicate with EEPROM data 
..* EEPROM address 12 is parity D-Connected double check address
..* EEPROM address >12 are zinc data to both of Twin 



![Twin-S](https://github.com/SmazControl/Twin-S/blob/master/DSC_0049.JPG?raw=true)
# Twin-S
Twin-S are two pieces of ESP8266 run as AP and STA together




