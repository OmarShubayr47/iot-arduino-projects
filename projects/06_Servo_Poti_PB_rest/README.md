# 06 - Potentiometer Servo Control with Button Reset

## What it does
Controls a servo motor using a potentiometer for position control.
A push button resets the servo to the center position (90 degrees).
Includes smoothing and threshold filtering for stable movement.

## Hardware
- Arduino Uno
- Potentiometer
- SG90 Servo Motor
- Push Button
- Jumper wires

## Wiring
| Component          | Arduino Pin |
|--------------------|-------------|
| Potentiometer Wiper| A0          |
| Button             | Pin 2       |
| Servo Signal       | Pin 3       |

## Concepts Learned
- Analog input from potentiometer
- INPUT_PULLUP for button without external resistor
- Averaging reads for smooth servo movement
- Threshold filtering to prevent unnecessary updates
- Comparing potentiometer vs joystick as input sources