#include "flamethrower.h"

Flamethrower::Flamethrower(unsigned int solenoidPin, unsigned int ledPin, unsigned int defaultDuration)
{
    m_solenoidPin = solenoidPin;
    m_ledPin = ledPin;
    m_defaultDuration = defaultDuration;
    this->stop();
}
    
void Flamethrower::poof(uint32_t currentTime, int duration)
{
    if(m_poofing || duration < 0)
    {
        return;
    }

    if(duration == 0 || duration > MAX_POOF_DURATION)
    {
        duration = m_defaultDuration;
    }
    m_poofEndTime = currentTime + duration;
    
    digitalWrite(m_solenoidPin, HIGH);
    digitalWrite(m_ledPin, LOW);

    Serial.println(m_defaultDuration);
    
    m_poofing = true;
}

void Flamethrower::stop(void)
{
    digitalWrite(m_solenoidPin, LOW);
    digitalWrite(m_ledPin, HIGH);
    
    m_poofing = false;
}

void Flamethrower::process(uint32_t currentTime)
{
    if(currentTime > m_poofEndTime)
    {
        this->stop();
    }
}
