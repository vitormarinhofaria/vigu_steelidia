#include "Interface.h"

InputSelecting InputSelecting_increment(const InputSelecting input)
{
    int en = static_cast<int>(input);
    en++;
    if (en >= SELECT_COUNT)
    {
        en = 0;
    }
    return static_cast<InputSelecting>(en);
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