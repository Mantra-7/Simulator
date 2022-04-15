#include "Structures.hpp"

void MEModule::ME()
{
   
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