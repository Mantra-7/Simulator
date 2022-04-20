#include "../include/Structures.hpp"

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