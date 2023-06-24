/*
 * Copyright (c) The ThingSet Project Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#include <stdio.h>

static float room_temp;
static float humidity;
static float target_temp = 22.0F;
static bool heater_on;

static const struct device *sensor = DEVICE_DT_GET_ONE(bosch_bme680);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static void run_controller()
{
    if (room_temp < target_temp) {
        heater_on = true;
        gpio_pin_set_dt(&led, 1);
    }
    else {
        heater_on = false;
        gpio_pin_set_dt(&led, 0);
    }
}

int main(void)
{
    struct sensor_value sval;

    if (!device_is_ready(sensor) || !gpio_is_ready_dt(&led)) {
        printf("Sensor or LED not ready\n");
        return -ENODEV;
    }

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

    while (true) {
        sensor_sample_fetch(sensor);

        sensor_channel_get(sensor, SENSOR_CHAN_AMBIENT_TEMP, &sval);
        room_temp = sensor_value_to_float(&sval);

        sensor_channel_get(sensor, SENSOR_CHAN_HUMIDITY, &sval);
        humidity = sensor_value_to_float(&sval);

        run_controller();

        printf("room_temp: %f, humidity: %f, heater_on: %d\n", room_temp, humidity, heater_on);

        k_sleep(K_MSEC(5000));
    }

    return 0;
}
