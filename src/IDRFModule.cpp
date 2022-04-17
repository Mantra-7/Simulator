#include "../include/Structures.hpp"

void IDRFModule::run()
{
    if(!ifidbuf.valid)
    {
        stall = true;
        idexbuf.valid = false;
        return;
    }

    idexbuf.arithmatic = false;
    idexbuf.logical = false;
    idexbuf.load = false;
    idexbuf.store = false;
    idexbuf.bneq = false;
    idexbuf.jump = false;
    idexbuf.halt = false;
    idexbuf.valid = true;

    int16 instruction = ifidbuf.getInstruction();
    int16 opcode = instruction >> 12;
    idexbuf.opcode = opcode;

    if(opcode<4)
    {
        idexbuf.arithmatic = true;

        int8 r3 = (instruction & 0x0f00)>>8;
        int8 r1 = (instruction & 0x00f0)>>4;
        int8 r2 = (instruction & 0x000f);

        idexbuf.src1 = RF.read(r1);
        idexbuf.src2 = RF.read(r2);
        idexbuf.dest = r3;
        idexbuf.subop = opcode & 0x03;
        return;
    }

    if(opcode<8)
    {
        idexbuf.logical = true;

        int8 r3 = (instruction & 0x0f00)>>8;
        int8 r1 = (instruction & 0x00f0)>>4;
        int8 r2 = (instruction & 0x000f);

        idexbuf.src1 = RF.read(r1);
        idexbuf.src2 = RF.read(r2);
        idexbuf.dest = r3;
        idexbuf.subop = opcode & 0x03;
        return;
    }

    if(opcode == 8)
    {
        idexbuf.load = true;
        int8 r1 = (instruction & 0x0f00)>>8;
        int8 r2 = (instruction & 0x00f0)>>4;
        int8 x = (instruction & 0x000f);

        idexbuf.src1 = RF.read(r1);
        idexbuf.src2 = RF.read(r2);
        idexbuf.offset = x;
        return;
    }

    if(opcode == 9)
    {
        idexbuf.store = true;
        int8 r1 = (instruction & 0x0f00)>>8;
        int8 r2 = (instruction & 0x00f0)>>4;
        int8 x = (instruction & 0x000f);

        idexbuf.src1 = RF.read(r1);
        idexbuf.src2 = RF.read(r2);
        idexbuf.offset = x;
        return;
    }

    if(opcode == 10)
    {
        idexbuf.jump = true;
        idexbuf.jump_addr = (instruction & 0x0ff0)>>4;
        return;
    }

    if(opcode == 11)
    {
        idexbuf.bneq = true;
        int8 r1 = (instruction & 0x0f00)>>8;
        idexbuf.src1 = RF.read(r1);
        idexbuf.jump_addr = instruction & 0x00ff;
        return;
    }

    idexbuf.halt = true;
    halt = true;
}