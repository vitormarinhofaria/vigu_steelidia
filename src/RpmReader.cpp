#include <Arduino.h>
#include "RpmReader.h"

static volatile int g_pulse_counter = 0;
static volatile float g_rpm = 0;
static int g_rpm_counter_pin = 0;
static unsigned long g_last_ts = 0;
static int g_pulses_per_rotation = 1;

static void rpm_reader_interrupt()
{
    if (digitalRead(g_rpm_counter_pin) == HIGH)
    {
        auto current_ts = millis();
        auto diff = current_ts - g_last_ts;
        g_last_ts = current_ts;
        float freq = (1.0f / diff) * 60000;
        g_rpm = freq / (float)g_pulses_per_rotation;
        g_pulse_counter += 1;
    }
}

RpmReader::RpmReader(int pin, int pulses_per_rotation) : m_pin(pin)
{
    g_pulses_per_rotation = pulses_per_rotation;
    g_rpm_counter_pin = pin;
    attachInterrupt(digitalPinToInterrupt(pin), rpm_reader_interrupt, CHANGE);
}
RpmReader::~RpmReader()
{
    detachInterrupt(digitalPinToInterrupt(m_pin));
}

float RpmReader::get()
{
    return g_rpm;
}