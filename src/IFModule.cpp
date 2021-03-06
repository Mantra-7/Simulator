#include "../include/Structures.hpp"

void IFModule::run() 
{
    //cout<<"br "<<branch_resolved<<" dh "<<dataHaz<<endl;
    if(stop || !branch_resolved || dataHaz)
    {
        prevDataHaz = dataHaz;
        ifidbuf.valid = false;
        stall = true;
        return;
    }
    
    stall = false;
    ifidbuf.valid = true;
    int16 instruction = I$.request(pc.read());
    //cout << "IFModule: " << hex << instruction << endl;
    IR.write(instruction);
    ifidbuf.instruction = instruction;
    pc.increment();
}