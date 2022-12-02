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

static void read_btn_states(Button &btn, bool *clicked, bool *hold, bool log = false)
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
    if (log)
    {
        Serial.write("Btn state: ");
        Serial.write(String(btn_on).c_str());
        Serial.write(" hold: ");
        Serial.write(String(*hold).c_str());
        Serial.write(" click: ");
        Serial.write(String(*clicked).c_str());
        Serial.write("\r");
    }
}

void InputManager::update()
{
    m_btn_clicked = m_btn_click.read();
    read_btn_states(m_btn_minus, &m_btn_minus_clicked, &m_btn_minus_hold);
    read_btn_states(m_btn_plus, &m_btn_plus_clicked, &m_btn_plus_hold);
}

#define DELAY_BTN_MS 100

bool InputManager::get_btn_plus()
{
    if(m_btn_plus_clicked){
        delay(DELAY_BTN_MS);
    }
    return m_btn_plus_clicked;
}
bool InputManager::get_btn_plus_click_only()
{
    return m_btn_plus_clicked && !m_btn_plus_hold;
}
bool InputManager::get_btn_plus_hold()
{
    return m_btn_plus_hold;
}
bool InputManager::get_btn_minus()
{
    if(m_btn_minus_clicked){
        delay(DELAY_BTN_MS);
    }
    return m_btn_minus_clicked;
}
bool InputManager::get_btn_minus_click_only()
{
    return m_btn_minus_clicked && !m_btn_minus_hold;
}
bool InputManager::get_btn_minus_hold()
{
    return m_btn_minus_hold;
}