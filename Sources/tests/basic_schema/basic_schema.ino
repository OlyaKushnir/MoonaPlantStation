/***************************************************
 [ ] AREF
 [ ] GND
 [ ] ~D13/SCK
 [ ] D12/MISO         
 [X] ~D11/MOSI       --> Fan Relay                      
 [X] ~D10/SS         --> Light Relay   
 [ ] ~D9       
 [ ] D8                  
 [ ] D7                   
 [ ] ~D6                             
 [ ] ~D5             --> RTC CLK/SCLK                       
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

CLK/SCLK → D5
DAT/IO → D4
RST/CE → D2

Relays:
Light → D10
Fan → D11



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
15-17 Driver Steper infusion pump 1 
18-20 Driver Steper infusion pump 2 
21    Water level Sensor Drain
22    Relay Drain pump
23    Temperature/Humidity Sensor 

Optinal on ESP32


****************************************************/


