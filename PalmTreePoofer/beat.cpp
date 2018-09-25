#include "beat.h"

Beat::Beat(const uint32_t *beatTimes, const int *durations, Flamethrower **flamethrowers, unsigned int length, uint32_t beatDuration)
{
    m_beatTimes = beatTimes;
    m_durations = durations;
    m_flamethrowers = flamethrowers;
    m_length = length;
    m_beatDuration = beatDuration;

    m_playing = false;
}

void Beat::start(uint32_t currentTime)
{
    m_index = 0;
    m_beatStartTime = currentTime;
    m_nextBeatTime = m_beatStartTime + m_beatTimes[m_index];
    m_playing = true;
    m_nFullBeats = 0;
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
        if(m_index >= m_length)
        {
            if(currentTime >= m_beatStartTime + m_beatDuration)
            {
                m_index = 0;
                m_beatStartTime = currentTime;
                m_nFullBeats++;
                if(m_nFullBeats >= N_FULL_BEATS_MAX)
                {
                    this->stop();
                }
            }
        }
        else
        {
            int duration = m_durations == NULL ? 0 : m_durations[m_index];
            m_flamethrowers[m_index]->poof(currentTime, duration);
            Serial.println(m_index);
            ++m_index;
            m_nextBeatTime = m_beatStartTime + m_beatTimes[m_index];
        }
    }
}

