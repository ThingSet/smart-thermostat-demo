/*
 * Copyright (c) The ThingSet Project Contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#include <thingset.h>
#include <thingset/sdk.h>

#include <stdio.h>

#define APP_ID_SENSOR          0x05
#define APP_ID_SENSOR_TEMP     0x50
#define APP_ID_SENSOR_HUMIDITY 0x51

#define APP_ID_CONTROL             0x06
#define APP_ID_CONTROL_HEATER_ON   0x60
#define APP_ID_CONTROL_TARGET_TEMP 0x61

static float room_temp;
static float humidity;
static float target_temp = 22.0F;
static bool heater_on;

static const struct device *sensor = DEVICE_DT_GET_ONE(bosch_bme680);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

THINGSET_ADD_GROUP(TS_ID_ROOT, APP_ID_SENSOR, "Sensor", THINGSET_NO_CALLBACK);

THINGSET_ADD_ITEM_FLOAT(APP_ID_SENSOR, APP_ID_SENSOR_TEMP, "rRoomTemp_degC", &room_temp, 1,
                        THINGSET_ANY_R, TS_SUBSET_LIVE);

THINGSET_ADD_ITEM_FLOAT(APP_ID_SENSOR, APP_ID_SENSOR_HUMIDITY, "rHumidity_pct", &humidity, 1,
                        THINGSET_ANY_R, TS_SUBSET_LIVE);

THINGSET_ADD_GROUP(TS_ID_ROOT, APP_ID_CONTROL, "Control", THINGSET_NO_CALLBACK);

THINGSET_ADD_ITEM_BOOL(APP_ID_CONTROL, APP_ID_CONTROL_HEATER_ON, "rHeaterOn", &heater_on,
                       THINGSET_ANY_R, TS_SUBSET_LIVE);

THINGSET_ADD_ITEM_FLOAT(APP_ID_CONTROL, APP_ID_CONTROL_TARGET_TEMP, "sTargetTemp_degC",
                        &target_temp, 1, THINGSET_ANY_RW, TS_SUBSET_NVM);

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

        k_sleep(K_MSEC(5000));
    }

    return 0;
}
