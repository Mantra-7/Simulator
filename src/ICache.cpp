#include "../include/Structures.hpp"

int16 ICache::request(int8 addr)
{
    int set = addr >> 2;
    int offset = addr & 3;
    int16 little = m_data[set].m_offset[offset];
    int16 big = m_data[set].m_offset[offset + 1];

    return little<<8 + big;
}

void ICache::write(int8 addr, int16 val)
{
    int set = addr >> 2;
    int offset = addr & 3;
    m_data[set].m_offset[offset] = val >> 8;
    m_data[set].m_offset[offset + 1] = val & 0xff;
}

ICache::ICache(ifstream &fin)
{
    int8 addr = 0;
    int input;
    while(fin>>input)
    {
        this->write(addr, input);
        addr += 4;
    }
}