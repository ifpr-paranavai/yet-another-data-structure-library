#include "csvfile.h"

namespace lib
{
    csvfile_t::~csvfile_t ()
    {
        flush();
        fs.close();
    }

    csvfile_t::csvfile_t (const char *filename, const char *separator, const std::ios_base::openmode& openmode) : fs()
    {
        this->separator = separator;
        fs.exceptions(std::ios::failbit | std::ios::badbit);
        fs.open(filename, openmode);
    } 

    std::string csvfile_t::escape (const std::string& val)
    {
        std::ostringstream result;
        result << '"';
        std::string::size_type to, from = 0u, len = val.length();
        
        while (from < len && std::string::npos != (to = val.find_first_of(special_chars, from))) {
            result << val.substr(from, to - from) << escape_seq << val[to];
            from = to + 1;
        }

        result << val.substr(from) << '"';
        return result.str();
    }

    void csvfile_t::flush ()
    {
        fs.flush();
    }

    void csvfile_t::endrow ()
    {
        fs << std::endl;
        is_first = true;
    }
}
