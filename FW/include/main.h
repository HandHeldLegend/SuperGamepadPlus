#include "hoja_includes.h"

#define PGPIO_BUTTON_PWRSELECT 14

#define PGPIO_BUTTON_USB_SEL 5
#define PGPIO_BUTTON_USB_EN 6
#define PGPIO_ESP_EN 4

#define PGPIO_SCAN_A    23
#define PGPIO_SCAN_B    20
#define PGPIO_SCAN_C    17
#define PGPIO_SCAN_D    19

#define PGPIO_PUSH_A    16
#define PGPIO_PUSH_B    18
#define PGPIO_PUSH_C    21
#define PGPIO_PUSH_D    22

void _gpio_put_od(uint gpio, bool level);

// Analog L Trigger ADC
//#define PADC_LT 1
// Analog R Trigger ADC
//#define PADC_RT 0

// Analog L Trigger GPIO
//#define PGPIO_LT 27
// Analog R Trigger GPIO
//#define PGPIO_RT 26

//#define PGPIO_RUMBLE_MAIN   25
//#define PGPIO_RUMBLE_BRAKE  24

// SPI ADC CLK pin
//#define PGPIO_SPI_CLK 2
// SPI ADC TX pin
//#define PGPIO_SPI_TX  3
// SPI ADC RX pin
//#define PGPIO_SPI_RX  0

// Left stick ADC Chip Select
//#define PGPIO_LS_CS   1
// Right stick ADC Chip Select
//#define PGPIO_RS_CS   4

//#define PGPIO_IMU0_CS 28
//#define PGPIO_IMU1_CS 29

  