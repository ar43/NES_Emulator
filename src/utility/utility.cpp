#include "utility.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <iomanip>
#include <algorithm>

namespace utility
{
    std::string int_to_hex(int num)
    {
        std::stringstream ss1;
        std::stringstream ss;
        ss1 << std::setfill('0') << std::setw(2) << std::hex << num;
        std::string res1 ( ss1.str() );
        std::transform(res1.begin(), res1.end(),res1.begin(), ::toupper);
        ss << "0x" << res1;
        std::string res(ss.str());
        return res;
    }

    bool IsBitSet(int value, int pos)
    {
        assert(pos >= 0);
        return (value & (1 << pos)) != 0;
    }

    void SetBit(int *value, int pos, bool which)
    {
        assert(pos >= 0);

        if (which)
        {
            *value |= 1 << pos;
        }
        else
        {
            *value &= ~(1 << pos);
        }
    }
}