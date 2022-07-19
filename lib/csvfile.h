#ifndef __LIB_CSVFILE__
#define __LIB_CSVFILE__

#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace lib 
{
    class csvfile_t;

    inline static csvfile_t& endrow(csvfile_t& file);
    inline static csvfile_t& flush(csvfile_t& file);
    class csvfile_t
    {
        private:
            std::ofstream fs;
            bool is_first = true;
            std::string separator;
            std::string escape_seq = "\"";
            std::string special_chars = "\"";

        private:
            std::string escape(const std::string &val);

            template <typename T>
            csvfile_t& write(const T &val)
            {
                if (!is_first)
                    fs << separator;
                else
                    is_first = false;
                fs << val;
                return *this;
            }
            
        public:
            ~csvfile_t ();
            csvfile_t (const char *filename, const char *separator = ",", const std::ios_base::openmode &openmode = std::ostream::out);
            void flush ();
            void endrow ();
        
        public:
            csvfile_t& operator << (csvfile_t& (* val)(csvfile_t&))
            {
                return val(*this);
            }

            csvfile_t& operator << (const char *val)
            {
                return write(escape(val));
            }

            csvfile_t& operator << (const std::string& val)
            {
                return write(escape(val));
            }

            template<typename T>
            csvfile_t& operator << (const T& val)
            {
                return write(val);
            }
    };


    inline static csvfile_t& endrow(csvfile_t& file)
    {
        file.endrow();
        return file;
    }

    inline static csvfile_t& flush(csvfile_t& file)
    {
        file.flush();
        return file;
    }
}


#endif
