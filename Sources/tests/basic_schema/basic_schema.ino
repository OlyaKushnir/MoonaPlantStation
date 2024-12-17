/***************************************************
Arduino Uno 1 (Light/Fan/Drain)

[X] SDA                --> ESP32 GPIO21 (SDA)
[X] SCL                --> ESP32 GPIO22 (SCL)
[ ] AREF
[ ] GND
[] ~D13/SCK          
[] D12/MISO           
[X] ~D11/MOSI          --> Fan Relay
[X] ~D10/SS            --> Light Relay
[X] ~D9                --> Drain Pump
[ ] D8
[ ] D7
[ ] ~D6
[X] ~D5                --> RTC CLK/SCLK
[X] D4                 --> RTC DAT/IO
[ ] ~D3
[X] D2                 --> RTC RST/CE
[ ] D1 TX
[ ] D0 RX

[ ] RESET
[ ] 3.3V
[ ] 5V
[ ] GND
[ ] VIN

[X] A0                 --> Drain Water Level Sensor
[ ] A1
[ ] A2
[ ] A3
[X] A4                 --> SDA (I2C to ESP32)
[X] A5                 --> SCL (I2C to ESP32)

RTC (Real-Time Clock):
CLK/SCLK → D5
DAT/IO → D4
RST/CE → D2

Relays 220V:
Light → D10
Fan → D11

Relays 12V:
Drain Pump → D9

Water Level Sensor:
Drain Water Level Sensor → A0

I2C (Connection to ESP32):
SDA → A4 → ESP32 GPIO21 (SDA)
SCL → A5 → ESP32 GPIO22 (SCL)


***************************************************


****************************************************/
/**********************************
Arduino Uno Watering/Steppers
[X] SDA                --> ESP32 GPIO21 (SDA)
[X] SCL                --> ESP32 GPIO22 (SCL)
[ ] AREF
[ ] GND
[X] ~D13/SCK           --> Stepper1 Direction
[X] D12/MISO           --> Fertilizer2 Direction Valve
[X] ~D11/MOSI          --> Fertilizer1 Direction Valve
[X] ~D10/SS            --> Stepper2 Pulse (Step Pin)
[X] ~D9                --> Fertilizer1 Out Valve
[X] D8                 --> Stepper2 Direction
[X] D7                 --> Water Valve Irrigation
[X] ~D6                --> Stepper1 Pulse (Step Pin)                             
[X] ~D5                --> RTC CLK/SCLK                             
[X] D4                 --> RTC DAT/IO                       
[X] ~D3                --> Water Valve Supply System                                  
[X] D2                 --> RTC RST/CE                                  
[ ] D1 TX                                  
[ ] D0 RX  

[ ] RESET
[ ] 3.3V
[ ] 5V
[ ] GND
[ ] VIN

[X] A0                 --> Water Tank Level Sensor
[X] A1                 --> Fertilizer2 Level Sensor
[X] A2                 --> Fertilizer1 Level Sensor
[ ] A3            
[X] A4                 --> Fertilizer1 Fill Valve
[X] A5                 --> Fertilizer2 Fill Valve


Irrigation (Arduino Uno2):

RTC (Real-Time Clock):
CLK/SCLK → D5
DAT/IO → D4
RST/CE → D2

Valves:
Water valve supply system → D7
Water valve irrigation → D8
Fertilizer1 fill valve → A4
Fertilizer2 fill valve → A5
Fertilizer1 out valve → D3
Fertilizer2 out valve → D5
Fertilizer1 direction valve → D11
Fertilizer2 direction valve → D12

Stepper Motors:
Stepper1 direction → D3
Stepper1 pulse → D5
Stepper2 direction → D9
Stepper2 pulse → D10

Water Level Sensors:
Water tank level sensor → A0
Fertilizer1 level sensor → A2
Fertilizer2 level sensor → A3

I2C (Connection to ESP32):
Arduino Uno2 SDA  → ESP32 GPIO21 (SDA)
Arduino Uno2 SCL  → ESP32 GPIO22 (SCL)

***************************************************/



/*********
 ESP32 DEVKIT V1

 DHT11 
 "+"   - 3.3V
 "out" - GPIO4 (D4)
 "-"   - GND
**********************


 
Devices list and pin count:
1-3. Timer (RTC)                                              (in use on 2 arduino)
4. Relay Fans                                                 (arduino1)
5. Relay Lamps                                                (arduino1)
6. Water Level Sensor Injector 1
7. Water Level Sensor Injector 2
8. Tap Fertilizer (bottom) 1
9. Tap Fertilizer (bottom) 2
10. 2-way Tap Fertilizer 1
11. 2-way Tap Fertilizer 2
12. Water Tap IN
13. Water Tap OUT
14. Water Level Sensor Tank
15-16. Driver Stepper Infusion Pump 1 (Direction, Pulse)
17-18. Driver Stepper Infusion Pump 2 (Direction, Pulse)
19. Water Level Sensor Drain (arduino1)
20. Relay Drain Pump                                          (arduino1)
21. Temperature/Humidity Sensor                               (on ESP)

*********/


