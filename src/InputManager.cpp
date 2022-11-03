#include "InputeManager.h"
#include "Input.h"

InputManager::InputManager(RotaryEncoder encoder, int encoder_increment, Button button) : m_encoder(encoder), m_button(button), m_rotary_step(encoder_increment)
{
}

int32_t InputManager::get_rotary_val()
{
    return m_rotary_val;
}

void InputManager::set_rotary_val(int32_t value)
{
    m_rotary_val = value;
    m_rotary_step_inc = 0;
    m_rotary_step_dec = 0;
}

bool InputManager::get_btn_clicked()
{
    return m_btn_clicked;
}

void InputManager::update()
{
    m_btn_clicked = m_button.read();

    auto enc_val = m_encoder.read();

    if (enc_val > 0)
    {
        m_rotary_step_inc += 1;
    }
    else if (enc_val < 0)
    {
        m_rotary_step_dec += 1;
    }

    if (m_rotary_step_dec == m_rotary_step)
    {
        m_rotary_val -= 1;
        m_rotary_step_dec = 0;
    }
    else if (m_rotary_step_inc == m_rotary_step)
    {
        m_rotary_val += 1;
        m_rotary_step_inc = 0;
    }
}

void InputManager::set_rotary_increment(int value){
    m_rotary_step = value;
}