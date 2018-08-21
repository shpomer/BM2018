#ifndef _BEAT_H_
#define _BEAT_H_

#include <stdint.h>
#include <Arduino.h>
#include "flamethrower.h"

class Beat
{
    protected:
        unsigned int    m_length;
        const uint32_t *m_beatTimes;
        const int      *m_durations;
        Flamethrower  **m_flamethrowers;
        uint32_t        m_beatDuration;
        
        bool            m_playing;
        unsigned int    m_index;
        uint32_t        m_beatStartTime;
        uint32_t        m_nextBeatTime;

    public:
        Beat(const uint32_t *beatTimes, const int *durations, Flamethrower **flamethrowers, unsigned int length, uint32_t beatDuration);
        void start(uint32_t currentTime);
        void stop(void);
        void process(uint32_t currentTime);
};

#endif //_BEAT_H_

