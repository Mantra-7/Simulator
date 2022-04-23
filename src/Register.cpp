#include "../include/Structures.hpp"

int8 Register::read()
{
    return m_val;
}

void Register::write(int8 val)
{
    m_val = val;
}

int16 Register16::read()
{
    return m_val;
}

void Register16::write(int16 val)
{
    m_val = val;
}

RegisterFile::RegisterFile(ifstream &rf)
{
    for(int i = 0; i < NUM_REGS; i++)
    {
        int input;
        rf >> hex >> input;
        R[i].write(input);
        R[i].valid = true;
    }
}

int8 RegisterFile::read(int Pos)
{
    return R[Pos].read();
}

void RegisterFile::write(int Pos, int8 val)
{
    R[Pos].write(val);
}

void RegisterFile::output(ofstream &fout)
{
    for(int i = 0; i < NUM_REGS; i++)
    {
        fout << hex << ((R[i].read() & 0xf0) >> 4) << (R[i].read() & 0xf) << endl;
    }
}