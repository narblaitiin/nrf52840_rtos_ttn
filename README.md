# Code for 6Sens Project : testing the LoRaWAN application with integration in TTN

## Overview
This application contains example code to allow testing of LoRaWAN Network Application.
This code also allows to test a physical link from the node to the gateway then from the gateway to TTN. The version of Zephyr RTOS used is the version v3.6.0.

The packets sent have the following characteristics:

    - Timer : packets every 30 secondes
    - Random value with the range from min = 0 to max = 100% like values from ADC (sensor/battery to percent of charge/discharge)
    - Random value with the range fron 0 to 100% for humidity 
    - Random value with the range from -25 to +50 degree celsius
    - Number of packets : forever loop
    - Number of nodes : 5  

LoRaWAN Device EUI, Join EUI and Application Key should be entered into the app_lorawan.h file prior to compiling.
The example stores the DevNonce in NVS (Non-volatile Storage) as per LoRaWAN 1.0.4 Specifications.
The version of Zephyr RTOS used is the version v3.6.0.

**Board used** : Original MBDT50Q board, powered by battery/solar panel. (see 6sens_project_nrf52 repository/hardware part, for more information.)

## General Information of Application
You will need to register new devices in your application (with OTAA activation method). Once this is done, retain the TTN Device Address(4 Bytes), the TTN Network Key(16 Bytes) and the TTN Application Key (16 Bytes). You also have to make sure that the activation method is OTAA.

    - After you account was created, you have to create a new application
    - After that, you have to add a new end device on this application. You have to complete the various fields using the available data below in manually mode :
        Frequency Plan                  Europe 863-870 MHz (SF9 for RX2 - Recommended)
        LoRaWAN Version                 MAC V1.0.4
        Regional Parameter Version      RP002 Regional Parameters 1.0.4
        Activation by personalization   OTAA
        Application ID                  give a name
        JoinEUI                         00 00 00 00 00 00 00 00
        DevUI number 1                  random value for 8-byte address
        Device                          random value for 4-byte address
        AppKey                          random value for 16-byte address
        NwkSKey                         random value for 16-byte address
        AppSKey                         random value for 16-byte address

## Building and Running
The following commands clean build folder, build and flash the sample:

**Command to use**
````
west build -t pristine

west build -p always -b mdbt50q_lora_dev applications/nrf52840_rtos_ttn_otaa

west flash --runner jlink