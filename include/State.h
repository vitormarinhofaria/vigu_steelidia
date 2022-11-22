#pragma once

#include <Arduino.h>

#include "InputeManager.h"
#include "Input.h"
#include "RpmReader.h"
#include "Interface.h"
#include "Material.h"

class State
{
public:
    InputManager m_input_manager = InputManager(Button(3), Button(5), Button(7));
    Interface m_interface;
    RpmReader m_rpm_reader = RpmReader(21, 1);

    Button m_btn_stop = Button(41);
    bool m_btn_stop_hold = false;
    TOOL_OPER m_tool_operation = TOOL_OPER::HSS_DESB;

    InputSelecting m_select_position = InputSelecting::MATERIAL;
    const char *m_select_position_text = InputSelecting_to_string(m_select_position);

    int m_prev_rotary_val = 0;

    int m_material = 0;

    int m_diameter = 10;
    int m_prev_diameter = m_diameter;

    int m_increment = 10;
    int m_prev_increment = m_increment;

    bool m_btn_hold = false;

    int m_counter = 0;

    State() = default;
};