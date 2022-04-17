#include "Structures.hpp"

void WBModule::WB()
{
    if(!mewbbuf.valid)
    {
        stall = true;
        return;
    }
    
    stall = false;
    if(mewbbuf.load)
    {
        RF.write(mewbbuf.dest, LMD.read());
    }
    else
    {
        RF.write(mewbbuf.dest, mewbbuf.alu_result);
    }
}