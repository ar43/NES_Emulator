#include "utility.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <iomanip>
#include <algorithm>
#include <string>
#include "../logger/logger.h"

namespace utility
{
    std::string int_to_hex(int num, bool tag)
    {
        int width = 2;
        if (num > 0xFF)
            width = 4;
        std::stringstream ss1;
        std::stringstream ss;
        ss1 << std::setfill('0') << std::setw(width) << std::hex << num;
        std::string res1 ( ss1.str() );
        std::transform(res1.begin(), res1.end(),res1.begin(), ::toupper);
        if (tag)
            ss << "0x" << res1;
        else
            ss << res1;
        std::string res(ss.str());
        return res;
    }

    bool IsBitSet(int value, int pos)
    {
        assert(pos >= 0);
        return (value & (1 << pos)) != 0;
    }

    int IsBitSetInt(int value, int pos)
    {
        assert(pos >= 0);
        return (value & (1 << pos)) != 0;
    }

    int GetOtherNametable(int nametable, int scrolling, int mirroring)
    {
        if (scrolling == 0) //horizontal scrolling
        {
            /*if (mirroring == 3)
            {
                if (nametable == 0x2000)
                {
                    return nametable + 0x800;
                }
                else
                {
                    return nametable - 0x800;
                }
            }
            else
            {*/
                if (nametable == 0x2400)
                {
                    return nametable - 0x400;
                }
                else
                {
                    return nametable + 0x400;
                }
            //}
        }
        else if (scrolling == 1) //vertical scrolling
        {
            if (nametable <= 0x2400)
            {
                return nametable + 0x800;
            }
            else
            {
                return nametable - 0x800;
            }
        }
        else
        {
            logger::PrintLine(logger::LogType::FATAL_ERROR, "bad scrolling type");
            return 0;
        }
    }

    void SetBit(int *value, int pos, bool which)
    {
        if (which)
        {
            *value |= 1 << pos;
        }
        else
        {
            *value &= ~(1 << pos);
        }
    }

    void SetBit(uint8_t *value, int pos, bool which)
    {
        if (which)
        {
            *value |= 1 << pos;
        }
        else
        {
            *value &= ~(1 << pos);
        }
    }

    void SetBit(uint32_t *value, int pos, bool which)
    {
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