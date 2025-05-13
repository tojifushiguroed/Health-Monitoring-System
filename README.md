# Health-Monitoring-System
Objectives:
Record real-time temperature, pulse, and steps
Alert users of abnormal temperature
Store data for later use
Provide immediate feedback via LCD and LED

Components:
Arduino Uno
ADXL335 Accelerometer
LCD Display I2C
SD Card Module
Digital Buzzer Module &  2 LED
Breadboard & Jumpers
9V Battery & Connector
DHT11 temparature sensor
Volumetric pulse sensor
ADXL335 accelerometer sensor

Sensor data was collected using an Arduino Uno, serving as the main microcontroller. The number of steps was calculated through an accelerometer, while the pulse was monitored using a PPG-based sensor. All the collected data was saved to an SD card for later review. To provide immediate feedback to the user, alerts were triggered via a buzzer, and information was displayed through both an LED and an LCD screen.

## Flowchart of the project
![Image 10 05 2025 at 21 23 (1)](https://github.com/user-attachments/assets/9cc74345-414d-44db-b959-1bfde73e4bf7)


## Circuit Design from Proteus
![image](https://github.com/user-attachments/assets/06d59b48-8fe8-471b-9ea3-b3963db30da9)



References
1	"Adams, H S. ,(2023,April 26) , Top 10 wearable healthcare devices."
2	Adafruit. (n.d.). Pulse Sensor Amped Getting Started Guide. 
3	DHT11 Sensor Datasheet – Aosong/Mouser.
4	RTC Application Manual – NDK.
5	Arduino SD Library.
6	SD Card with Arduino – Last Minute Engineers.
7	ADXL335 Accelerometer Info.
8	Medevel. (n.d.). 15 Arduino Medical Projects.
