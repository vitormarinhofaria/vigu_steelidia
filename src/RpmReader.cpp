#include <Arduino.h>
#include "RpmReader.h"

// static volatile int g_pulse_counter = 0;
// static volatile float g_rpm = 0;
// static int g_rpm_counter_pin = 0;
// static unsigned long g_last_ts = 0;
// static int g_pulses_per_rotation = 1;

static RpmReader *g_rpm_readers[2];

static void rpm_reader_interrupt(RpmReader* reader)
{
    if (digitalRead(reader->m_rpm_counter_pin) == HIGH)
    {
        auto current_ts = millis();
        auto diff = current_ts - reader->m_last_ts;
        reader->m_last_ts = current_ts;
        float freq = (1.0f / diff) * 60000;
        reader->m_rpm = freq / (float)reader->m_pulses_per_rotation;
        reader->m_pulse_counter += 1;
    }
}
static int interrupt_count = 0;
static void rpm_interrupt_0()
{
    detachInterrupt(g_rpm_readers[0]->m_rpm_counter_pin);
    String s = String("Interrupt 0\t Count: " + String(interrupt_count) + "\n");
    Serial.write(s.c_str());
    interrupt_count++;
    rpm_reader_interrupt(g_rpm_readers[0]);
    attachInterrupt(digitalPinToInterrupt(g_rpm_readers[0]->m_rpm_counter_pin), rpm_interrupt_0, CHANGE);
}
static void rpm_interrupt_1()
{
    detachInterrupt(g_rpm_readers[1]->m_rpm_counter_pin);
    String s = String("Interrupt 1\t Count: " + String(interrupt_count) + "\n");
    Serial.write(s.c_str());
    interrupt_count++;
    rpm_reader_interrupt(g_rpm_readers[1]);
    attachInterrupt(digitalPinToInterrupt(g_rpm_readers[1]->m_rpm_counter_pin), rpm_interrupt_1, CHANGE);
}
static int reader_count = 0;
RpmReader::RpmReader(int pin, int pulses_per_rotation) : m_pin(pin)
{
    m_pulses_per_rotation = pulses_per_rotation;
    m_rpm_counter_pin = pin;
    if (reader_count == 0)
    {
        attachInterrupt(digitalPinToInterrupt(pin), rpm_interrupt_0, CHANGE);
        g_rpm_readers[0] = this;
    }
    else if (reader_count == 1)
    {
        attachInterrupt(digitalPinToInterrupt(pin), rpm_interrupt_1, CHANGE);
        g_rpm_readers[1] = this;
    }
    reader_count += 1;
}
RpmReader::~RpmReader()
{
    detachInterrupt(digitalPinToInterrupt(m_pin));
}

float RpmReader::get()
{
    return m_rpm;
}