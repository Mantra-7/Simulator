#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <iostream>
#include <fstream>
using namespace std;

#define BLOCK_SIZE 4
#define NUM_SETS 64
#define NUM_REGS 16

typedef int8_t int8;
typedef int16_t int16;
typedef bool flag;

class Register
{
    int8 m_val;
public:
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
    Register R[NUM_REGS];
public:
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
    int16 request(int8 addr);
    void write(int8 addr, int16 val);
};

class DCache{
    Block m_data[NUM_SETS];
public:
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

class IFModule{
    PC &pc;
    Register16 &IR;
    ICache &I$;
    IFIDBuffer &ifidbuf;
    flag stall;
public:
    IFModule(PC &pc, ICache &icache, Register16 &ir, IFIDBuffer &ifidbuf) : pc(pc), I$(icache), IR(ir) , ifidbuf(ifidbuf)
    {
        stall = false;
    }
    void IF();
};

class IFIDBuffer{
    int16 instruction;
public:
    flag valid;
    IFIDBuffer() : valid(false), instruction(0xf000) {}
    int16 getInstruction();
    void set(int16 instruction);
    void invalidate();
};

class IDRFModule{
public:
    IDRFModule(RegisterFile &rf, DCache &D$, IFIDBuffer &ifidbuf, IDEXBuffer &idexbuf, flag &HALT) : RF(rf), D$(D$) , ifidbuf(ifidbuf) , idexbuf(idexbuf) , halt(HALT)
    {
        stall = false;
    }
    RegisterFile &RF;
    DCache &D$;
    IFIDBuffer &ifidbuf;
    IDEXBuffer &idexbuf;
    flag &halt;
    flag stall;
    void IDRF();
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

class EXModule{
public: 
    EXModule(ALU &alu, PC &pc, RegisterFile &rf, IDEXBuffer &idexbuf, EXMEBuffer &exmebuf) : alu(alu), pc(pc), RF(rf), idexbuf(idexbuf), exmebuf(exmebuf) {}
    ALU &alu;
    PC &pc;
    RegisterFile &RF;
    IDEXBuffer &idexbuf;
    EXMEBuffer &exmebuf;

    flag stall;
    void EX();
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
};

class MEModule{
public:
    MEModule(DCache &D$, EXMEBuffer &exmebuf, MEWBBuffer &mewbbuf, Register &LMD) : D$(D$), exmebuf(exmebuf), mewbbuf(mewbbuf), LMD(LMD){}
    DCache &D$;
    EXMEBuffer &exmebuf;
    MEWBBuffer &mewbbuf;
    Register &LMD;
    flag stall;
    void ME();
};

class MEWBBuffer
{
public:
    flag valid;
    flag load;
    flag store;
    flag writeToReg;
    flag halt;
    //flag alu_inst;
    int alu_result;
    int8 dest;
    int valToWrite;
};

class WBModule{
public:
    flag valid;
    flag stall;
    WBModule(RegisterFile &rf, MEWBBuffer &mewbbuf, Register &lmd) : RF(rf), mewbbuf(mewbbuf) , LMD(lmd) {}
    RegisterFile &RF;
    MEWBBuffer &mewbbuf;
    Register &LMD;
    int alu_result;
    void WB();
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
    EXMEBuffer EM1;
    EXMEBuffer EM2;
    MEModule MEM;
    MEWBBuffer MW1;
    MEWBBuffer MW2;
    WBModule WB;

    flag halt;

    void run();
};

#endif