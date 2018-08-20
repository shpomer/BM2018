#include "beat.h"

Beat::Beat(const unsigned int *intervals, const unsigned int *durations, Flamethrower **flamethrowers, unsigned int length)
{
    m_intervals = intervals;
    m_durations = durations;
    m_flamethrowers = flamethrowers;
    m_length = length;

    m_playing = false;
}

void Beat::start(uint32_t currentTime)
{
    m_index = 0;
    m_nextBeatTime = 0;
    m_playing = true;
}

void Beat::stop(void)
{
    m_playing = false;
}

void Beat::process(uint32_t currentTime)
{
    if(!m_playing)
    {
        return;
    }
    
    if(currentTime >= m_nextBeatTime)
    {
        m_flamethrowers[m_index]->poof(currentTime, m_durations[m_index]);
        m_nextBeatTime = currentTime + m_intervals[m_index];
        m_index = (m_index + 1) % m_length;
    }
}

