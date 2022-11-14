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
    RpmReader m_rpm_reader;

    InputSelecting m_select_position = InputSelecting::MATERIAL;

    int m_diameter = 10;
    int m_prev_diameter = m_diameter;
    int m_increment = 1;
    
    bool m_btn_hold = false;

    int m_counter = 0;

    
};