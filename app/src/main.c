/*
 * Copyright (c) The ThingSet Project Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#include <stdio.h>

static float room_temp;
static float humidity;

static const struct device *sensor = DEVICE_DT_GET_ONE(bosch_bme680);

int main(void)
{
    struct sensor_value sval;

    if (!device_is_ready(sensor)) {
        printf("Sensor not ready\n");
        return -ENODEV;
    }

    while (true) {
        sensor_sample_fetch(sensor);

        sensor_channel_get(sensor, SENSOR_CHAN_AMBIENT_TEMP, &sval);
        room_temp = sensor_value_to_float(&sval);

        sensor_channel_get(sensor, SENSOR_CHAN_HUMIDITY, &sval);
        humidity = sensor_value_to_float(&sval);

        printf("room_temp: %f, humidity: %f\n", room_temp, humidity);

        k_sleep(K_MSEC(5000));
    }

    return 0;
}
