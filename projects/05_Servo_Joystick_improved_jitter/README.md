# 05 - Joystick Servo Control (Jitter Improved)

## What it does
Controls a servo motor using a joystick (HW-504) on the X axis.
Includes dead zone, smoothing, threshold filtering, and 
non-blocking timing using millis() to prevent servo freeze.

## Hardware
- Arduino Uno
- HW-504 Joystick Module
- SG90 Servo Motor
- Jumper wires

## Wiring
| Component    | Arduino Pin |
|--------------|-------------|
| Joystick VRx | A0          |
| Joystick SW  | Pin 2       |
| Servo Signal | Pin 3       |

## Key Concepts Learned
- Analog input mapping to servo angle
- Dead zone implementation
- Averaging multiple ADC readings to reduce noise
- millis() vs delay() — non-blocking timing
- Threshold filtering to eliminate jitter

## Problem Solved
Initial version used delay() inside the reading loop which caused
the servo to freeze after extended use. Fixed by replacing delay()
with millis()-based non-blocking timing.