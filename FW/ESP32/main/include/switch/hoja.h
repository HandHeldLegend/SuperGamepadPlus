#ifndef HOJA_H
#define HOJA_H

#include "hoja_includes.h"

typedef struct
{
    uint8_t device_mac[6];
    uint8_t switch_host_mac[6];
} hoja_settings_s;

extern hoja_settings_s global_loaded_settings;

typedef union
  {
    struct
    {
      uint8_t connection  : 4; // 1 is charging, 0 is not connected
      uint8_t bat_lvl     : 4; // 0-8 battery level
    };
    uint8_t bat_status;
  } switch_battery_status_u;

#endif