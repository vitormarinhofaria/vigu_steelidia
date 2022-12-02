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
    Serial.write("Initialized\n");
}

void loop(void)
{
    // atualizar input
    g_state.m_input_manager.update();
    bool btn_clicked = g_state.m_input_manager.get_btn_clicked();

    if (btn_clicked && !g_state.m_btn_hold)
    {
        g_state.m_select_position = InputSelecting_increment(g_state.m_select_position);
        g_state.m_select_position_text = InputSelecting_to_string(g_state.m_select_position);
        g_state.m_btn_hold = true;
    }
    switch (g_state.m_select_position)
    {
    case InputSelecting::DIAMETRO:
    {
        if (g_state.m_input_manager.get_btn_plus())
        {
            g_state.m_diameter += g_state.m_increment;
        }
        else if (g_state.m_input_manager.get_btn_minus())
        {
            g_state.m_diameter -= g_state.m_increment;
        }

        if (g_state.m_diameter > 350)
        {
            g_state.m_diameter = 350;
            g_state.m_prev_diameter = g_state.m_diameter;
        }
        else if (g_state.m_diameter < 10)
        {
            g_state.m_diameter = 10;
            g_state.m_prev_diameter = g_state.m_diameter;
        }
        break;
    }
    case InputSelecting::INCREMENTO:
    {
        if (g_state.m_input_manager.get_btn_plus())
        {
            g_state.m_increment *= 10;
        }
        else if (g_state.m_input_manager.get_btn_minus())
        {
            g_state.m_increment /= 10;
        }

        if (g_state.m_increment <= 0)
        {
            g_state.m_increment = 1;
        }
        if (g_state.m_increment > 100)
        {
            g_state.m_increment = 100;
        }

        break;
    }
    case InputSelecting::MATERIAL:
    {
        if (g_state.m_input_manager.get_btn_plus())
        {
            g_state.m_material = material_inc_indx(g_state.m_material);
            g_state.m_current_material = material_get(g_state.m_material);
        }
        else if (g_state.m_input_manager.get_btn_minus())
        {
            g_state.m_material = material_dec_indx(g_state.m_material);
            g_state.m_current_material = material_get(g_state.m_material);
        }
        break;
    }
    default:
        break;
    }

    // Preparar as variavéis para mostrar no display
    String s_material = String(g_state.m_current_material->m_name);
    String s_diametro = String(g_state.m_diameter);
    String s_incremento = String(g_state.m_increment);
    String s_rpm = String(String(g_state.m_rpm_reader.get(), 1) + " RPM");
    String s_avanco_rpm = String(String(g_state.m_rpm_avanco.get()) + " RPM");
    float mm_p_volta = 0.0f;
    if(g_state.m_rpm_reader.get() != 0){
        mm_p_volta = (g_state.m_rpm_avanco.get() * 6) / g_state.m_rpm_reader.get();
    }
    String s_mm_p_volta = String(String(mm_p_volta) + " mm/R");

    String s_hss_desbaste = String(g_state.m_current_material->CalculateRpmDesbasteHss(g_state.m_rpm_reader.get(), g_state.m_diameter));
    String s_hss_acabamento = String(g_state.m_current_material->CalculateRpmAcabamentoHss(g_state.m_rpm_reader.get(), g_state.m_diameter));
    String s_widia_desbaste = String(g_state.m_current_material->CalculateRpmDesbasteWidia(g_state.m_rpm_reader.get(), g_state.m_diameter));
    String s_widia_acabamento = String(g_state.m_current_material->CalculateRpmAcabamentoWidia(g_state.m_rpm_reader.get(), g_state.m_diameter));

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

        {
            int bottom_line = 43;
            // RPM READING
            u8g.setFont(u8g_font_4x6);
            u8g.drawStr(23, 20, "MOTOR");
            u8g.drawStr(10, 30, s_rpm.c_str());
            u8g.drawLine(0, bottom_line, (WIDTH / 2), bottom_line);

            // RPM AVANÇO
            u8g.setFont(u8g_font_4x6);
            u8g.drawStr((WIDTH / 2) + 20, 20, "AVANCO");
            u8g.drawStr((WIDTH / 2) + 10, 30, s_avanco_rpm.c_str());
            u8g.drawStr((WIDTH / 2) + 10, 38, s_mm_p_volta.c_str());
            u8g.drawLine((WIDTH / 2), bottom_line, WIDTH, bottom_line);
        }
        {
            int top_text = 51;
            int middle_text = 57;
            int bottom_text = 64;
            { // HSS
                u8g.setFont(u8g_font_4x6);
                u8g.drawStr(25, top_text, "HSS");
                u8g.drawStr(5, middle_text, "DESB.");
                u8g.drawStr(5, bottom_text, s_hss_desbaste.c_str());
                u8g.drawStr(((WIDTH / 4) * 1) + 5, middle_text, "ACAB.");
                u8g.drawStr(((WIDTH / 4) * 1) + 5, bottom_text, s_hss_acabamento.c_str());
            }
            { // WIDIA
                u8g.setFont(u8g_font_4x6);
                u8g.drawStr((WIDTH / 2) + 20, top_text, "WIDIA");
                u8g.drawStr((WIDTH / 2) + 5, middle_text, "DESB.");
                u8g.drawStr((WIDTH / 2) + 5, bottom_text, s_widia_desbaste.c_str());
                u8g.drawStr(((WIDTH / 4) * 3) + 5, middle_text, "ACAB.");
                u8g.drawStr(((WIDTH / 4) * 3) + 5, bottom_text, s_widia_acabamento.c_str());
            }
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
