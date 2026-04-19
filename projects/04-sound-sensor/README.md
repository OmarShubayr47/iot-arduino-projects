# 04 - Sound Sensor Monitor (KY-037)

## What it does
Detects sound levels using the KY-037 microphone sensor.
Reads both analog sound level (0-1023) and digital detection 
trigger. Prints results to Serial Monitor in real time.

## Hardware
- Arduino Uno
- KY-037 Sound Sensor
- Jumper wires

## Wiring
| KY-037 Pin | Arduino Pin |
|------------|-------------|
| VCC        | 5V          |
| GND        | GND         |
| AO         | A0          |
| DO         | Pin 7       |

## Concepts Learned
- Analog vs digital sensor output
- Sensitivity tuning with onboard potentiometer
- Real-time threshold detection