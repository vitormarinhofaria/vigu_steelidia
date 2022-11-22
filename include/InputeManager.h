#pragma once
#include <Arduino.h>

#include "Input.h"
class InputManager
{
public:
    InputManager(Button btn_click, Button btn_plus, Button btn_minus);
    bool get_btn_clicked();
    bool get_btn_plus();
    bool get_btn_minus();
    void update();

private:
    Button m_btn_click;
    Button m_btn_plus;
    Button m_btn_minus;

    bool m_btn_clicked = false;
    
    bool m_btn_plus_clicked = false;
    bool m_btn_plus_hold = false;

    bool m_btn_minus_clicked = false;
    bool m_btn_minus_hold = false;
};