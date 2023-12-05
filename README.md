# EmbeddedLCDGroup
Embedded assignment, group task. November 2023.

## Groupmembers:
- Pontus Bergström
- Edvin Hansson
- Kazem Hassan
- Yuusuf Abdirashid

## Table of Contents

- [Introduction](#introduction)
- [Components](#components)
- [Setup](#setup)
- [Usage](#usage)
- [Custom Characters](#custom-characters)
- [IoT Bitmap Image](#iot-bitmap-image)
- [Customer Messages](#customer-messages)
- [Debugging](#debugging)

## Introduction

This Arduino project turns an LCD screen into a dynamic billboard that displays custom messages for different customers. The messages are randomly selected and include scrolling text, blinking messages, and even custom characters.

## Components

- Arduino Board
- LCD Screen
- Various Sensors (if any, based on your actual setup)

## Setup

1. Connect the LCD screen to the Arduino board following the provided pinouts.
2. Upload the provided Arduino code (`main.cpp`, `customer.cpp`, `message.cpp`, `clock.cpp`, `lcd.cpp`) to your Arduino board.
3. Make sure to set up any custom characters or IoT bitmap images as defined in the code.
4. It is also possible to utilize the online wokwi-Arduino to run the program, at URL: `https://wokwi.com/projects/382811813035164673`

## Usage

The program runs continuously, displaying messages from different customers with varying effects. The messages change every 19 seconds, creating a dynamic and attention-grabbing billboard.

## Custom Characters

The project uses custom characters for special characters like å, ä, ö, Å, Ä, Ö. These characters are defined in the code to enhance the visual appearance of the messages.

## IoT Bitmap Image

A custom bitmap image is used for IoT advertising. This image is created using predefined character codes and adds a unique visual element to the messages.

## Customer Messages

The billboard features messages from different customers, each with its own set of messages. For example:
- Hederlige Harrys Bilar: Messages related to buying cars from Harry's dealership.
- Farmor Ankas Pajer AB: Messages encouraging customers to buy pies from Grandma Anka.
- Svarte Petters Svartbyggen: Messages promoting construction services by Svarte Petter.
- Långbens Detective Agency: Messages to solve mysteries with Långben.
- IoT Advertising Bureau: Promote your brand with messages from IoT's advertising agency.

## Debugging

The code includes a debugging function to display elapsed seconds, helping in troubleshooting and understanding the program's timing.
