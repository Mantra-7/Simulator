#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <iostream>
#include <fstream>
using namespace std;

#define BLOCK_SIZE 4
#define NUM_SETS 64
#define NUM_REGS 16

typedef unsigned char int8;
typedef unsigned short int16;
typedef bool flag;

class Register
{
    int8 m_val;
public:
    flag valid;
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
    RegisterFile();
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
    flag BNEQ(int X);
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
    flag &dataHaz;
    flag &prevDataHaz;
    IFModule();
    IFModule(PC &pc, ICache &icache, Register16 &ir, IFIDBuffer &ifidbuf, flag &branch_resolved, flag &dataHaz, flag &prevDataHaz) : pc(pc), I$(icache), IR(ir) , ifidbuf(ifidbuf) , branch_resolved(branch_resolved) , dataHaz(dataHaz) , prevDataHaz(prevDataHaz)
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
    flag bneq;
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
    IDRFModule(RegisterFile &rf, DCache &D$, IFIDBuffer &ifidbuf, IDEXBuffer &idexbuf, flag &HALT, flag &branch_resolved, flag &dataHaz) : RF(rf), D$(D$) , ifidbuf(ifidbuf) , idexbuf(idexbuf) , halt(HALT) , branch_resolved(branch_resolved) , dataHaz(dataHaz) 
    {
        stall = false;
    }
    RegisterFile &RF;
    DCache &D$;
    IFIDBuffer &ifidbuf;
    IDEXBuffer &idexbuf;
    flag &halt;
    flag stall;
    flag &branch_resolved;
    flag &dataHaz;
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
    flag &dataHaz;
    WBModule();
    WBModule(RegisterFile &rf, MEWBBuffer &mewbbuf, Register &lmd, flag &dataHaz) : RF(rf), mewbbuf(mewbbuf) , LMD(lmd) , dataHaz(dataHaz) {}
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
    Register LMD;

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
    flag branch_resolved;
    flag dataHaz;
    flag prevDataHaz;

    Processor(ifstream &icache, ifstream &dcache) : IF(pc, I$, IR, IFID1, branch_resolved, dataHaz, prevDataHaz) , IDRF(rf, D$, IFID2, IDEX1, halt, branch_resolved, dataHaz), EX(alu, pc, rf, IDEX2, EXME1, branch_resolved), MEM(D$, EXME2, MEWB1, LMD), WB(rf, MEWB2, LMD, dataHaz) , I$(icache), D$(dcache)
    {
        pc.write(0);
        halt = false;
        branch_resolved = true;
        dataHaz = false;
        prevDataHaz = false;
    }
    void run();
};

#endif