#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

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
    }

    void run();
    void flushIFID();
};

#endif