#include "Input.h"

#include <Arduino.h>

RotaryEncoder::RotaryEncoder(int pin1, int pin2) : m_pin1(pin1), m_pin2(pin2)
{
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);
}

int32_t RotaryEncoder::read()
{
    int current_state = digitalRead(m_pin1);
    int32_t value = 0;
    
    if (current_state != m_prev_state)
        value = (digitalRead(m_pin2) != current_state) ? -1 : 1;
    
    m_prev_state = current_state;
    return value;
}

Button::Button(int pin) : m_pin(pin)
{
    pinMode(pin, INPUT_PULLUP);
}

bool Button::read()
{
    return !digitalRead(m_pin);
}