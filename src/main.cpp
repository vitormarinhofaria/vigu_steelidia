#include <Arduino.h>
#include "U8glib-HAL.h"

#include "InputeManager.h"
#include "Interface.h"
#include "RpmReader.h"

U8GLIB_ST7920_128X64_4X u8g(23, 17, 16); // Pinos do Display

volatile int pinValue = 0;
volatile bool pinState = 1;

#define ENC_CLICK_PIN 35 // Pino do Botão
#define ENC_CLK_PIN 33   // Pino 1 do Encoder
#define ENC_DT_PIN 31    // Pino 2 do Encoder

InputManager input = InputManager(RotaryEncoder(33, 31), 2, Button(35));
Interface interface;
RpmReader rpm_reader = RpmReader(21, 1);

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

int diametro = 10;
int prev_diametro = diametro;
int incremento = 1;
int prev_increment_val = incremento;
InputSelecting select_position = InputSelecting::MATERIAL;
bool btn_is_hold = false;
int counter = 0;
String s_selecting = "Material";
void loop(void)
{
    // atualizar input
    input.update();
    bool btn_clicked = input.get_btn_clicked();
    digitalWrite(13, btn_clicked);

    if (btn_clicked && !btn_is_hold)
    {
        select_position = InputSelecting_increment(select_position);
        s_selecting = InputSelecting_to_string(select_position);
        switch (select_position)
        {
        case InputSelecting::DIAMETRO:
        {
            input.set_rotary_val(diametro);
            prev_diametro = diametro;
            break;
        }
        case InputSelecting::INCREMENTO:
        {
            input.set_rotary_val(incremento);
            prev_increment_val = incremento;
            break;
        }
        case InputSelecting::MATERIAL:
        {
            input.set_rotary_val(0);
            break;
        }
        default:
            break;
        }

        btn_is_hold = true;
    }
    switch (select_position)
    {
    case InputSelecting::DIAMETRO:
    {
        auto val = input.get_rotary_val();
        diametro += (val - prev_diametro) * incremento;
        prev_diametro = val;
        if (diametro > 350)
        {
            diametro = 350;
            input.set_rotary_val(diametro);
            prev_diametro = diametro;
        }
        else if (diametro < 10)
        {
            diametro = 10;
            input.set_rotary_val(diametro);
            prev_diametro = diametro;
        }
        break;
    }
    case InputSelecting::INCREMENTO:
    {
        auto val = input.get_rotary_val();
        if (val != 0)
        {
            if (val > prev_increment_val)
            {
                incremento *= 10;
            }
            else if (val < prev_increment_val)
            {
                incremento /= 10;
            }
        }
        if (incremento <= 0)
        {
            incremento = 1;
        }
        if (incremento > 100)
        {
            incremento = 100;
        }
        prev_increment_val = val;
        break;
    }
    case InputSelecting::MATERIAL:
    {
        break;
    }
    default:
        break;
    }

    // Preparar as variavéis para mostrar no display
    String s_material = String("Valor: " + String(input.get_rotary_val()));
    String s_diametro = String(diametro);
    String s_incremento = String(incremento);
    String s_rpm = String(String(rpm_reader.get(), 1) + " RPM");
    u8g.firstPage();
    do
    {

        int header_h = 12;
        int select_h = 0;

        interface.drawCenter(u8g);

        u8g.drawLine((WIDTH / 4) * 3, 0, (WIDTH / 4) * 3, header_h);
        u8g.drawLine(0, header_h, WIDTH, header_h);

        switch (select_position)
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

        u8g.setFont(u8g_font_4x6);
        u8g.drawStr(0, 34, s_rpm.c_str());

        u8g.drawStr(0, HEIGHT - 10, s_selecting.c_str());

        input.update();
    } while (u8g.nextPage());

    input.update();
    counter++;

    if (!btn_clicked)
    {
        btn_is_hold = false;
    }
}
