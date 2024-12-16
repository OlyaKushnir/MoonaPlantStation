/***************************************************
Arduino Uno 1 light/fan
 [ ] AREF
 [ ] GND
 [?] ~D13/SCK         --> free Relay3 220V  
 [?] D12/MISO         --> free Relay4 220V 
 [X] ~D11/MOSI       --> Fan Relay                      
 [X] ~D10/SS         --> Light Relay   
 [ ] ~D9       
 [ ] D8                  
 [ ] D7                   
 [ ] ~D6                             
 [X] ~D5             --> RTC CLK/SCLK                       
 [X] D4              --> RTC DAT/IO                      
 [ ] ~D3                                
 [X] D2              --> RTC RST/CE
 [ ] D1 TX                                 
 [ ] D0 RX  


 [ ] RESET
 [ ] 3.3V
 [ ] 5V
 [ ] GND
 [ ] VIN

 [ ] A0
 [ ] A1
 [ ] A2
 [ ] A3
 [ ] A4
 [ ] A5

 Pin Assignments:
DS1302 RTC (Real-Time Clock):

CLK/SCLK                    → D5
DAT/IO                      → D4
RST/CE                      → D2

Relays:
Light                       → D10
Fan                         → D11

Arduino Uno A4 (SDA) → ESP32 GPIO21 (SDA)
Arduino Uno A5 (SCL) → ESP32 GPIO22 (SCL)


***************************************************
Irrigation (Arduino Uno2):
RTC:
CLK/SCLK → D5
DAT/IO → D4
RST/CE → D2

Valves:
Water valve supply system → D3
Water valve irrigation → D7
Drain pump → D8
Fertilizer1 fill valve → A4
Fertilizer2 fill valve → A5
Fertilizer1 out valve → D9
Fertilizer2 out valve → D10
Fertilizer1 direction valve → D11
Fertilizer2 direction valve → D12

Stepper Motors:
Stepper1 direction → D13
Stepper1 pulse → D6
Stepper2 direction → A0
Stepper2 pulse → A1

Water Level Sensors:
Water tank level sensor → A2
Drain water level sensor → A3
Fertilizer1 level sensor → A4
Fertilizer2 level sensor → A5

I2C (Connection to ESP32):
Arduino Uno (SDA) → ESP32 GPIO21 (SDA)
Arduino Uno (SCL) → ESP32 GPIO22 (SCL)


****************************************************/
/**********************************
Arduino Uno watering/drain (Steppers)
 [X] SDA          --> ESP32 GPIO21 (SDA)
 [X] SCL           --> ESP32 GPIO22 (SCL)
 [ ] AREF
 [ ] GND
 [X] ~D13/SCK          --> Fertilizer2 Out Valve
 [X] D12/MISO          --> Fertilizer2 Direction Valve
 [X] ~D11/MOSI         --> Fertilizer1 Direction Valve
 [X] ~D10/SS           --> Stepper2 Pulse (Step Pin)
 [X] ~D9               --> Stepper2 Direction
 [X] D8                --> Fertilizer1 Out Valve
 [X] D7                --> Water Valve Irrigation
 [X] ~D6               --> Stepper1 Pulse (Step Pin)                             
 [X] ~D5               --> Drain Pump                             
 [ ] D4                --> DAT/IO (RTC)                   
 [X] ~D3               --> Stepper1 Direction                                  
 [X] D2                --> RST/CE (RTC)                                  
 [ ] D1 TX                                  
 [ ] D0 RX  

 [ ] RESET
 [ ] 3.3V
 [ ] 5V
 [ ] GND
 [ ] VIN

 [X] A0                --> Water Tank Level Sensor
 [X] A1                --> Drain Water Level Sensor
 [X] A2                --> Fertilizer1 Level Sensor
 [X] A3                --> Fertilizer2 Level Sensor
 [X] A4                --> Fertilizer1 Fill Valve
 [X] A5                --> Fertilizer2 Fill Valve

Pin Assignments:
A4988 Stepper Driver:
DIR (Direction Pin) → D6
STEP (Step Pin) → D3
ENABLE (optional) → Not Connected
***************************************************/
/*********
 ESP32 DEVKIT V1

 DHT11 
 "+"   - 3.3V
 "out" - GPIO4 (D4)
 "-"   - GND
**********************


 
Devices list and pin count:
1-3   Timer
4     Relay Fans
5     Relay Lamps
6     Water level Sensor Injector 1
7     Water level Sensor Injector 2
8     Tap Fertilizer (bottom) 1 
9     Tap Fertilizer (bottom) 2
10    2-way Tap Fertilizer 1
11    2-way Tap Fertilizer 2
12    Water Tap IN
13    Water Tap OUT
14    Water level Sensor Tank
15-16 Driver Steper infusion pump 1 
17-18 Driver Steper infusion pump 2 
19    Water level Sensor Drain
20    Relay Drain pump
21    Temperature/Humidity Sensor 
*********/


