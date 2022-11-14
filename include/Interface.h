#pragma once

#include <Arduino.h>
#include <U8glib-HAL.h>

#define WIDTH 128
#define HEIGHT 64

enum InputSelecting
{
    MATERIAL = 0,
    DIAMETRO,
    INCREMENTO,

SELECT_COUNT
};

InputSelecting InputSelecting_increment(const InputSelecting input);
const char* InputSelecting_to_string(const InputSelecting input_selecting);
class Interface
{

public:
    void drawCenter(U8GLIB &g)
    {
        g.drawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
    }
};