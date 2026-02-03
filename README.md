# Autonomous Line-Following Vehicle on FPGA
<img width="400" height="500" alt="image" src="https://github.com/user-attachments/assets/51d028d1-dd7d-4f92-90b4-24d9b0c18a30" />

## Overview
This project implements an autonomous line-following vehicle using the Xilinx Arty FPGA platform and a custom MicroBlaze-based System-on-Chip (SoC). The system integrates multiple sensors and motor control peripherals to enable real-time line tracking, obstacle detection, and autonomous navigation on a taped track.

The project demonstrates FPGA-based system design, hardware/software co-design, embedded programming, and autonomous robotics.

## Hardware Platform
- Xilinx Arty FPGA board  
- Infrared line-following sensors (Pmod)  
- Ultrasonic distance sensor (Pmod)  
- Motor driver (Pmod)  
- DC motors and chassis  

## System Architecture
- Custom MicroBlaze soft processor  
- AXI interconnect for peripheral communication  
- AXI-based IP cores for sensor input and motor control  
- Quad-SPI flash memory for standalone boot  

## Software Stack
- Vivado for hardware design and IP integration  
- Vitis for embedded software development  
- Bare-metal C application for real-time control logic  

## Features
- Real-time line detection using infrared sensors  
- Obstacle detection using ultrasonic sensing  
- Closed-loop motor control for autonomous navigation  
- Custom boot process using Quad-SPI flash and SREC bootloader  
- Fully standalone operation on power-up  

## Boot Process
The system is configured to boot autonomously without host interaction:
1. Hardware bitstream and application stored in Quad-SPI flash  
2. SREC bootloader initializes the MicroBlaze system  
3. Embedded application executes automatically on power-up  

## How It Works
1. Infrared sensors detect the taped path and determine vehicle alignment  
2. Embedded software computes steering adjustments  
3. Motor driver receives control signals via AXI interface  
4. Ultrasonic sensor monitors obstacles and halts the vehicle when needed  

## Project Outcomes
- Reliable line following on a taped track  
- Safe obstacle detection and stopping behavior  
- Demonstrates effective hardware/software co-design on FPGA  

## Skills Demonstrated
- FPGA system design with Vivado  
- MicroBlaze SoC architecture  
- AXI protocol and peripheral integration  
- Embedded C programming in Vitis  
- Autonomous robotic control systems  

