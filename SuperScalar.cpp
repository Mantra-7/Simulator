#include <bits/stdc++.h>

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
    RegisterFile(){}
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

ICache::ICache(ifstream &fin)
{
    int8 addr = 0;
    int16 input;

    while(fin>>hex>>input)
    {
        int16 x = input;
        fin>>hex>>input;
        x = (x<<8) + input;
        //cout<<"input "<<x<<endl;
        write(addr, x);
        addr += 2;
    }
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
    int offset;

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
    int dest;
    dispbufelem(){
        busy = false;
    }
};

class ResStElem
{
public:
    dispbufelem elem;
    flag busy;
    flag ready;
    ResStElem() : busy(false), ready(false){}
};

class ROBelem
{
public:
    flag busy;
    flag issued;
    flag finished;
    dispbufelem elem;
    flag valid;
    ROBelem() : busy(false) {}
    ROBelem(dispbufelem d) : elem(d), busy(true) {}
};

PC pc;
ICache icache;
DCache dcache;
ALU alu;
RegisterFile ARF;
RenameRegisterFile RRF;
instbufelem FetchDecodeBuf[8];
dispbufelem DispatchBuf[8];
ResStElem intResSt[8];
ResStElem branchResSt[8];
ResStElem loadStoreResSt[8];
flag halt=false;
flag stop=false;
vector<ROBelem> ROB;

bool operator==(const dispbufelem &x, const dispbufelem &y)
{
    
}

void Fetch()
{
    if(halt) return;

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
        DispatchBuf[free].subop = opcode>>2;

        if(opcode<4)
        {
            DispatchBuf[free].arithmatic = true;

            int8 r3 = (instruction & 0x0f00)>>8;
            int8 r1 = (instruction & 0x00f0)>>4;
            int8 r2 = (instruction & 0x000f);

            //inc 
            if(opcode==3)
            {
                if(!ARF.R[r3].busy)
                {
                    DispatchBuf[free].src1 = ARF.R[r3];
                }
                else
                {
                    int tag = ARF.R[r3].tag;
                    DispatchBuf[free].src1.tag = tag;
                }

                int rfree=findFree();
                ARF.R[r3].busy = true;
                ARF.R[r3].tag = rfree;

                DispatchBuf[free].dest = rfree;
                RRF.R[rfree].busy = true;
                RRF.R[rfree].valid = false;
            }
            else
            {
                if(!ARF.R[r1].busy)
                {
                    DispatchBuf[free].src1 = ARF.R[r1];
                }
                else
                {
                    int tag = ARF.R[r1].tag;
                    DispatchBuf[free].src1.tag = tag;
                }

                if(!ARF.R[r2].busy)
                {
                    DispatchBuf[free].src2 = ARF.R[r2];
                }
                else
                {
                    int tag = ARF.R[r2].tag;
                    DispatchBuf[free].src2.tag = tag;
                }

                int rfree=findFree();
                ARF.R[r3].busy = true;
                ARF.R[r3].tag = rfree;

                DispatchBuf[free].dest = rfree;
                RRF.R[rfree].busy = true;
                RRF.R[rfree].valid = false;
            }            
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
                if(!ARF.R[r1].busy)
                {
                    DispatchBuf[free].src1 = ARF.R[r1];
                }
                else
                {
                    int tag = ARF.R[r1].tag;
                    DispatchBuf[free].src1.tag = tag;
                }

                int rfree=findFree();
                ARF.R[r3].busy = true;
                ARF.R[r3].tag = rfree;

                DispatchBuf[free].dest = rfree;
                RRF.R[rfree].busy = true;
                RRF.R[rfree].valid = false;
                
            }

            // and , or, xor
            else
            {
                if(!ARF.R[r1].busy)
                {
                    DispatchBuf[free].src1 = ARF.R[r1];
                }
                else
                {
                    int tag = ARF.R[r1].tag;
                    DispatchBuf[free].src1.tag = tag;
                }   
                if(!ARF.R[r2].busy)
                {
                    DispatchBuf[free].src2 = ARF.R[r2];
                }
                else
                {
                    int tag = ARF.R[r2].tag;
                    DispatchBuf[free].src2.tag = tag;
                }

                int rfree=findFree();
                ARF.R[r3].busy = true;
                ARF.R[r3].tag = rfree;

                DispatchBuf[free].dest = rfree;
                RRF.R[rfree].busy = true;
                RRF.R[rfree].valid = false;
            }
        }
        
        // load
        if(opcode == 8)
        {
            DispatchBuf[free].load = true;

            int offset = (instruction & 0x000f);
            int8 r2 = (instruction & 0x00f0)>>4;
            int8 r1 = (instruction & 0x0f00)>>8;

            if(!ARF.R[r2].busy)
            {
                DispatchBuf[free].src2 = ARF.R[r2];
            }
            else
            {
                int tag = ARF.R[r2].tag;
                DispatchBuf[free].src2.tag = tag;
            }

            int rfree=findFree();
            ARF.R[r1].busy = true;
            ARF.R[r1].tag = rfree;

            DispatchBuf[free].dest = rfree;
            RRF.R[rfree].busy = true;
            RRF.R[rfree].valid = false;

            DispatchBuf[free].offset = offset;            
        }

        // store
        if(opcode == 9)
        {
            DispatchBuf[free].store = true;

            int offset = (instruction & 0x000f);
            int8 r2 = (instruction & 0x00f0)>>4;
            int8 r1 = (instruction & 0x0f00)>>8;

            if(!ARF.R[r1].busy)
            {
                DispatchBuf[free].src1 = ARF.R[r1];
            }
            else
            {
                int tag = ARF.R[r1].tag;
                DispatchBuf[free].src1.tag = tag;
            }

            if(!ARF.R[r2].busy)
            {
                DispatchBuf[free].src2 = ARF.R[r2];
            }
            else
            {
                int tag = ARF.R[r2].tag;
                DispatchBuf[free].src2.tag = tag;
            }

            DispatchBuf[free].offset = offset;
        }

        //jmp
        if(opcode == 10)
        {
            //huh
        }

        //beqz
        if(opcode == 11)
        {
            //huh
        }

        if(opcode==15)
        {
            DispatchBuf[free].halt = true;
            //stop = true;
        }
    }
}

void Dispatch()
{
    for(int i=0;i<256;i++)
    {
        if(DispatchBuf[i].busy)
        {
            if(DispatchBuf[i].arithmatic || DispatchBuf[i].logical)
            {
                flag passed=false;
                for(int j=0;j<8;j++)
                {
                    if(!intResSt[j].busy)
                    {
                        intResSt[j].elem = DispatchBuf[i];
                        intResSt[j].busy = true;
                        intResSt[j].ready = false;
                        passed = true;
                        ROB.push_back(ROBelem(DispatchBuf[i]));
                        break;
                    }
                }

                if(passed)
                {
                    DispatchBuf[i].busy = false;
                }
            }

            if(DispatchBuf[i].beqz || DispatchBuf[i].jump)
            {
                flag passed=false;
                for(int j=0;j<8;j++)
                {
                    if(!branchResSt[j].busy)
                    {
                        branchResSt[j].elem = DispatchBuf[i];
                        branchResSt[j].busy = true;
                        branchResSt[j].ready = false;
                        passed = true;
                        ROB.push_back(ROBelem(DispatchBuf[i]));
                        break;
                    }
                }

                if(passed)
                {
                    DispatchBuf[i].busy = false;
                }
            }
            
            if(DispatchBuf[i].load || DispatchBuf[i].store)
            {
                flag passed=false;
                for(int j=0;j<8;j++)
                {
                    if(!loadStoreResSt[j].busy)
                    {
                        loadStoreResSt[j].elem = DispatchBuf[i];
                        loadStoreResSt[j].busy = true;
                        loadStoreResSt[j].ready = false;
                        passed = true;
                        ROB.push_back(ROBelem(DispatchBuf[i]));
                        break;
                    }
                }

                if(passed)
                {
                    DispatchBuf[i].busy = false;
                }
            }

            if(DispatchBuf[i].halt)
            {
                ROB.push_back(ROBelem(DispatchBuf[i]));
                halt=true;
            }
        }
    }
}

int findInROB(dispbufelem inst)
{
    for(int i=0;i<ROB.size();i++)
    {
        if(ROB[i].elem == inst) return i;
    }
}

void intexecute()
{
    int ready=-1;
    for(int i=0;i<8;i++)
    {
        if(intResSt[i].ready)
        {
            ready=i;
            break;
        }
    }

    if(ready!=-1)
    {
        dispbufelem inst = intResSt[ready].elem;
        
        if(inst.arithmatic)
        {
            int result;
            
            if(inst.subop==0) result=alu.adder(inst.src1.read(),inst.src2.read(),0);
            if(inst.subop==1) result=alu.adder(inst.src1.read(),inst.src2.read(),1);
            if(inst.subop==2) result=alu.adder(inst.src1.read(),1,0);
            if(inst.subop==3) result=alu.MUL(inst.src1.read(),inst.src2.read());

            int tag = inst.dest;
            RRF.R[tag].write(result);
            RRF.R[tag].valid = true;
        }

        if(inst.logical)
        {
            int result;
            
            if(inst.subop==0) result=alu.AND(inst.src1.read(),inst.src2.read());
            if(inst.subop==1) result=alu.OR(inst.src1.read(),inst.src2.read());
            if(inst.subop==2) result=alu.NOT(inst.src2.read());
            if(inst.subop==3) result=alu.XOR(inst.src1.read(),inst.src2.read());

            int tag = inst.dest;
            RRF.R[tag].write(result);
            RRF.R[tag].valid = true;
        }


    }
}

dispbufelem lsbuff[3];
int addressbuf = -1;
int destaddressbuf = -1;

void addressGeneration(dispbufelem inst)
{
    if(inst.busy)
    {
        int src2 = inst.src2.read();
        int offset = inst.offset;
        int result = alu.adder(src2,offset,0);
        addressbuf = result;
    }
}

void addressTranslation(dispbufelem inst)
{
    if(addressbuf!=-1 && inst.busy)
    {
        if(inst.load)
        {
            destaddressbuf = dcache.request(addressbuf);
        }
        else
        { 
            destaddressbuf = -1;
            dcache.write(addressbuf,inst.src1.read());
        }    
    }
}

void loading(dispbufelem inst)
{
    if(destaddressbuf!=-1 && inst.busy)
    {
        if(inst.load)
        {
            int tag = inst.dest;
            RRF.R[tag].write(destaddressbuf);
            RRF.R[tag].valid = true;
        }
    }
}

void loadstoreexecute()
{
    int ready=-1;
    for(int i=0;i<8;i++)
    {
        if(intResSt[i].ready)
        {
            ready=i;
            break;
        }
    }
    lsbuff[2] = lsbuff[1];
    lsbuff[1] = lsbuff[0];
    if(ready==-1)
    {
        lsbuff[0] = dispbufelem(); 
    }
    else
    {
        lsbuff[0] = intResSt[ready].elem;
    }
    loading(lsbuff[2]);
    addressTranslation(lsbuff[1]);
    addressGeneration(lsbuff[0]);
}

void branchexecute()
{
    
}

void execute()
{
    intexecute();
    branchexecute();
    loadstoreexecute();
}

void RSdispatch()
{
    for(int i=0;i<8;i++)
    {
        if(intResSt[i].busy)
        {
            dispbufelem x = intResSt[i].elem;
            bool ready=true;

            if(x.opcode != 3 && x.opcode!=6)
            {
                if(x.src1.tag!=-1)
                {
                    if(!RRF.R[x.src1.tag].valid) ready=false;
                }
            }

            if(x.src2.tag!=-1)
            {
                if(!RRF.R[x.src2.tag].valid) ready=false;
            }

            intResSt[i].ready = ready;
        }
    }

    for(int i=0;i<8;i++)
    {
        if(branchResSt[i].busy)
        {
            dispbufelem x = branchResSt[i].elem;
            bool ready=true;
            if(x.src1.tag!=-1)
            {
                if(!RRF.R[x.src1.tag].valid) ready=false;
            }

            if(x.src2.tag!=-1)
            {
                if(!RRF.R[x.src2.tag].valid) ready=false;
            }

            branchResSt[i].ready = ready;
        }
    }

    for(int i=0;i<8;i++)
    {
        if(loadStoreResSt[i].busy)
        {
            dispbufelem x = loadStoreResSt[i].elem;
            bool ready=true;

            if(x.load)
            {
                if(x.src1.tag!=-1)
                {
                    if(!RRF.R[x.src1.tag].valid) ready=false;
                }
            }

            if(x.src2.tag!=-1)
            {
                if(!RRF.R[x.src2.tag].valid) ready=false;
            }

            loadStoreResSt[i].ready = ready;
        }
    }
}

void execute_cycle()
{
    RSdispatch();
    execute();
    Dispatch();
    Decode();
    Fetch();
}

int main()
{
    ifstream icache("input/ICache.txt");
    ifstream dcache("input/DCache.txt");
    ifstream rf("input/RF.txt");
    ofstream dCacheOutput("output/ODCache.txt");
    ofstream rfOutput("output/ORF.txt");
    ofstream stats("output/Output.txt");

    ICache(icache);
    DCache(dcache);
    RegisterFile(rf);

    execute_cycle();
}