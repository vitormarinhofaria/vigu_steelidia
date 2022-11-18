#include <Arduino.h>
#include "Material.h"

static const MatEntry material_table[] = {
    {
        .m_name = "ACO 1020",
        .desbaste_hss = 25,
        .acabamento_hss = 30,
        .desbaste_widia = 200,
        .acabamento_widia = 300,
    },
    {
        .m_name = "ACO 1045",
        .desbaste_hss = 20,
        .acabamento_hss = 25,
        .desbaste_widia = 120,
        .acabamento_widia = 160,
    },
    {
        .m_name = "ACO EXTRADURO 1060",
        .desbaste_hss = 15,
        .acabamento_hss = 25,
        .desbaste_widia = 40,
        .acabamento_widia = 60
    },
    {
        .m_name = "FERRO FUND. MALEAVEL",
        .desbaste_hss = 20,
        .acabamento_hss = 25,
        .desbaste_widia = 70,
        .acabamento_widia = 85
    },
    {
        .m_name = "FERRO FUND. GRIS",
        .desbaste_hss = 15,
        .acabamento_hss = 20,
        .desbaste_widia = 65,
        .acabamento_widia = 95
    },
    {
        .m_name = "FERRO FUND. DURO",
        .desbaste_hss = 10,
        .acabamento_hss = 15,
        .desbaste_widia = 30,
        .acabamento_widia = 50
    },
    {
        .m_name = "BRONZE",
        .desbaste_hss = 30,
        .acabamento_hss = 40,
        .desbaste_widia = 300,
        .acabamento_widia = 380
    },
    {
        .m_name = "LATAO E COBRE",
        .desbaste_hss = 40,
        .acabamento_hss = 50,
        .desbaste_widia = 350,
        .acabamento_widia = 400
    },
    {
        .m_name = "ALUMINIO",
        .desbaste_hss = 60,
        .acabamento_hss = 90,
        .desbaste_widia = 500,
        .acabamento_widia = 700,
    },
    {
        .m_name = "FIBRA E EBONITE",
        .desbaste_hss = 25,
        .acabamento_hss = 40,
        .desbaste_widia = 120,
        .acabamento_widia = 150
    },
};

MatEntry material_get(int index)
{
    if (index < sizeof(material_table))
    {
        return material_table[index];
    }
    else
    {
        return {
            .m_name = "ERROR"};
    }
}

static constexpr int material_count()
{
    return sizeof(material_table) / sizeof(MatEntry);
}

int material_inc_indx(int index)
{
    if (index < material_count() - 1)
    {
        return index + 1;
    }

    if (index >= material_count())
    {
        return 0;
    }
}
int material_dec_indx(int index)
{
    if (index > 0)
    {
        return index - 1;
    }
    if (index <= 0)
    {
        return material_count() - 1;
    }
}

static constexpr float g_CALC_CONST = 1000;

const int MatEntry::CalculateRpmDesbasteHss(float rpm, int diameter)
{
    return (desbaste_hss * g_CALC_CONST) / (diameter * PI);
}
const int MatEntry::CalculateRpmAcabamentoHss(float rpm, int diameter)
{
    return (acabamento_hss * g_CALC_CONST) / (diameter * PI);
}

const int MatEntry::CalculateRpmDesbasteWidia(float rpm, int diameter)
{
    return (desbaste_widia * g_CALC_CONST) / (diameter * PI);
}
const int MatEntry::CalculateRpmAcabamentoWidia(float rpm, int diameter)
{
    return (acabamento_widia * g_CALC_CONST) / (diameter * PI);
}

TOOL_OPER TOOL_OPER_inc(const TOOL_OPER op)
{
    int en = static_cast<int>(op);
    en++;
    if (en >= TOOL_OPER::TOOL_OPER_COUNT)
    {
        en = 0;
    }
    return static_cast<TOOL_OPER>(en);
}