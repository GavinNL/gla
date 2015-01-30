#ifndef GLRETIMER_H
#define GLRETIMER_H

#include <iostream>
#include <chrono>
#include <ctime>

namespace glre {

    template<typename FloatOrDouble>
    class Timer_T {

        public:

            Timer_T()
            {
                reset();
            }


            /** Gets the time elapsed since the timer was constructed, or when the last reset() was called.
             *
             * @return double seconds since construction or reset call()
             */
            double   getElapsedTime()
            {
                auto end = std::chrono::system_clock::now();
                std::chrono::duration<FloatOrDouble> elapsed_seconds = end-mStart;
                return elapsed_seconds.count();
            }

            /** Resets the timer
             *
             * @param zero Optional argument to set the zero point of the reset.
             */
            void     reset(FloatOrDouble zero = 0.0)
            {
                mStart = std::chrono::system_clock::now();
                mStart += std::chrono::microseconds( static_cast<unsigned int>( zero*1e6) );
            }


        private:
            std::chrono::time_point<std::chrono::system_clock> mStart;
    };

};

#endif // TIMER_H
