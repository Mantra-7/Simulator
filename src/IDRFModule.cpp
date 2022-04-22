#include "../include/Structures.hpp"

void IDRFModule::run(int x)
{
    if((stop || !ifidbuf.valid || !branch_resolved || dataHaz) && x)
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
        idexbuf.src1 = RF.read(r2);
        idexbuf.src2 = RF.read(r1);
        idexbuf.offset = x;
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
        branch_resolved = false;
        idexbuf.beqz = true;
        int8 r1 = (instruction & 0x0f00)>>8;
        if(!RF.R[r1].valid)
        {
            stall = true;
            idexbuf.valid = false;
            RF.R[r1].dh = true;
            dataHaz++;
        }
        idexbuf.src1 = RF.read(r1);
        idexbuf.jump_addr = instruction & 0x00ff;
    }

    if(opcode==15)
    {
        idexbuf.halt = true;
        stop = true;
    }
}