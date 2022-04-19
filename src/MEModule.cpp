#include "../include/Structures.hpp"

void MEModule::run()
{
    if(!exmebuf.valid)
    {
        stall = true;
        mewbbuf.valid = false;
        return;
    }
    cout<<"MEModule: "<<endl;

    mewbbuf.alu_result = exmebuf.alu_result;
    mewbbuf.writeToReg = exmebuf.writeToRegister;
    mewbbuf.dest = exmebuf.dest;
    mewbbuf.valid = true;
    stall = false;
    mewbbuf.load = false;

    if(exmebuf.load)
    {
        cout<<"MEM: Loading from address "<<exmebuf.alu_result<<endl;
        int val = D$.request(exmebuf.alu_result);
        LMD.write(val); 
        mewbbuf.load = true;
    }
    
    if(exmebuf.store)
    {
        cout<<"MEM: Storing to address "<<exmebuf.alu_result<<endl;
        D$.write(exmebuf.alu_result, exmebuf.store_src);
        mewbbuf.store = true;
    }

    if(exmebuf.halt)
    {
        mewbbuf.halt = true;
        return;
    }
}