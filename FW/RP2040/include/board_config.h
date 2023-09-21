#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// Device stuff
#define HOJA_DEVICE_ID  0xA002
#define HOJA_FW_VERSION 0x0A00
#define HOJA_SETTINGS_VERSION 0xA000

// RGB Stuff
#define HOJA_RGB_PIN 15
#define HOJA_RGB_COUNT 11
#define HOJA_RGBW_EN 0

// GPIO definitions
#define HOJA_SERIAL_PIN 7
#define HOJA_CLOCK_PIN 8
#define HOJA_LATCH_PIN 9

// If we do not have native analog triggers
// set this to zero
#define HOJA_ANALOG_TRIGGERS 0

// Sets the analog light trigger level for SP function
#define HOJA_ANALOG_LIGHT 50

// URL that will display to open a config tool
#define HOJA_WEBUSB_URL     "retroplayground.com"
#define HOJA_MANUFACTURER   "RetroPlayground"
#define HOJA_PRODUCT        "SuperGamepad+"

#define HOJA_CAPABILITY_ANALOG_STICK_L 1
#define HOJA_CAPABILITY_ANALOG_STICK_R 1
#define HOJA_CAPABILITY_ANALOG_TRIGGER_L 0
#define HOJA_CAPABILITY_ANALOG_TRIGGER_R 0

#define HOJA_CAPABILITY_BLUETOOTH 0
#define HOJA_CAPABILITY_RGB 1
#define HOJA_CAPABILITY_GYRO 0

#define HOJA_CAPABILITY_NINTENDO_SERIAL 0
#define HOJA_CAPABILITY_NINTENDO_JOYBUS 1

#define HOJA_CAPABILITY_RUMBLE 0

#define HOJA_RGB_GROUP_RS       {10, 10, 10, 10}
#define HOJA_RGB_GROUP_LS       {10, 10, 10, 10}
#define HOJA_RGB_GROUP_DPAD     {0, 1, 2, 3}
#define HOJA_RGB_GROUP_MINUS    {4}
#define HOJA_RGB_GROUP_CAPTURE  {10}
#define HOJA_RGB_GROUP_HOME     {10}
#define HOJA_RGB_GROUP_PLUS     {5}
#define HOJA_RGB_GROUP_Y        {6}
#define HOJA_RGB_GROUP_X        {8}
#define HOJA_RGB_GROUP_A        {9}
#define HOJA_RGB_GROUP_B        {7}

#endif
