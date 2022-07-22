#ifndef __LIB_TIMER__
#define __LIB_TIMER__

#include <chrono>

namespace lib
{
    template<typename T = std::chrono::steady_clock>
    class timer_t
    {
        using clock_t = T;
        using time_point_t = std::chrono::time_point<clock_t>;

        private:
            bool is_running = false;
            time_point_t start_time;
            time_point_t end_time;
            std::chrono::nanoseconds _elapsed;

        public:
            void start () noexcept
            {
                if (this->is_running)
                    return;

                this->start_time = clock_t::now();
                this->is_running = true;
            }
            void stop () noexcept
            {
                if (!this->is_running)
                    return;

                this->end_time = clock_t::now();
                this->_elapsed = this->end_time - this->start_time;
                this->is_running = false;
            }

            inline std::chrono::nanoseconds elapsed () const noexcept
            {
                return this->_elapsed;
            }
            double nanoseconds () const noexcept
            {
                return std::chrono::duration<double, std::nano>(this->_elapsed).count();
            }
            double microseconds () const noexcept
            {
                return std::chrono::duration<double, std::micro>(this->_elapsed).count();
            }
            double milliseconds () const noexcept
            {
                return std::chrono::duration<double, std::milli>(this->_elapsed).count();
            }
            double seconds () const noexcept
            {
                return std::chrono::duration_cast<std::chrono::duration<double>>(this->_elapsed).count();
            }
    };
}

#endif
