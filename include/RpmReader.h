#pragma once

class RpmReader
{
public:
    RpmReader(int pin, int pulses_per_rotation);
    ~RpmReader();
    float get();

private:
    int m_pin = 0;
    int m_rpm_value = 0;
    //unsigned long m_last_ts = 0;
public:
    volatile int m_pulse_counter = 0;
    volatile float m_rpm = 0;
    int m_rpm_counter_pin = 0;
    unsigned long m_last_ts = 0;
    int m_pulses_per_rotation = 1;
};