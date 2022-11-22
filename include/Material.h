#pragma once

enum Tool
{
    HSS,
    WIDIA
};
enum TOOL_OPER
{
    HSS_DESB,
    HSS_ACAB,
    WIDIA_DESB,
    WIDIA_ACAB,

    TOOL_OPER_COUNT,
};
TOOL_OPER TOOL_OPER_inc(const TOOL_OPER op);

struct MatEntry
{
    const char *m_name;
    const int desbaste_hss;
    const int acabamento_hss;
    const int desbaste_widia;
    const int acabamento_widia;

    int CalculateRpmDesbasteHss(float rpm, int diameter) const;
    int CalculateRpmAcabamentoHss(float rpm, int diameter) const;

    int CalculateRpmDesbasteWidia(float rpm, int diameter) const;
    int CalculateRpmAcabamentoWidia(float rpm, int diameter) const;
};

const MatEntry* material_get(size_t index);
int material_inc_indx(int index);
int material_dec_indx(int index);
