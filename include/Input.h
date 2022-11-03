#pragma once
#include <Arduino.h>

class RotaryEncoder
{
public:
    RotaryEncoder(int pin1, int pin2);
    int32_t read();

private:
    uint8_t m_pin1 = 0;
    uint8_t m_pin2 = 0;
    int m_prev_state = 0;
};

class Button
{
public:
    Button(int pin);
    bool read();

private:
    uint8_t m_pin = 0;
};