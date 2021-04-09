#pragma once
#include <string>
namespace utility
{
    std::string int_to_hex(int num, bool tag = false);
    bool IsBitSet(int value, int pos);
    int IsBitSetInt(int value, int pos);
    void SetBit(int *value, int pos, bool which);
    void SetBit(uint8_t *value, int pos, bool which);
    void SetBit(uint32_t *value, int pos, bool which);
    int GetOtherNametable(int nametable, int scrolling, int nametable_mirroring);
    std::wstring CharToWString(const char* text);
}