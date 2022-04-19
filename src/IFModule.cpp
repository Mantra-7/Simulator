#include "../include/Structures.hpp"

void IFModule::run() 
{
    if(stall || !branch_resolved)
    {
        ifidbuf.valid = false;
        return;
    }
    
    ifidbuf.valid = true;
    int16 instruction = I$.request(pc.read());
    cout << "IFModule: " << hex << instruction << endl;
    IR.write(instruction);
    ifidbuf.instruction = instruction;
    pc.increment();
}