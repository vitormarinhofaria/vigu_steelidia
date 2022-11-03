#pragma once
#include <Arduino.h>

#include "Input.h"
class InputManager
{
public:
    InputManager(RotaryEncoder encoder, int encoder_increment, Button button);
    int32_t get_rotary_val();
    void set_rotary_val(int32_t value);
    void set_rotary_increment(int value);
    bool get_btn_clicked();
    void update();

private:
    RotaryEncoder m_encoder;
    Button m_button;

    bool m_btn_clicked = false;

    int32_t m_rotary_val = 0;
    int32_t m_rotary_step = 1;
    int32_t m_rotary_step_inc = 0;
    int32_t m_rotary_step_dec = 0;
};