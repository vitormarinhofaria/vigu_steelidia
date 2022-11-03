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
InputSelecting operator+(InputSelecting& s, int s2){
    int en = static_cast<int>(s);
    en += s2;
    if(en >= SELECT_COUNT){
        en = 0;
    }
    return static_cast<InputSelecting>(en);
}
InputSelecting operator++(InputSelecting&s){
    return  s + 1;
}
InputSelecting operator+=(InputSelecting&s, int i){
    return s + i;
}

class Interface
{

public:
    void drawCenter(U8GLIB &g)
    {
        g.drawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
    }
};