#include "Structures.hpp"

int8 DCache::request(int8 addr)
{
    int set = addr >> 2;
    int offset = addr & 3;
    return m_data[set].m_offset[offset];
}

void DCache::write(int8 addr, int8 val)
{
    int set = addr >> 2;
    int offset = addr & 3;
    m_data[set].m_offset[offset] = val;
}

void DCache::DCache(ifstream fin)
{
    int8 addr = 0;
    while(getline(fin, input))
    {
        this->write(addr, input);
        addr += 4;
    }
}
    