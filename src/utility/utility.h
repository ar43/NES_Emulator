#pragma once
#include <string>
namespace utility
{
    std::string int_to_hex(int num);
    bool IsBitSet(int value, int pos);
    void SetBit(int *value, int pos, bool which);
}