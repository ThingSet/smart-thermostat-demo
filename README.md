# Smart Thermostat demo for ThingSet SDK

This program is used during the presentation at the Zephyr Developer Summit 2023 to demonstrate
the use of the ThingSet node library and the ThingSet SDK for Zephyr.

## Workspace setup

It is assumed that the Zephyr SDK and `west` are already installed on the system.

Run the following commands to pull the sample sources and initialize a west workspace:

    mkdir west-workspace
    cd west-workspace
    git clone https://github.com/ThingSet/smart-thermostat-demo
    cd smart-thermostat-demo
    west init -l .
    west update

## Build the application

Inside the `smart-thermostat-demo` folder run the following command:

    west build -b nrf52840dk_nrf52840 app

## Run the application

Flash the application, e.g. with built-in JLink runner:

    west flash -r jlink

Connect via serial with your favourite terminal program, for example:

    picocom -b 115200 /dev/ttyACM0
