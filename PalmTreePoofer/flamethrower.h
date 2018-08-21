#ifndef _FLAMETHROWER_H_
#define _FLAMETHROWER_H_

#include <stdint.h>
#include <Arduino.h>

#define MAX_POOF_DURATION 3000

class Flamethrower
{
    protected:
    unsigned int m_solenoidPin;
    unsigned int m_ledPin;
    uint32_t m_poofEndTime;
    unsigned int m_defaultDuration;
    bool m_poofing;

    public:
        Flamethrower(unsigned int solenoidPin, unsigned int ledPin, unsigned int defaultDuration);
        void poof(uint32_t currentTime, int duration = 0);
        void stop(void);
        void process(uint32_t currentTime);
};

#endif //_FLAMETHROWER_H_
