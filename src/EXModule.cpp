#include "../include/Structures.hpp"

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

    if(idexbuf.arithmatic)
    {
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
        exmebuf.load = true;
        exmebuf.dest = idexbuf.src1;
        exmebuf.alu_result = alu.adder(idexbuf.src2,idexbuf.offset,false);
        return;
    }

    if(idexbuf.store)
    {
        exmebuf.store = true;
        exmebuf.store_src = idexbuf.src1;
        exmebuf.alu_result = alu.adder(idexbuf.src2,idexbuf.offset,false);
        exmebuf.dest = exmebuf.alu_result;
        return;
    }

    if(idexbuf.jump)
    {
        exmebuf.jump = true;
        exmebuf.alu_result = alu.adder(pc.read(),idexbuf.jump_addr << 1,false);
        pc.write(exmebuf.alu_result);
        idexbuf.valid = false;
        stall = true;
        return;
    }

    if(idexbuf.bneq)
    {
        exmebuf.alu_result = alu.BNEQ(idexbuf.src1);
        if(exmebuf.alu_result)
        {
            exmebuf.alu_result = alu.adder(pc.read(),idexbuf.jump_addr << 1,false);
            pc.write(exmebuf.alu_result);
            idexbuf.valid = false;
            stall = true;
        }
        return;
    }
}