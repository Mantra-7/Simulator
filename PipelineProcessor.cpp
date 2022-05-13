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
    flag valid=true;
    flag dh=false;
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

class IFIDBuffer{
public:
    int16 instruction;
    flag valid;
    IFIDBuffer() : valid(false), instruction(0xf000) {}
};

class IFModule{
public:
    PC &pc;
    Register16 &IR;
    ICache &I$;
    IFIDBuffer &ifidbuf;
    flag stall;
    flag &branch_resolved;
    int &dataHaz;
    int &prevDataHaz;
    flag &stop;
    IFModule();
    IFModule(PC &pc, ICache &icache, Register16 &ir, IFIDBuffer &ifidbuf, flag &branch_resolved, int &dataHaz, int &prevDataHaz, flag &stop) : pc(pc), I$(icache), IR(ir) , ifidbuf(ifidbuf) , branch_resolved(branch_resolved) , dataHaz(dataHaz) , prevDataHaz(prevDataHaz) , stop(stop) 
    {
        stall = false;
        prevDataHaz = false;
    }
    void run();
};

class IDEXBuffer{
public:
    int opcode;
    
    flag valid;
    flag arithmatic;
    flag logical;
    flag load;
    flag store;
    flag beqz;
    flag jump;
    flag halt;

    int8 src1;
    int8 src2;
    int8 dest;
    int subop;
    int8 jump_addr;
    int offset;
};

class IDRFModule{
public:
    IDRFModule();
    IDRFModule(RegisterFile &rf, DCache &D$, IFIDBuffer &ifidbuf, IDEXBuffer &idexbuf, flag &branch_resolved, int &dataHaz, flag &stop) : RF(rf), D$(D$) , ifidbuf(ifidbuf) , idexbuf(idexbuf) , branch_resolved(branch_resolved) , dataHaz(dataHaz) , stop(stop)
    {
        stall = false;
    }
    RegisterFile &RF;
    DCache &D$;
    IFIDBuffer &ifidbuf;
    IDEXBuffer &idexbuf;
    flag stall;
    flag &branch_resolved;
    int &dataHaz;
    flag &stop;
    void run();
};

class EXMEBuffer{
public: 
    flag valid;
    flag halt;
    flag writeToRegister;
    flag load;
    flag store;
    flag jump;
    int alu_result;
    int8 dest;
    int8 store_src;
    int16 getInstruction();
};

class EXModule{
public: 
    EXModule();
    EXModule(ALU &alu, PC &pc, RegisterFile &rf, IDEXBuffer &idexbuf, EXMEBuffer &exmebuf, flag &branch_resolved) : alu(alu), pc(pc), RF(rf), idexbuf(idexbuf), exmebuf(exmebuf) , branch_resolved(branch_resolved) {}
    ALU &alu;
    PC &pc;
    RegisterFile &RF;
    IDEXBuffer &idexbuf;
    EXMEBuffer &exmebuf;

    flag stall;
    flag &branch_resolved;
    void run();
};

class MEWBBuffer
{
public:
    flag valid;
    flag load;
    flag store;
    flag writeToReg;
    flag halt;
    int alu_result;
    int8 dest;
};

class MEModule{
public:
    MEModule();
    MEModule(DCache &D$, EXMEBuffer &exmebuf, MEWBBuffer &mewbbuf, Register &LMD) : D$(D$), exmebuf(exmebuf), mewbbuf(mewbbuf), LMD(LMD){}
    DCache &D$;
    EXMEBuffer &exmebuf;
    MEWBBuffer &mewbbuf;
    Register &LMD;
    flag stall;
    void run();
};

class WBModule{
public:
    flag valid;
    flag stall;
    int &dataHaz;
    flag &halt;
    WBModule();
    WBModule(RegisterFile &rf, MEWBBuffer &mewbbuf, Register &lmd, int &dataHaz, flag &halt) : RF(rf), mewbbuf(mewbbuf) , LMD(lmd) , dataHaz(dataHaz) , halt(halt) {}
    RegisterFile &RF;
    MEWBBuffer &mewbbuf;
    Register &LMD;
    int alu_result;
    void run();
};

class Processor{
public:
    ICache I$;
    DCache D$;
    RegisterFile rf;
    PC pc;
    Register16 IR;
    Register LMD1;
    Register LMD2;

    IFModule IF;
    IFIDBuffer IFID1;
    IFIDBuffer IFID2;
    IDRFModule IDRF;
    IDEXBuffer IDEX1;
    IDEXBuffer IDEX2;
    EXModule EX;
    EXMEBuffer EXME1;
    EXMEBuffer EXME2;
    MEModule MEM;
    MEWBBuffer MEWB1;
    MEWBBuffer MEWB2;
    WBModule WB;

    ALU alu;
    flag halt;
    flag prevBranchRes;
    flag branch_resolved;
    int dataHaz;
    int prevDataHaz;
    flag stop;

    Processor(ifstream &icache, ifstream &dcache, ifstream &rfIn) : IF(pc, I$, IR, IFID1, branch_resolved, dataHaz, prevDataHaz, stop) , IDRF(rf, D$, IFID2, IDEX1, branch_resolved, dataHaz, stop), EX(alu, pc, rf, IDEX2, EXME1, branch_resolved), MEM(D$, EXME2, MEWB1, LMD1), WB(rf, MEWB2, LMD2, dataHaz, halt) , I$(icache), D$(dcache) , rf(rfIn) 
    {
        IFID2.valid = false;
        IDEX2.valid = false;
        EXME2.valid = false;
        MEWB2.valid = false;
        pc.write(0);
        halt = false;
        prevBranchRes = false;
        branch_resolved = true;
        dataHaz = false;
        prevDataHaz = false;
        stop = false;
    }

    void run();
    void flushIFID();
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
int x=0;
void PC::decrement()
{
    m_addr-=2;
}

void IFModule::run() 
{
    //cout<<"br "<<branch_resolved<<" dh "<<dataHaz<<endl;
    if(stop || !branch_resolved || dataHaz)
    {
        prevDataHaz = dataHaz;
        ifidbuf.valid = false;
        stall = true;
        return;
    }
    
    stall = false;
    ifidbuf.valid = true;
    int16 instruction = I$.request(pc.read());
    //cout << "IFModule: " << hex << instruction << endl;
    IR.write(instruction);
    ifidbuf.instruction = instruction;
    pc.increment();
}

void IDRFModule::run()
{
    if((stop || !ifidbuf.valid || !branch_resolved || dataHaz))
    {
        stall = true;
        idexbuf.valid = false;
        return;
    }

    idexbuf.arithmatic = false;
    idexbuf.logical = false;
    idexbuf.load = false;
    idexbuf.store = false;
    idexbuf.beqz = false;
    idexbuf.jump = false;
    idexbuf.halt = false;
    idexbuf.valid = true;

    int16 instruction = ifidbuf.instruction;
    //cout << "IDRFModule: " << hex << instruction << endl;
    int16 opcode = instruction >> 12;
    idexbuf.opcode = opcode;

    if(opcode<4)
    {
        idexbuf.arithmatic = true;

        int8 r3 = (instruction & 0x0f00)>>8;
        int8 r1 = (instruction & 0x00f0)>>4;
        int8 r2 = (instruction & 0x000f);
        // increment
        if(opcode == 3) 
        {
            if(RF.R[r3].valid)
            {
                
                idexbuf.src1 = RF.read(r3);
                idexbuf.dest = r3;
                RF.R[r3].valid = false;
                idexbuf.subop = opcode & 0x03;
            }
            else
            {
                stall = true;
                idexbuf.valid = false;
                dataHaz++;
                RF.R[r3].dh = true;
            }
        }
        // add, mul, sub
        else if(RF.R[r1].valid && RF.R[r2].valid)
        {
            idexbuf.src1 = RF.read(r1);
            idexbuf.src2 = RF.read(r2);
            idexbuf.dest = r3;
            RF.R[r3].valid = false;
            idexbuf.subop = opcode & 0x03;
        }
        else
        {
            stall = true;
            idexbuf.valid = false;
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

    else if(opcode<8)
    {
        idexbuf.logical = true;

        int8 r3 = (instruction & 0x0f00)>>8;
        int8 r1 = (instruction & 0x00f0)>>4;
        int8 r2 = (instruction & 0x000f);

        // not
        if(opcode==6)
        {
            if(RF.R[r1].valid)
            {
                idexbuf.src1 = RF.read(r1);
                idexbuf.dest = r3;
                RF.R[r3].valid = false;
                idexbuf.subop = opcode & 0x03;
            }
            else
            {
                stall = true;
                idexbuf.valid = false;
                RF.R[r1].dh=true;
                dataHaz++;
            }
            
        }

        // and , or, xor
        else if(RF.R[r1].valid && RF.R[r2].valid)
        {
            idexbuf.src1 = RF.read(r1);
            idexbuf.src2 = RF.read(r2);
            idexbuf.dest = r3;
            RF.R[r3].valid = false;
            idexbuf.subop = opcode & 0x03;
        }
        else
        {
            stall = true;
            idexbuf.valid = false;
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
        idexbuf.load = true;
        int8 r1 = (instruction & 0x0f00)>>8;
        int8 r2 = (instruction & 0x00f0)>>4;
        int8 x = (instruction & 0x000f);
        if(RF.R[r2].valid)
        {
            idexbuf.src1 = r1;
            idexbuf.src2 = RF.read(r2);
            idexbuf.offset = x;
            RF.R[r1].valid = false;
        }
        else
        {
            stall = true;
            idexbuf.valid = false;
            RF.R[r2].dh = true;
            dataHaz++;
        }   
    }

    // store
    if(opcode == 9)
    {
        idexbuf.store = true;
        int8 r1 = (instruction & 0x0f00)>>8;
        int8 r2 = (instruction & 0x00f0)>>4;
        int8 x = (instruction & 0x000f);

        if(RF.R[r1].valid && RF.R[r2].valid)
        {
            idexbuf.src2 = RF.read(r2);
            idexbuf.src1 = RF.read(r1);
            idexbuf.offset = x;
        }
        else
        {
            stall = true;
            idexbuf.valid = false;
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
        idexbuf.jump = true;
        idexbuf.jump_addr = (instruction & 0x0ff0)>>4;
    }

    //beqz
    if(opcode == 11)
    {
        idexbuf.beqz = true;
        int8 r1 = (instruction & 0x0f00)>>8;
        if(!RF.R[r1].valid)
        {
            stall = true;
            idexbuf.valid = false;
            RF.R[r1].dh = true;
            dataHaz++;
        }
        else
        {
            branch_resolved = false;
            idexbuf.src1 = RF.read(r1);
            idexbuf.jump_addr = instruction & 0x00ff;
        }
    }

    if(opcode==15)
    {
        idexbuf.halt = true;
        stop = true;
    }
}

int sign_Extension8(int x)
{
    if(x>127)
        return x-256;
    else
        return x;
}

int sign_Extension4(int x)
{
    if(x>7)
        return x-16;
    else return x;
}

void EXModule::run()
{
    if(!idexbuf.valid)
    {
        stall = true;
        exmebuf.valid = false;
        return;
    }
    
    stall = false;
    exmebuf.valid = true;
    exmebuf.load = false;
    exmebuf.store = false;
    exmebuf.halt = idexbuf.halt;
    exmebuf.writeToRegister = false;

    if(idexbuf.arithmatic)
    {
        //cout<<"EX: Arithmatic"<<endl;
        exmebuf.writeToRegister = true;
        exmebuf.dest = idexbuf.dest;
        switch(idexbuf.subop)
        {
            case 0:
                exmebuf.alu_result = alu.adder(idexbuf.src1, idexbuf.src2, false);
                break;
            case 1:
                exmebuf.alu_result = alu.adder(idexbuf.src1, idexbuf.src2, true);
                break;
            case 2:
                exmebuf.alu_result = alu.MUL(idexbuf.src1, idexbuf.src2);
                break;
            case 3:
                exmebuf.alu_result = alu.adder(idexbuf.src1, 1, false);
                break;
        }

        return;
    }

    if(idexbuf.logical)
    {
        //cout<<"EX: Logical"<<endl;
        exmebuf.writeToRegister = true;
        exmebuf.dest = idexbuf.dest;
        switch(idexbuf.subop)
        {
            case 0:
                exmebuf.alu_result = alu.AND(idexbuf.src1, idexbuf.src2);
                break;
            case 1:
                exmebuf.alu_result = alu.OR(idexbuf.src1, idexbuf.src2);
                break;
            case 2:
                exmebuf.alu_result = alu.NOT(idexbuf.src1);
                break;
            case 3:
                exmebuf.alu_result = alu.XOR(idexbuf.src1, idexbuf.src2);
                break;
        }
        return;
    }
        
    if(idexbuf.load)
    {
        //cout<<"EX: Load"<<endl;
        exmebuf.load = true;
        exmebuf.dest = idexbuf.src1;
        exmebuf.alu_result = alu.adder(idexbuf.src2,sign_Extension4(idexbuf.offset),false);
        return;
    }

    if(idexbuf.store)
    {
        //cout<<"EX: Store"<<endl;
        exmebuf.store = true;
        exmebuf.store_src = idexbuf.src1;
        exmebuf.alu_result = alu.adder(idexbuf.src2,sign_Extension4(idexbuf.offset),false);
        exmebuf.dest = exmebuf.alu_result;
        return;
    }

    if(idexbuf.jump)
    {
        //cout<<"EX: Jump"<<endl;
        exmebuf.jump = true;
        exmebuf.alu_result = alu.adder(pc.read(),sign_Extension8(idexbuf.jump_addr)<<1,false);
        pc.write(exmebuf.alu_result);
        branch_resolved = true;
        return;
    }

    if(idexbuf.beqz)
    {
        //cout<<"EX: BEQZ"<<endl;
        exmebuf.alu_result = alu.BEQZ(idexbuf.src1);
        if(exmebuf.alu_result)
        {
            exmebuf.alu_result = alu.adder(pc.read(),sign_Extension8(idexbuf.jump_addr) << 1,false);
            pc.write(exmebuf.alu_result);
            idexbuf.valid = false;
        }
        branch_resolved = true;
        return;
    }
}

void MEModule::run()
{
    if(!exmebuf.valid)
    {
        stall = true;
        mewbbuf.valid = false;
        return;
    }
    //cout<<"MEModule: "<<endl;

    mewbbuf.alu_result = exmebuf.alu_result;
    mewbbuf.writeToReg = exmebuf.writeToRegister;
    mewbbuf.dest = exmebuf.dest;
    mewbbuf.valid = true;
    stall = false;
    mewbbuf.load = false;
    mewbbuf.halt = exmebuf.halt;

    if(exmebuf.load)
    {
        //cout<<"MEM: Loading from address "<<exmebuf.alu_result<<endl;
        int val = D$.request(exmebuf.alu_result);
        //cout<<"val is "<<val<<endl;
        LMD.write(val); 
        mewbbuf.load = true;
    }
    
    if(exmebuf.store)
    {
        //cout<<"MEM: Storing to address "<<exmebuf.alu_result<<endl;
        D$.write(exmebuf.alu_result, exmebuf.store_src);
        mewbbuf.store = true;
    }
}

void WBModule::run()
{
    if(!mewbbuf.valid)
    {
        stall = true;
        return;
    }

    if(mewbbuf.halt)
    {
        halt = true;
        return;
    }
    
    stall = false;
    if(mewbbuf.load)
    {
        if(RF.R[mewbbuf.dest].valid == false)
        {
            RF.R[mewbbuf.dest].valid = true;
            if(RF.R[mewbbuf.dest].dh==true)
            {
                RF.R[mewbbuf.dest].dh=false;
                dataHaz--;
            }
            
        }
        //cout<<"WB: writing from load at "<<(int)mewbbuf.dest<<" val is "<<(int)LMD.read()<<endl;
        RF.write(mewbbuf.dest, LMD.read());
    }
    
    if(mewbbuf.writeToReg)
    {
        if(RF.R[mewbbuf.dest].valid == false)
        {
            RF.R[mewbbuf.dest].valid = true;
            if(RF.R[mewbbuf.dest].dh==true)
            {
                RF.R[mewbbuf.dest].dh=false;
                dataHaz--;
            }
        }
        //cout<<"WB: Writing from alu at "<<(int)mewbbuf.dest<<" val is "<<mewbbuf.alu_result<<endl;
        RF.write(mewbbuf.dest, mewbbuf.alu_result);
    }
}

void Processor::run()
{
    IF.run();
    IDRF.run();
    if(prevBranchRes && !branch_resolved)
    {
        //cout<<"flush"<<endl;
        flushIFID();
    }
    prevBranchRes = branch_resolved;
    EX.run();
    MEM.run();
    int prevdh = dataHaz;
    WB.run();
    //cout<<"datahaz bef is "<<dataHaz<<endl;
    if(prevdh && !dataHaz)
    {
        IDRF.run();
        IFID2 = IFID1;
        IFID2.valid = true;
    }

    if(!dataHaz && !prevdh) IFID2 = IFID1;
    IDEX2 = IDEX1;
    EXME2 = EXME1;
    MEWB2 = MEWB1;
    LMD2 = LMD1;
}

void Processor::flushIFID()
{
    IFID1.valid = false;
    pc.decrement();
}

int instructions = 0;
int arith = 0;
int logic = 0;
int dataa = 0;
int control = 0;
int halt = 0;
int stall = -4;
int datastall = 0;
int cc=1;

void update_stats(Processor pc, IDEXBuffer idex)
{
    if(!pc.EX.stall) 
    {
        instructions++;
        if(idex.arithmatic) arith++;
        if(idex.logical) logic++;
        if(idex.load)  dataa++;
        if(idex.store) dataa++;
        if(idex.jump) control++;
        if(idex.beqz) control++;
        if(idex.halt) halt++;
    }
    else 
    {
        //cout<<"stall here: "<<cc-1<<endl;
        stall++;
    }

    if(pc.dataHaz) 
    {
        //cout<<"datahaz in "<<pc.dataHaz<<endl;
        datastall++;
    }
}

void print_stats(ofstream &fout)
{
    fout<<dec<<"Total number of instructions executed: "<<instructions<<endl;
    fout<<dec<<"Number of instructions in each class"<<endl;
    fout<<dec<<"Arithmetic instructions              : "<<arith<<endl;
    fout<<dec<<"Logical instructions                 : "<<logic<<endl;
    fout<<dec<<"Data instructions                    : "<<dataa<<endl;
    fout<<dec<<"Control instructions                 : "<<control<<endl;
    fout<<dec<<"Halt instructions                    : "<<halt<<endl;
    fout<<dec<<"Cycles Per Instruction               : "<<(float)(cc-1)/instructions<<endl;
    fout<<dec<<"Total number of stalls               : "<<stall<<endl;
    fout<<dec<<"Data stalls (RAW)                    : "<<datastall<<endl;
    fout<<dec<<"Control stalls                       : "<<stall-datastall<<endl;
}

int main()
{
    ifstream icache("input/ICache.txt");
    ifstream dcache("input/DCache.txt");
    ifstream rf("input/RF.txt");
    ofstream dCacheOutput("output/ODCache.txt");
    ofstream rfOutput("output/ORF.txt");
    ofstream stats("output/Output.txt");

    Processor processor(icache, dcache, rf);

    while(!processor.halt)
    {
        IDEXBuffer id = processor.EX.idexbuf;
        cc++;
        //cout<<cc++<<endl<<"-------------------------------------------"<<endl;
        processor.run();
        update_stats(processor, id);
        //cout<<"-------------------------------------------"<<endl;
    }

    processor.D$.output(dCacheOutput);
    processor.rf.output(rfOutput);
    print_stats(stats);
}
