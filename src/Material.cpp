#include "Material.h"

namespace
{
    static MatEntry steel_1020 = MatEntry{
        .m_name = "AÇO 1020",
        .desbaste_hss = 25,
        .acabamento_hss = 30,
        .desbaste_widia = 200,
        .acabamento_widia = 300,
    };
    static MatEntry steel_1045 = {
        .m_name = "AÇO 1045",
        .desbaste_hss = 20,
        .acabamento_hss = 25,
        .desbaste_widia = 120,
        .acabamento_widia = 160};

    static MatEntry aluminio = {
        .m_name = "ALUMINIO",
        .desbaste_hss = 60,
        .acabamento_hss = 90,
        .desbaste_widia = 500,
        .acabamento_widia = 700};
};

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
        .m_name = "ALUMINIO",
        .desbaste_hss = 60,
        .acabamento_hss = 90,
        .desbaste_widia = 500,
        .acabamento_widia = 700,
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

static constexpr float g_PI = 3.14;
static constexpr float g_CALC_CONST = 1000;

const int MatEntry::CalculateRpmDesbasteHss(float rpm, int diameter)
{
    return (desbaste_hss * g_CALC_CONST) / (diameter * g_PI);
}
const int MatEntry::CalculateRpmAcabamentoHss(float rpm, int diameter)
{
    return (acabamento_hss * g_CALC_CONST) / (diameter * g_PI);
}

const int MatEntry::CalculateRpmDesbasteWidia(float rpm, int diameter)
{
    return (desbaste_widia * g_CALC_CONST) / (diameter * g_PI);
}
const int MatEntry::CalculateRpmAcabamentoWidia(float rpm, int diameter)
{
    return (acabamento_widia * g_CALC_CONST) / (diameter * g_PI);
}