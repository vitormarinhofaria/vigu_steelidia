#include "Interface.h"

InputSelecting operator+(InputSelecting &s, int s2)
{
    int en = static_cast<int>(s);
    en += s2;
    if (en >= SELECT_COUNT)
    {
        en = 0;
    }
    return static_cast<InputSelecting>(en);
}
InputSelecting operator++(InputSelecting &s)
{
    return s + 1;
}
InputSelecting operator+=(InputSelecting &s, int i)
{
    return s + i;
}

const char *InputSelecting_to_string(const InputSelecting input_selecting)
{
    switch( input_selecting){
        case InputSelecting::MATERIAL :{
            return "Material";
        }
        case InputSelecting::DIAMETRO : {
            return "Diametro";
        }
        case InputSelecting::INCREMENTO : {
            return "Incremento";
        }
        default:{
            return "ERROR";
        }

    }
}