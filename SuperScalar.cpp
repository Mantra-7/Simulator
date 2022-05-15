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
    PC() : m_addr(0){}
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
    //cout<<"add"<<endl;
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
    //cout<<"mul"<<endl;
    return X * Y;
}

int8 ALU::OR(int8 X, int8 Y)
{
    //cout<<"or"<<endl;
    return X | Y;
}

int8 ALU::AND(int8 X, int8 Y)
{
    //cout<<"and"<<endl;
    return X & Y;
}

int8 ALU::NOT(int8 X)
{
    //cout<<"not"<<endl;
    return ~X;
}

int8 ALU::XOR(int8 X, int8 Y)
{
    //cout<<"xor"<<endl;
    return X ^ Y;
}

flag ALU::BEQZ(int X)
{
    return X == 0;
}

int8 DCache::request(int8 addr)
{
    ////cout<<"req "<<(int)addr<<endl;
    int set = addr >> 2;
    int offset = addr & 3;
    ////cout<<"getting "<<(int)m_data[set].m_offset[offset]<<endl;
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
        // //cout<<"input "<<x<<endl;
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
    // //cout<<"ICache: "<< (int)little<<" "<<(int)big<<endl;
    return (little<<8) + big;
}

void ICache::write(int8 addr, int16 val)
{
    int set = addr >> 2;
    int offset = addr & 3;
    m_data[set].m_offset[offset] = val >> 8;
    m_data[set].m_offset[offset + 1] = val & 0xff;
    // //cout<<"ICache: "<< (int)m_data[set].m_offset[offset]<<" "<<(int)m_data[set].m_offset[offset + 1]<<endl;
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
    int8 addr;
    bool busy;
};

class dispbufelem{
public:
    int16 instruction;
    int8 addr;
    int opcode;
    int subop;
    bool busy;
    int offset;

    flag arithmatic;
    flag logical;
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
    ROBelem() : busy(false) , finished(false) {}
    ROBelem(dispbufelem d) : elem(d), busy(true) , finished(false) {}
};

ifstream icaches("input/ICache.txt");
ifstream dcaches("input/DCache.txt");
ifstream rf("input/RF.txt");
ofstream dCacheOutput("output/ODCache.txt");
ofstream rfOutput("output/ORF.txt");
ofstream stats("output/Output.txt");

int instr = 0;
int arith = 0;
int logic = 0;
int load = 0;
int store = 0;
int branch = 0;
int hlt_cnt=0;

PC pc;
ICache icache(icaches);
DCache dcache(dcaches);
ALU alu;
RegisterFile ARF(rf);
RenameRegisterFile RRF;
instbufelem FetchDecodeBuf[8];
dispbufelem DispatchBuf[256];
ResStElem intRS[8];
ResStElem branchRS[8];
ResStElem loadStoreRS[8];
vector<ROBelem> ROB;

flag halt=false;
flag stop=false;

bool operator==(Register a, Register b)
{
    bool ans = true;
    ans = ans && (a.tag == b.tag);
    ans = ans && (a.busy == b.busy);
    ans = ans && (a.valid == b.valid);
    return ans;
}

bool operator==(const dispbufelem &x, const dispbufelem &y)
{
    bool ans = true;
    ans = ans && (x.instruction == y.instruction);
    return ans;
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
        FetchDecodeBuf[free].instruction = icache.request(addr);
        FetchDecodeBuf[free].addr = addr;
        pc.increment();
        //cout<<"Fetch: "<<hex<<(int)FetchDecodeBuf[free].instruction<<endl;
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
    return -1;
}

void Decode()
{
    if(halt) return;

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
        DispatchBuf[free].instruction = instruction;
        DispatchBuf[free].addr = FetchDecodeBuf[instfree].addr;

        int16 opcode = instruction >> 12;
        DispatchBuf[free].opcode = opcode;
        DispatchBuf[free].subop = (opcode & 0x3);

        //cout<<"Decode: "<<hex<<(int)instruction<<endl;
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
                    DispatchBuf[free].src2 = ARF.R[r1];
                }
                else
                {
                    int tag = ARF.R[r1].tag;
                    DispatchBuf[free].src2.tag = tag;
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
            halt = true;

            int offset = (instruction & 0x0ff0) >> 4;

            DispatchBuf[free].jump = true;
            DispatchBuf[free].offset = offset;
        }

        //beqz
        if(opcode == 11)
        {
            DispatchBuf[free].beqz = true;

            int offset = (instruction & 0x00ff);
            int8 r1 = (instruction & 0x0f00)>>8;

            DispatchBuf[free].offset = offset;

            if(!ARF.R[r1].busy)
            {
                DispatchBuf[free].src1 = ARF.R[r1];
            }
            else
            {
                int tag = ARF.R[r1].tag;
                DispatchBuf[free].src1.tag = tag;
            }

            halt = true;
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
            if(DispatchBuf[i].arithmatic || DispatchBuf[i].logical || DispatchBuf[i].halt)
            {
                flag passed=false;
                for(int j=0;j<8;j++)
                {
                    if(!intRS[j].busy)
                    {
                        intRS[j].elem = DispatchBuf[i];
                        intRS[j].busy = true;
                        intRS[j].ready = false;
                        passed = true;
                        ROB.push_back(ROBelem(DispatchBuf[i]));
                        //cout<<"Dispatching "<<hex<<DispatchBuf[i].instruction<<endl;
                        break;
                    }
                }

                if(passed)
                {
                    if(DispatchBuf[i].halt) halt=true;
                    DispatchBuf[i].busy = false;
                }
            }

            if(DispatchBuf[i].beqz || DispatchBuf[i].jump)
            {
                flag passed=false;
                for(int j=0;j<8;j++)
                {
                    if(!branchRS[j].busy)
                    {
                        branchRS[j].elem = DispatchBuf[i];
                        branchRS[j].busy = true;
                        branchRS[j].ready = false;
                        passed = true;
                        ROB.push_back(ROBelem(DispatchBuf[i]));
                        //cout<<"Dispatching "<<DispatchBuf[i].instruction<<endl;
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
                    if(!loadStoreRS[j].busy)
                    {
                        loadStoreRS[j].elem = DispatchBuf[i];
                        loadStoreRS[j].busy = true;
                        loadStoreRS[j].ready = false;
                        passed = true;
                        ROB.push_back(ROBelem(DispatchBuf[i]));
                        // //cout<<"Dispatching "<<DispatchBuf[i].instruction<<"in loadStore"<<endl;
                        break;
                    }
                }

                if(passed)
                {
                    DispatchBuf[i].busy = false;
                }
            }
        }
    }
}

int findInROB(dispbufelem inst)
{
    //cout<<"findROB"<<endl;
    for(int i=0;i<ROB.size();i++)
    {
        if(ROB[i].elem == inst) 
        {
            //cout<<"found "<<i<<endl;
            return i;
        }
    }
    return -1;
}

void intexecute()
{
    int ready=-1;
    for(int i=0;i<8;i++)
    {
        if(intRS[i].busy && intRS[i].ready)
        {
            ready=i;
            break;
        }
    }

    //cout<<"int ready "<<dec<<ready<<endl;

    if(ready!=-1)
    {
        intRS[ready].busy=false;
        //cout<<"intexecute :"<<hex<<intRS[ready].elem.instruction<<endl;
        dispbufelem inst = intRS[ready].elem;
        
        if(inst.arithmatic)
        {
            int result;
            
            if(inst.subop==0) result=alu.adder(inst.src1.read(),inst.src2.read(),0);
            if(inst.subop==1) result=alu.adder(inst.src1.read(),inst.src2.read(),1);
            if(inst.subop==3) result=alu.adder(inst.src1.read(),1,0);
            if(inst.subop==2) result=alu.MUL(inst.src1.read(),inst.src2.read());

            int tag = inst.dest;
            RRF.R[tag].write(result);
            RRF.R[tag].valid = true;
        }

        if(inst.logical)
        {
            //cout<<"logic "<<inst.subop<<endl;
            int result;
            
            if(inst.subop==0) result=alu.AND(inst.src1.read(),inst.src2.read());
            if(inst.subop==1) result=alu.OR(inst.src1.read(),inst.src2.read());
            if(inst.subop==2) result=alu.NOT(inst.src2.read());
            if(inst.subop==3) result=alu.XOR(inst.src1.read(),inst.src2.read());

            int tag = inst.dest;
            RRF.R[tag].write(result);
            RRF.R[tag].valid = true;
        }

        int indInROB = findInROB(inst);
        ROB[indInROB].finished = true;
        
    }
}

dispbufelem lsbuff[3];
int addressbuf = -1;
int destaddressbuf = -1;

int sign_Extension4(int x)
{
    if(x>7)
        return x-16;
    else return x;
}

void addressGeneration(dispbufelem inst)
{
    if(inst.busy)
    {
        //cout<<"Load/Store 1 :"<<hex<<inst.instruction<<endl;
        int src2 = inst.src2.read();
        int offset = inst.offset;
        int result = alu.adder(src2,sign_Extension4(offset),0);
        addressbuf = result;
    }
}

void addressTranslation(dispbufelem inst)
{

    if(addressbuf!=-1 && inst.busy)
    {
        //cout<<"Load/Store 2 :"<<hex<<inst.instruction<<endl;
        if(inst.load)
        {
            destaddressbuf = dcache.request(addressbuf);
        }
        else
        { 
            destaddressbuf = -1;
            dcache.write(addressbuf,inst.src1.read());

            int indInROB = findInROB(inst);
            ROB[indInROB].finished = true;
        }    
    }
}

void loading(dispbufelem inst)
{
    if(destaddressbuf!=-1 && inst.busy)
    {
        //cout<<"Load/Store 3 :"<<hex<<inst.instruction<<endl;
        if(inst.load)
        {
            int tag = inst.dest;
            RRF.R[tag].write(destaddressbuf);
            RRF.R[tag].valid = true;

            int indInROB = findInROB(inst);
            ROB[indInROB].finished = true;
        }
    }
}

void loadstoreexecute()
{
    int ready=-1;
    for(int i=0;i<8;i++)
    {
        if(loadStoreRS[i].busy && loadStoreRS[i].ready)
        {
            ready=i;
            break;
        }
    }
    // //cout<<"ready "<<ready<<endl;
    lsbuff[2] = lsbuff[1];
    lsbuff[1] = lsbuff[0];
    dispbufelem d;
    if(ready==-1)
    {
        lsbuff[0] = d; 
    }
    else
    {
        loadStoreRS[ready].busy=false;
        lsbuff[0] = loadStoreRS[ready].elem;
    }
    loading(lsbuff[2]);
    addressTranslation(lsbuff[1]);
    addressGeneration(lsbuff[0]);
}

int sign_Extension8(int x)
{
    if(x>127)
        return x-256;
    else
        return x;
}


void branchexecute()
{
    int ready=-1;
    for(int i=0;i<8;i++)
    {
        if(branchRS[i].busy && branchRS[i].ready)
        {
            ready=i;
            break;
        }
    }

    if(ready!=-1)
    {
        pc.write(branchRS[ready].elem.addr);
        pc.increment();

        dispbufelem d = branchRS[ready].elem;

        //cout<<"Branchexecute : "<<hex<<d.instruction<<endl;

        if(d.jump)
        {
            pc.write(alu.adder(pc.read(),sign_Extension8(d.offset)<<1,0));
        }

        if(d.beqz)
        {
            bool x = alu.BEQZ(d.src1.read());
            if(x)
            {
                pc.write(alu.adder(pc.read(),sign_Extension8(d.offset) << 1,0));
            }
        }

        int indInROB = findInROB(d);
        ROB[indInROB].finished = true;
    }
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
        if(intRS[i].busy && !intRS[i].ready)
        {
            dispbufelem x = intRS[i].elem;
            bool ready=true;

            if(x.opcode != 6)
            {
                if(x.src1.tag!=-1)
                {
                    if(!RRF.R[x.src1.tag].valid) ready=false;
                    else
                    {
                        intRS[i].elem.src1.write(RRF.R[x.src1.tag].read());
                    }
                }
            }

            if(x.opcode != 3)
            {
                if(x.src2.tag!=-1)
                {
                    if(!RRF.R[x.src2.tag].valid) ready=false;
                    else
                    {
                        intRS[i].elem.src2.write(RRF.R[x.src2.tag].read());
                    }
                }
            }

            intRS[i].ready = ready;
            if(ready)
            {
                //cout<<"RSDispatch :"<<intRS[i].elem.instruction<<endl;
            }
        }
    }

    for(int i=0;i<8;i++)
    {
        if(branchRS[i].busy && !branchRS[i].ready)
        {
            dispbufelem x = branchRS[i].elem;
            bool ready=true;

            if(x.beqz)
            {
                if(x.src1.tag!=-1)
                {
                    if(!RRF.R[x.src1.tag].valid) ready=false;
                    else
                    {
                        branchRS[i].elem.src1.write(RRF.R[x.src1.tag].read());
                    }
                }
            }

            branchRS[i].ready = ready;
        }
    }

    for(int i=0;i<8;i++)
    {
        if(loadStoreRS[i].busy && !loadStoreRS[i].ready)
        {
            dispbufelem x = loadStoreRS[i].elem;
            bool ready=true;

            if(x.store)
            {
                if(x.src1.tag!=-1)
                {
                    if(!RRF.R[x.src1.tag].valid) ready=false;
                    else
                    {
                        loadStoreRS[i].elem.src1.write(RRF.R[x.src1.tag].read());
                    }
                }
            }

            if(x.src2.tag!=-1)
            {
                if(!RRF.R[x.src2.tag].valid) ready=false;
                else
                    {
                        loadStoreRS[i].elem.src2.write(RRF.R[x.src2.tag].read());
                    }
            }

            loadStoreRS[i].ready = ready;
            if(ready)
            {
            //cout<<"RSdispatch : "<<hex<<x.instruction<<" "<<dec<<x.src1.tag<<" "<<x.src2.tag<<endl;

            }
        }
    }
}

void flushAll()
{
    ROB.clear();
    for(int i=0;i<8;i++)
    {
        intRS[i].busy = false;
        branchRS[i].busy = false;
        loadStoreRS[i].busy = false;
        FetchDecodeBuf[i].busy = false;
    }

    for(int i=0;i<256;i++)
    {
        DispatchBuf[i].busy = false;
    }
}

void popROB()
{
    if(ROB.empty()) return;

    ROBelem x = ROB.front();

    if(x.finished)
    {
        ROB.erase(ROB.begin());

        dispbufelem d = x.elem;
        //cout<<"ROB "<<hex<<d.instruction<<endl;

        if(d.beqz || d.jump)
        {
            flushAll();
            // //cout<<"changed halt"<<endl;
            halt = false;
        }

        instr++;

        if(d.arithmatic) arith++;
        else if(d.logical) logic++;
        else if(d.load) load++;
        else if(d.store) store++;
        else if(d.beqz || d.jump) branch++;
        else if(d.halt) hlt_cnt++;

        if(d.halt) 
        {
            stop=true;
            return;
        }

        if(d.beqz || d.store || d.jump)
        {
            return;
        }

        int val = RRF.R[d.dest].read();
        RRF.R[d.dest].busy = false;
        
        for(int i=0;i<16;i++)
        {
            if(ARF.R[i].tag==d.dest)
            {
                ARF.R[i].tag = -1;
                ARF.R[i].valid = true;
                ARF.R[i].busy = false;
                ARF.R[i].write(val);
            }
        }
    }
}

void printROB()
{
    //cout<<"ROB: ";
    for(auto i:ROB)
    {
        if(i.finished) 
        {
            //cout<<"1 ";
        }
        else 
        {
            //cout<<"0 ";
        }
    }
    //cout<<endl;
}

void execute_cycle()
{
    popROB();
    RSdispatch();
    execute();
    Dispatch();
    Decode();
    Fetch();

}

int main()
{
    int cc=0;
    while(!stop)
    //for(int i=0;i<20;i++)
    {
        //cout<<cc<<"------------------------------------------"<<endl;
        execute_cycle();
        //cout<<"--------------------------------------------"<<endl;
        printROB();
        cc++;
    }

    dcache.output(dCacheOutput);
    ARF.output(rfOutput);

    stats<<"Total instructions              : "<<instr<<endl;
    stats<<"Total Arithmatic instructions   : "<<arith<<endl;
    stats<<"Total Logical instructions      : "<<logic<<endl;
    stats<<"Total Load instructions         : "<<load<<endl;
    stats<<"Total Store instructions        : "<<store<<endl;
    stats<<"Total Branch instructions       : "<<branch<<endl;
    stats<<"Total Halt instructions         : "<<hlt_cnt<<endl;
    stats<<"Total cycles                    : "<<cc<<endl;
    stats<<"Total cycles per instruction    : "<<(double)cc/instr<<endl;
}