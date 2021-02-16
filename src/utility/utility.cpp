#include "utility.h"
#include <iostream>
#include <sstream>

namespace utility
{
    std::string int_to_hex(int num)
    {
        std::stringstream ss;
        ss<< std::hex << num;
        std::string res ( ss.str() );
        return res;
    }
}