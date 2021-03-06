
  
#include "../include/Structures.hpp"

int8 DCache::request(int8 addr)
{
    //cout<<"req "<<(int)addr<<endl;
    int set = addr >> 2;
    int offset = addr & 3;
    //cout<<"getting "<<(int)m_data[set].m_offset[offset]<<endl;
    return m_data[set].m_offset[offset];
}

void DCache::write(int8 addr, int8 val)
{
    int set = addr >> 2;
    int offset = addr & 3;
    m_data[set].m_offset[offset] = val;
}

DCache::DCache(ifstream &fin)
{
    int8 addr = 0;
    int8 input;
    while(fin>>hex>>input)
    {
        this->write(addr, input);
        addr += 1;
    }
}

void DCache::output(ofstream &fout)
{
    for(int i = 0; i < NUM_SETS; i++)
    {
        for(int j = 0; j < BLOCK_SIZE; j++)
        {
            int y=(int)m_data[i].m_offset[j];
            fout<<hex<<((y&0xf0)>>4)<<(y&0xf)<<endl;
        }
    }
}
