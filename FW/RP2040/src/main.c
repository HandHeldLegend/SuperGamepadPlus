#include "hoja_includes.h"
#include "main.h"

void _gpio_put_od(uint gpio, bool level)
{
    if(level)
    {
        gpio_set_dir(gpio, GPIO_IN);
        gpio_pull_up(gpio);
        gpio_put(gpio, 1);
    }
    else
    {
        gpio_set_dir(gpio, GPIO_OUT);
        gpio_disable_pulls(gpio);
        gpio_put(gpio, 0);
    }
}

button_remap_s user_map = {
    .dpad_up = MAPCODE_DUP,
    .dpad_down = MAPCODE_DDOWN,
    .dpad_left = MAPCODE_DLEFT,
    .dpad_right = MAPCODE_DRIGHT,

    .button_a = MAPCODE_B_A,
    .button_b = MAPCODE_B_B,
    .button_x = MAPCODE_B_X,
    .button_y = MAPCODE_B_Y,

    .trigger_l = MAPCODE_T_ZL,
    .trigger_r = MAPCODE_T_ZR,
    .trigger_zl = MAPCODE_T_L,
    .trigger_zr = MAPCODE_T_R,

    .button_plus = MAPCODE_B_PLUS,
    .button_minus = MAPCODE_B_MINUS,
    .button_stick_left = MAPCODE_B_STICKL,
    .button_stick_right = MAPCODE_B_STICKR,
};

void cb_hoja_hardware_setup()
{
    // Set up GPIO for input buttons

    hoja_setup_gpio_push(PGPIO_PUSH_A);
    hoja_setup_gpio_push(PGPIO_PUSH_B);
    hoja_setup_gpio_push(PGPIO_PUSH_C);
    hoja_setup_gpio_push(PGPIO_PUSH_D);

    hoja_setup_gpio_scan(PGPIO_SCAN_A);
    hoja_setup_gpio_scan(PGPIO_SCAN_B);
    hoja_setup_gpio_scan(PGPIO_SCAN_C);

    hoja_setup_gpio_button(PGPIO_BUTTON_PWRSELECT);
}

void cb_hoja_read_buttons(button_data_s *data)
{
    // Keypad version
    gpio_put(PGPIO_SCAN_A, false);
    sleep_us(5);
    data->button_a  = !gpio_get(PGPIO_PUSH_C);

    data->button_b  = !gpio_get(PGPIO_PUSH_D);
    data->button_x  = !gpio_get(PGPIO_PUSH_A);
    data->button_y  = !gpio_get(PGPIO_PUSH_B);
    gpio_put(PGPIO_SCAN_A, true);

    gpio_put(PGPIO_SCAN_B, false);
    sleep_us(5);
    data->dpad_left     = !gpio_get(PGPIO_PUSH_D);
    data->dpad_right    = !gpio_get(PGPIO_PUSH_C);
    data->dpad_down     = !gpio_get(PGPIO_PUSH_B);
    data->dpad_up       = !gpio_get(PGPIO_PUSH_A);
    gpio_put(PGPIO_SCAN_B, true);

    gpio_put(PGPIO_SCAN_C, false);
    sleep_us(5);
    data->trigger_l       = !gpio_get(PGPIO_PUSH_C);
    data->trigger_r       = !gpio_get(PGPIO_PUSH_B);
    data->button_plus     = !gpio_get(PGPIO_PUSH_A);
    //
    gpio_put(PGPIO_SCAN_C, true);

    data->button_minus      = !gpio_get(PGPIO_BUTTON_PWRSELECT);
}

void cb_hoja_read_analog(a_data_s *data)
{
    // Convert data
    data->lx = 128<<4;
    data->ly = 128<<4;
    data->rx = 128<<4;
    data->ry = 128<<4;
}

void cb_hoja_task_1_hook(uint32_t timestamp)
{
    //app_rumble_task(timestamp);
}

int main()
{
    stdio_init_all();
    sleep_ms(100);

    printf("SuperGamepad+ Started.\n");

    cb_hoja_hardware_setup();

    gpio_init(PGPIO_ESP_EN);
    _gpio_put_od(PGPIO_ESP_EN, false);

    gpio_init(PGPIO_BUTTON_USB_EN);
    gpio_set_dir(PGPIO_BUTTON_USB_EN, GPIO_OUT);
    gpio_put(PGPIO_BUTTON_USB_EN, 0);

    gpio_init(PGPIO_BUTTON_USB_SEL);
    gpio_set_dir(PGPIO_BUTTON_USB_SEL, GPIO_OUT);
    gpio_put(PGPIO_BUTTON_USB_SEL, 0);

    button_data_s tmp = {0};
    cb_hoja_read_buttons(&tmp);

    if(tmp.trigger_l)
    {
        reset_usb_boot(0, 0);
    }
    else if (tmp.trigger_r)
    {
        // Release ESP to be controlled externally
        _gpio_put_od(PGPIO_ESP_EN, true);

        sleep_ms(100);
        gpio_put(PGPIO_BUTTON_USB_SEL, 1);
        sleep_ms(100);

        gpio_put(PGPIO_BUTTON_USB_EN, 1);
        sleep_ms(100);
        gpio_put(PGPIO_BUTTON_USB_EN, 0);
    }
    else
    {

        hoja_config_t _config = {
            .input_method   = INPUT_METHOD_BLUETOOTH,
            .input_mode     = INPUT_MODE_SWPRO,
        };

        // Release ESP to be enabled
        _gpio_put_od(PGPIO_ESP_EN, true);
        sleep_ms(100);
        gpio_put(PGPIO_BUTTON_USB_SEL, 1);
        sleep_ms(100);

        gpio_put(PGPIO_BUTTON_USB_EN, 1);
        sleep_ms(100);
        gpio_put(PGPIO_BUTTON_USB_EN, 0);

        sleep_ms(5000);
        hoja_init(&_config);
    }
}
