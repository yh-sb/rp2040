#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "periph/gpio.hpp"
#include "hardware/gpio.h"

using namespace periph;

constexpr auto pins = 30;  // Total number of pins

gpio::gpio(uint8_t pin, enum mode mode, bool state):
    _pin(pin),
    _mode(mode)
{
    assert(_pin < pins);
    
    gpio::mode(_mode, state);
}

gpio::~gpio()
{
    gpio_set_function(_pin, GPIO_FUNC_NULL);
}

void gpio::set(bool state) const
{
    gpio_put(_pin, state);
}

bool gpio::get() const
{
    return gpio_get(_pin);
}

void gpio::toggle() const
{
    gpio_xor_mask(1 << _pin);
}

void gpio::mode(enum mode mode, bool state)
{
    _mode = mode;
    
    gpio_set_dir(_pin, mode == periph::gpio::mode::DO ? GPIO_OUT : GPIO_IN);
    gpio_put(_pin, state);
    gpio_set_function(_pin, GPIO_FUNC_SIO);
}
