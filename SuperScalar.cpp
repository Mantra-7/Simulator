#include <iostream>
#include <fstream>
using namespace std;

#define BLOCK_SIZE 4
#define NUM_SETS 64
#define NUM_REGS 16

typedef int int8;
typedef int int16;
typedef bool flag;

class Register
{
    int8 m_val;
public:
    flag busy;
    flag valid;
    int tag;
    int8 read();
    void write(int8 val); 
};

class Register16
{
    int16 m_val;
public:
    int16 read();
    void write(int16 val); 
};

class RegisterFile{
public:
    Register R[NUM_REGS];
    int8 read(int Pos);
    void write(int Pos, int8 val);
    void output(ofstream &fout);
    RegisterFile(ifstream &rf);
};

class RenameRegisterFile{
public:
    Register R[NUM_REGS];
    int8 read(int Pos);
    void write(int Pos, int8 val);
};

typedef struct Block{
    int8 m_offset[BLOCK_SIZE];
} Block;

class ICache{
    Block m_data[NUM_SETS];
public:
    ICache(){}
    ICache(ifstream &fin);
    int16 request(int8 addr);
    void write(int8 addr, int16 val);
};

class DCache{
    Block m_data[NUM_SETS];
public:
    DCache(){}
    DCache(ifstream &fin);
    void output(ofstream &fout);
    int8 request(int8 addr);
    void write(int8 addr, int8 val);
};

class PC
{
    int8 m_addr;
public:
    int8 read();
    void write(int8 addr);
    void increment();
    void decrement();
};

class ALU
{
public:
    int8 adder(int8 X , int8 Y, flag as);
    int8 MUL(int8 X, int8 Y);
    int8 AND(int8 X, int8 Y);
    int8 OR(int8 X, int8 Y);
    int8 NOT(int8 X);
    int8 XOR(int8 X, int8 Y);
    flag BEQZ(int X);
};

int8 ALU::adder(int8 X , int8 Y, flag as)
{
    if(!as)
    {
        return X + Y;
    }
    else
    {
        return X - Y;
    }
}

int8 ALU::MUL(int8 X, int8 Y)
{
    return X * Y;
}

int8 ALU::OR(int8 X, int8 Y)
{
    return X | Y;
}

int8 ALU::AND(int8 X, int8 Y)
{
    return X & Y;
}

int8 ALU::NOT(int8 X)
{
    return ~X;
}

int8 ALU::XOR(int8 X, int8 Y)
{
    return X ^ Y;
}

flag ALU::BEQZ(int X)
{
    return X == 0;
}

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

int16 ICache::request(int8 addr)
{
    int set = addr >> 2;
    int offset = addr & 3;
    int16 little = m_data[set].m_offset[offset];
    int16 big = m_data[set].m_offset[offset + 1];
    return (little<<8) + big;
}

void ICache::write(int8 addr, int16 val)
{
    int set = addr >> 2;
    int offset = addr & 3;
    m_data[set].m_offset[offset] = val >> 8;
    m_data[set].m_offset[offset + 1] = val & 0xff;
}

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
        R[i].busy = false;
        R[i].tag = -1;
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

int8 PC::read()
{
    return m_addr;
}

void PC::write(int8 addr)
{
    m_addr = addr;
}

void PC::increment()
{
    m_addr+=2;
}

void PC::decrement()
{
    m_addr-=2;
}

class instbufelem{
public:
    int16 instruction;
    bool busy;
};

class dispbufelem{
public:
    int16 instruction;
    int opcode;
    int subop;
    bool busy;

    flag arithmatic;
    flag logical;
    flag branch;
    flag load;
    flag store;
    flag jump;
    flag beqz;
    flag halt;

    Register src1;
    Register src2;
    Register dest;
};

PC pc;
ICache icache;
DCache dcache;
RegisterFile ARF;
RenameRegisterFile RRF;
instbufelem FetchDecodeBuf[8];
dispbufelem DispatchBuf[8];

void Fetch()
{
    int free=-1;
    for(int i=0;i<8;i++)
    {
        if(FetchDecodeBuf[i].busy==false)
        {
            free=i;
            FetchDecodeBuf[i].busy=true;
            break;
        }
    }

    if(free!=-1)
    {
        int8 addr = pc.read();
        pc.increment();
        FetchDecodeBuf[free].instruction = icache.request(addr);
    }
}

int findFree()
{
    for(int i=0;i<256;i++)
    {
        if(!RRF.R[i].busy)
        {
            return i;
        }
    }
}

void Decode()
{
    int free=-1;
    for(int i=0;i<8;i++)
    {
        if(DispatchBuf[i].busy==false)
        {
            free=i;
            DispatchBuf[i].busy=true;
            break;
        }
    }

    int instfree=-1;
    for(int i=0;i<8;i++)
    {
        if(FetchDecodeBuf[i].busy==true)
        {
            instfree=i;
            FetchDecodeBuf[i].busy=false;
            break;
        }
    }

    if(instfree!=-1)
    {
        int16 instruction = FetchDecodeBuf[instfree].instruction;

        DispatchBuf[free].arithmatic = false;
        DispatchBuf[free].logical = false;
        DispatchBuf[free].load = false;
        DispatchBuf[free].store = false;
        DispatchBuf[free].beqz = false;
        DispatchBuf[free].jump = false;
        DispatchBuf[free].halt = false;

        int16 opcode = instruction >> 12;
        DispatchBuf[free].opcode = opcode;

        if(opcode<4)
        {
            DispatchBuf[free].arithmatic = true;

            int8 r3 = (instruction & 0x0f00)>>8;
            int8 r1 = (instruction & 0x00f0)>>4;
            int8 r2 = (instruction & 0x000f);

            if(!ARF.R[r1].busy)
            {
                DispatchBuf[free].src1 = ARF.R[r1];
            }
            else
            {
                int tag = ARF.R[r1].tag;
                DispatchBuf[free].src1 = RRF.R[tag];
            }

            if(!ARF.R[r2].busy)
            {
                DispatchBuf[free].src2 = ARF.R[r2];
            }
            else
            {
                int tag = ARF.R[r2].tag;
                DispatchBuf[free].src2 = RRF.R[tag];
            }

            int rfree=findFree();
            ARF.R[r3].busy = true;
            ARF.R[r3].tag = rfree;

            DispatchBuf[free].dest = RRF.R[rfree];
            RRF.R[rfree].busy = true;
            RRF.R[rfree].valid = false;
        }

        else if(opcode<8)
        {
            DispatchBuf[free].logical = true;

            int8 r3 = (instruction & 0x0f00)>>8;
            int8 r1 = (instruction & 0x00f0)>>4;
            int8 r2 = (instruction & 0x000f);

            // not
            if(opcode==6)
            {
                if(RF.R[r1].valid)
                {
                    DispatchBuf[free].src1 = RF.read(r1);
                    DispatchBuf[free].dest = r3;
                    RF.R[r3].valid = false;
                    DispatchBuf[free].subop = opcode & 0x03;
                }
                else
                {
                    stall = true;
                    DispatchBuf[free].valid = false;
                    RF.R[r1].dh=true;
                    dataHaz++;
                }
                
            }

            // and , or, xor
            else if(RF.R[r1].valid && RF.R[r2].valid)
            {
                DispatchBuf[free].src1 = RF.read(r1);
                DispatchBuf[free].src2 = RF.read(r2);
                DispatchBuf[free].dest = r3;
                RF.R[r3].valid = false;
                DispatchBuf[free].subop = opcode & 0x03;
            }
            else
            {
                stall = true;
                DispatchBuf[free].valid = false;
                if(!RF.R[r1].valid)  
                {
                    RF.R[r1].dh=true;
                    dataHaz++;
                }
                if(!RF.R[r2].valid && r1!=r2) 
                {
                    RF.R[r2].dh=true;
                    dataHaz++;
                }
            }
        }
        
        // load
        if(opcode == 8)
        {
            DispatchBuf[free].load = true;
            int8 r1 = (instruction & 0x0f00)>>8;
            int8 r2 = (instruction & 0x00f0)>>4;
            int8 x = (instruction & 0x000f);
            if(RF.R[r2].valid)
            {
                DispatchBuf[free].src1 = r1;
                DispatchBuf[free].src2 = RF.read(r2);
                DispatchBuf[free].offset = x;
                RF.R[r1].valid = false;
            }
            else
            {
                stall = true;
                DispatchBuf[free].valid = false;
                RF.R[r2].dh = true;
                dataHaz++;
            }   
        }

        // store
        if(opcode == 9)
        {
            DispatchBuf[free].store = true;
            int8 r1 = (instruction & 0x0f00)>>8;
            int8 r2 = (instruction & 0x00f0)>>4;
            int8 x = (instruction & 0x000f);

            if(RF.R[r1].valid && RF.R[r2].valid)
            {
                DispatchBuf[free].src2 = RF.read(r2);
                DispatchBuf[free].src1 = RF.read(r1);
                DispatchBuf[free].offset = x;
            }
            else
            {
                stall = true;
                DispatchBuf[free].valid = false;
                if(!RF.R[r1].valid)  
                {
                    RF.R[r1].dh=true;
                    dataHaz++;
                }
                if(!RF.R[r2].valid && r1!=r2) 
                {
                    RF.R[r2].dh=true;
                    dataHaz++;
                }
            }
        }

        //jmp
        if(opcode == 10)
        {
            branch_resolved = false;
            DispatchBuf[free].jump = true;
            DispatchBuf[free].jump_addr = (instruction & 0x0ff0)>>4;
        }

        //beqz
        if(opcode == 11)
        {
            DispatchBuf[free].beqz = true;
            int8 r1 = (instruction & 0x0f00)>>8;
            if(!RF.R[r1].valid)
            {
                stall = true;
                DispatchBuf[free].valid = false;
                RF.R[r1].dh = true;
                dataHaz++;
            }
            else
            {
                branch_resolved = false;
                DispatchBuf[free].src1 = RF.read(r1);
                DispatchBuf[free].jump_addr = instruction & 0x00ff;
            }
        }

        if(opcode==15)
        {
            DispatchBuf[free].halt = true;
            stop = true;
        }
    }
}