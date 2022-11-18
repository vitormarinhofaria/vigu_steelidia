#include <Arduino.h>
#include "U8glib-HAL.h"

#include "InputeManager.h"
#include "Interface.h"
#include "RpmReader.h"
#include "State.h"
#include "Material.h"

U8GLIB_ST7920_128X64_4X u8g(23, 17, 16); // Pinos do Display

volatile int pinValue = 0;
volatile bool pinState = 1;

#define ENC_CLICK_PIN 35 // Pino do Botão
#define ENC_CLK_PIN 33   // Pino 1 do Encoder
#define ENC_DT_PIN 31    // Pino 2 do Encoder

State g_state;

void setup(void)
{
    pinMode(21, INPUT);
    u8g.init(23, 17, 16);
    u8g.setFont(u8g_font_6x10);

    Serial.begin(9600);

#if defined(ARDUINO)
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
#endif
}

void loop(void)
{
    // atualizar input
    g_state.m_input_manager.update();
    bool btn_clicked = g_state.m_input_manager.get_btn_clicked();
    digitalWrite(13, btn_clicked);

    if (btn_clicked && !g_state.m_btn_hold)
    {
        g_state.m_select_position = InputSelecting_increment(g_state.m_select_position);
        g_state.m_select_position_text = InputSelecting_to_string(g_state.m_select_position);
        switch (g_state.m_select_position)
        {
        case InputSelecting::DIAMETRO:
        {
            g_state.m_input_manager.set_rotary_val(g_state.m_diameter);
            g_state.m_prev_diameter = g_state.m_diameter;
            break;
        }
        case InputSelecting::INCREMENTO:
        {
            g_state.m_input_manager.set_rotary_val(g_state.m_increment);
            g_state.m_prev_increment = g_state.m_increment;
            break;
        }
        case InputSelecting::MATERIAL:
        {
            g_state.m_input_manager.set_rotary_val(g_state.m_material);
            g_state.m_prev_rotary_val = g_state.m_material;
            break;
        }
        default:
            break;
        }

        g_state.m_btn_hold = true;
    }
    switch (g_state.m_select_position)
    {
    case InputSelecting::DIAMETRO:
    {
        auto val = g_state.m_input_manager.get_rotary_val();
        g_state.m_diameter += (val - g_state.m_prev_diameter) * g_state.m_increment;
        g_state.m_prev_diameter = val;
        if (g_state.m_diameter > 350)
        {
            g_state.m_diameter = 350;
            g_state.m_input_manager.set_rotary_val(g_state.m_diameter);
            g_state.m_prev_diameter = g_state.m_diameter;
        }
        else if (g_state.m_diameter < 10)
        {
            g_state.m_diameter = 10;
            g_state.m_input_manager.set_rotary_val(g_state.m_diameter);
            g_state.m_prev_diameter = g_state.m_diameter;
        }
        break;
    }
    case InputSelecting::INCREMENTO:
    {
        auto val = g_state.m_input_manager.get_rotary_val();
        if (val != 0)
        {
            if (val > g_state.m_prev_increment)
            {
                g_state.m_increment *= 10;
            }
            else if (val < g_state.m_prev_increment)
            {
                g_state.m_increment /= 10;
            }
        }
        if (g_state.m_increment <= 0)
        {
            g_state.m_increment = 1;
        }
        if (g_state.m_increment > 100)
        {
            g_state.m_increment = 100;
        }
        g_state.m_prev_increment = val;
        break;
    }
    case InputSelecting::MATERIAL:
    {
        auto val = g_state.m_input_manager.get_rotary_val();
        if (val != 0)
        {
            if (val > g_state.m_prev_rotary_val)
            {
                g_state.m_material = material_inc_indx(g_state.m_material);
            }
            else if (val < g_state.m_prev_rotary_val)
            {
                g_state.m_material = material_dec_indx(g_state.m_material);
            }
        }
        g_state.m_prev_rotary_val = val;
        break;
    }
    default:
        break;
    }

    //
    if (g_state.m_btn_stop.read() && !g_state.m_btn_stop_hold)
    {
        g_state.m_tool_operation = TOOL_OPER_inc(g_state.m_tool_operation);
        g_state.m_btn_stop_hold = true;
        Serial.write("BTN STOP PRESSED");
    }
    MatEntry current_mat = material_get(g_state.m_material);
    int avanco_rpm = 0;
    switch (g_state.m_tool_operation)
    {
    case TOOL_OPER::HSS_DESB:
    {
        avanco_rpm = current_mat.CalculateRpmDesbasteHss(g_state.m_rpm_reader.get(), g_state.m_diameter);
        break;
    }
    case TOOL_OPER::HSS_ACAB:
    {
        avanco_rpm = current_mat.CalculateRpmAcabamentoHss(g_state.m_rpm_reader.get(), g_state.m_diameter);
        break;
    }
    case TOOL_OPER::WIDIA_DESB:
    {
        avanco_rpm = current_mat.CalculateRpmDesbasteWidia(g_state.m_rpm_reader.get(), g_state.m_diameter);
        break;
    }
    case TOOL_OPER::WIDIA_ACAB:
    {
        avanco_rpm = current_mat.CalculateRpmAcabamentoWidia(g_state.m_rpm_reader.get(), g_state.m_diameter);
        break;
    }

    default:
        break;
    }

    // Preparar as variavéis para mostrar no display
    String s_material = String(material_get(g_state.m_material).m_name);
    String s_diametro = String(g_state.m_diameter);
    String s_incremento = String(g_state.m_increment);
    String s_rpm = String(String(g_state.m_rpm_reader.get(), 1) + " RPM");
    String s_avanco_rpm = String(String(avanco_rpm) + " RPM");

    String s_hss_desbaste = String(current_mat.desbaste_hss);
    String s_hss_acabamento = String(current_mat.acabamento_hss);
    String s_widia_desbaste = String(current_mat.desbaste_widia);
    String s_widia_acabamento = String(current_mat.acabamento_widia);

    u8g.firstPage();
    do
    {
        int header_h = 12;
        int select_h = 0;

        g_state.m_interface.drawCenter(u8g);

        u8g.drawLine((WIDTH / 4) * 3, 0, (WIDTH / 4) * 3, header_h);
        u8g.drawLine(0, header_h, WIDTH, header_h);

        switch (g_state.m_select_position)
        {
        case InputSelecting::MATERIAL:
        {
            u8g.drawLine(0, select_h, WIDTH / 2, select_h);
            break;
        }
        case InputSelecting::DIAMETRO:
        {
            u8g.drawLine((WIDTH / 2), select_h, (WIDTH / 4) * 3, select_h);
            break;
        }
        case InputSelecting::INCREMENTO:
        {
            u8g.drawLine((WIDTH / 4) * 3, select_h, WIDTH, select_h);
            break;
        }
        default:
            break;
        }

        u8g.setFont(u8g_font_6x10);
        u8g.drawStr((WIDTH / 2) + 5, 10, s_diametro.c_str());
        u8g.setFont(u8g_font_4x6);
        u8g.drawStr(((WIDTH / 4) * 3) - 8, 11, "mm");

        u8g.setFont(u8g_font_6x10);
        u8g.drawStr(((WIDTH / 4) * 3) + 3, 10, s_incremento.c_str());
        u8g.setFont(u8g_font_4x6);
        u8g.drawStr(((WIDTH / 4) * 4) - 8, 11, "mm");

        u8g.drawStr(0, 10, s_material.c_str());

        { // RPM READING
            u8g.setFont(u8g_font_4x6);
            u8g.drawStr(23, 20, "MOTOR");
            u8g.drawStr(10, 30, s_rpm.c_str());
            u8g.drawLine(0, 32, (WIDTH / 2), 32);
        }
        { // RPM AVANÇO
            u8g.setFont(u8g_font_4x6);
            u8g.drawStr((WIDTH / 2) + 20, 20, "AVANCO");
            u8g.drawStr((WIDTH / 2) + 10, 30, s_avanco_rpm.c_str());
            u8g.drawLine((WIDTH / 2), 32, WIDTH, 32);
        }
        { // HSS
            u8g.setFont(u8g_font_4x6);
            u8g.drawStr(25, 40, "HSS");
            u8g.drawStr(5, 50, "DESB.");
            u8g.drawStr(5, 60, s_hss_desbaste.c_str());
            u8g.drawStr(((WIDTH / 4) * 1) + 5, 50, "ACAB.");
            u8g.drawStr(((WIDTH / 4) * 1) + 5, 60, s_hss_acabamento.c_str());
        }
        { // WIDIA
            u8g.setFont(u8g_font_4x6);
            u8g.drawStr((WIDTH / 2) + 20, 40, "WIDIA");
            u8g.drawStr((WIDTH / 2) + 5, 50, "DESB.");
            u8g.drawStr((WIDTH / 2) + 5, 60, s_widia_desbaste.c_str());
            u8g.drawStr(((WIDTH / 4) * 3) + 5, 50, "ACAB.");
            u8g.drawStr(((WIDTH / 4) * 3) + 5, 60, s_widia_acabamento.c_str());
        }

        { // TOOL OP selection line
            int height_x = HEIGHT - 1;
            int height_y = HEIGHT - 1;
            int w_x = (WIDTH / 4) * (g_state.m_tool_operation);
            int w_y = (WIDTH / 4) * (g_state.m_tool_operation + 1);
            u8g.drawLine(w_x, height_x, w_y, height_y);
        }

        g_state.m_input_manager.update();
    } while (u8g.nextPage());

    g_state.m_input_manager.update();

    if (!btn_clicked)
    {
        g_state.m_btn_hold = false;
    }
    if (!g_state.m_btn_stop.read())
    {
        g_state.m_btn_stop_hold = false;
    }
}
