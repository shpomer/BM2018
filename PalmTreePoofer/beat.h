#ifndef _BEAT_H_
#define _BEAT_H_

#include <stdint.h>
#include <Arduino.h>
#include "flamethrower.h"

class Beat
{
    protected:
        unsigned int    m_length;
        const unsigned int *m_intervals;
        const unsigned int *m_durations;
        Flamethrower  **m_flamethrowers;
        bool            m_playing;
        unsigned int    m_index;
        uint32_t        m_nextBeatTime;

    public:
        Beat(const unsigned int *intervals, const unsigned int *durations, Flamethrower **flamethrowers, unsigned int length);
        void start(uint32_t currentTime);
        void stop(void);
        void process(uint32_t currentTime);
};

#endif //_BEAT_H_

