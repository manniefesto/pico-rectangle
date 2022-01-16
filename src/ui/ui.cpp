#include "ui/ui.h"
#include "ui/ssd1306/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "global.hpp"

namespace UI {
    UIOutput uiOutput = UIOutput::LED;
    bool led = true;
    States states = States();

    void updateState(State *state) {
        if (uiOutput == UIOutput::SSD1306) {
            pico_ssd1306::SSD1306 display = pico_ssd1306::SSD1306(i2c0, 0x3C, pico_ssd1306::Size::W128xH32);
            display.clearAndDrawText(state->oledMessage);
        }

        if (uiOutput == UIOutput::LED) {
            gpio_put(LED_PIN, state->ledMode);
        }
    }

    void initUI() {
        int ret;
        uint8_t rxdata;

        i2c_init(i2c0, 1000000);
        gpio_set_function(8, GPIO_FUNC_I2C);
        gpio_set_function(9, GPIO_FUNC_I2C);
        gpio_pull_up(8);
        gpio_pull_up(9);

        ret = i2c_read_blocking_until(i2c_default, 0x3C, &rxdata, 1, false, make_timeout_time_ms(1000));

        if (ret < 0) {
            uiOutput = UIOutput::LED;
            i2c_deinit(i2c0);
            // gpio_set_function(8, GPIO);
            // gpio_set_function(9, GPIO_FUNC_I2C);
        } else {
            uiOutput = UIOutput::SSD1306;
        } 
    }
}