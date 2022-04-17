#include "Structures.hpp"

void WBModule::run()
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
    
    if(mewbbuf.writeToReg)
    {
        RF.write(mewbbuf.dest, mewbbuf.alu_result);
    }
}