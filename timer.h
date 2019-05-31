#ifndef SEEMMO_TIMER_H
#define SEEMMO_TIMER_H

#include <stdio.h>
#include <sys/time.h>

namespace seemmo{

class Timer{

    public:
        Timer(){
            gettimeofday(&_tv, NULL);
        }
        long interval(){
            return Timer::currentMillionSecond() - getMillionSecond();
        }
        void reset(){
            gettimeofday(&_tv, NULL); 
        }
        long getSecond(){
            return getMicroSecond() / 1000000;
        }
        long getMillionSecond(){
            return getMicroSecond() / 1000;
        }
        long getMicroSecond(){
            return _tv.tv_sec * 1000000 + _tv.tv_usec;
        }
        static long currentSecond(){
            return Timer::currentMicroSecond() / 1000000;
        }
        static long currentMillionSecond(){
            return Timer::currentMicroSecond() / 1000;
        }
        static long currentMicroSecond(){
            struct timeval currentTime;
            gettimeofday(&currentTime, NULL);
            return currentTime.tv_sec * 1000000 + currentTime.tv_usec;
        }
    private:
        struct timeval _tv;

};

}


#endif

