#pragma once

#include <Arduino.h>

#include "InputeManager.h"
#include "Input.h"
#include "RpmReader.h"
#include "Interface.h"

class State {
public:
    InputManager m_input_manager = InputManager(RotaryEncoder(33, 31), 2, Button(35));
    Interface m_interface;
    RpmReader m_rpm_reader = RpmReader(21, 1);

    InputSelecting m_select_position = InputSelecting::MATERIAL;
    const char* m_select_position_text = InputSelecting_to_string(m_select_position);

    int m_diameter = 10;
    int m_prev_diameter = m_diameter;
    int m_increment = 1;
    int m_prev_increment = m_increment;
    
    bool m_btn_hold = false;

    int m_counter = 0;

    State() = default;
};