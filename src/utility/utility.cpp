#include "utility.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <iomanip>

namespace utility
{
    std::string int_to_hex(int num)
    {
        std::stringstream ss;
        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << num;
        std::string res ( ss.str() );
        return res;
    }

    bool IsBitSet(int value, int pos)
    {
        assert(pos >= 0);
        return (value & (1 << pos)) != 0;
    }
}