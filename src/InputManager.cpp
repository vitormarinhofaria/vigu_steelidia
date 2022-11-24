#include "InputeManager.h"
#include "Input.h"

InputManager::InputManager(Button btn_click, Button btn_plus, Button btn_minus)
    : m_btn_click(btn_click), m_btn_plus(btn_plus), m_btn_minus(btn_minus)
{
}

bool InputManager::get_btn_clicked()
{
    return m_btn_clicked;
}

static void read_btn_states(Button &btn, bool *clicked, bool *hold)
{
    bool btn_on = btn.read();
    if (btn_on && *clicked)
    {
        *hold = true;
    }
    else if (btn_on && !(*clicked))
    {
        *clicked = true;
    }
    else if (!btn_on)
    {
        *hold = false;
        *clicked = false;
    }
}

void InputManager::update()
{
    m_btn_clicked = m_btn_click.read();
    read_btn_states(m_btn_minus, &m_btn_minus_clicked, &m_btn_minus_hold);
    read_btn_states(m_btn_plus, &m_btn_plus_clicked, &m_btn_plus_hold);
}

bool InputManager::get_btn_plus()
{
    return m_btn_plus_clicked;
}
bool InputManager::get_btn_plus_click_only(){
    return m_btn_plus_clicked && !m_btn_plus_hold;
}
bool InputManager::get_btn_plus_hold(){
    return m_btn_plus_hold;
}
bool InputManager::get_btn_minus()
{
    return m_btn_minus_clicked;
}
bool InputManager::get_btn_minus_click_only(){
    return m_btn_minus_clicked && !m_btn_minus_hold;
}
bool InputManager::get_btn_minus_hold(){
    return m_btn_minus_hold;
}