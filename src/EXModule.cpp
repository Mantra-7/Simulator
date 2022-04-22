#include "../include/Structures.hpp"

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