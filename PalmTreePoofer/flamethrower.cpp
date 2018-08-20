#include "flamethrower.h"

Flamethrower::Flamethrower(unsigned int solenoidPin, unsigned int ledPin)
{
    m_solenoidPin = solenoidPin;
    m_ledPin = ledPin;
    this->stop();
}
    
void Flamethrower::poof(uint32_t currentTime, int duration)
{
    if(m_poofing)
    {
        return;
    }

    if(duration < 1 || duration > MAX_POOF_DURATION)
    {
        duration = MAX_POOF_DURATION;
    }
    m_poofEndTime = currentTime + duration;
    
    digitalWrite(m_solenoidPin, HIGH);
    digitalWrite(m_ledPin, HIGH);
    
    m_poofing = true;
}

void Flamethrower::stop(void)
{
    digitalWrite(m_solenoidPin, LOW);
    digitalWrite(m_ledPin, LOW);
    
    m_poofing = false;
}

void Flamethrower::process(uint32_t currentTime)
{
    if(currentTime > m_poofEndTime)
    {
        this->stop();
    }
}
