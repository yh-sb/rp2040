#pragma once

#include <stdint.h>

namespace periph
{
class gpio
{
public:
    enum class mode
    {
        DO, /**< Digital output */
        DI, /**< Digital input */
    };
    
    gpio(uint8_t pin, mode mode, bool state = false);
    ~gpio();
    
    void set(bool state) const;
    bool get() const;
    void toggle() const;
    void mode(mode mode, bool state = false);
    
    enum mode mode() const { return _mode; }
    uint8_t pin() const { return _pin; }
    
private:
    uint8_t _pin;
    enum mode _mode;
};
}
