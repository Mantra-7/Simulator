#include "Structures.hpp"

void MEModule::ME()
{
    if(!exmebuf.valid)
    {
        stall = true;
        mewbbuf.valid = false;
        return;
    }

    mewbbuf.alu_result = exmebuf.alu_result;
    mewbbuf.dest = exmebuf.dest;
    stall = false;

    if(exmebuf.load)
    {
        int val = D$.request(exmebuf.alu_result);
        LMD.write(val); 
        mewbbuf.load = true;
    }
    
    if(exmebuf.store)
    {
        D$.write(exmebuf.alu_result, exmebuf.store_src);
        mewbbuf.store = true;
    }

}