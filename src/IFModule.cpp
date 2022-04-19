#include "../include/Structures.hpp"

void IFModule::run() 
{
    if(!branch_resolved || dataHaz || prevDataHaz)
    {
        prevDataHaz = dataHaz;
        ifidbuf.valid = false;
        stall = true;
        return;
    }
    
    stall = false;
    ifidbuf.valid = true;
    int16 instruction = I$.request(pc.read());
    cout << "IFModule: " << hex << instruction << endl;
    IR.write(instruction);
    ifidbuf.instruction = instruction;
    pc.increment();
}