#pragma once

enum Tool{
    HSS,
    WIDIA
};

struct MatEntry{
    const char* m_name;
    const int desbaste_hss;
    const int acabamento_hss;
    const int desbaste_widia;
    const int acabamento_widia;

    const int CalculateRpmDesbasteHss(float rpm, int diameter);
    const int CalculateRpmAcabamentoHss(float rpm, int diameter);

    const int CalculateRpmDesbasteWidia(float rpm, int diameter);
    const int CalculateRpmAcabamentoWidia(float rpm, int diameter);
};

MatEntry material_get(int index);
int material_inc_indx(int index);
int material_dec_indx(int index);
