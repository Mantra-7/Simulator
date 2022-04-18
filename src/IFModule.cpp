#include "../include/Structures.hpp"

void IFModule::run() 
{
    if(stall) 
    {
        ifidbuf.invalidate();
        return;
    }
    int16 instruction = I$.request(pc.read());
    cout << "IFModule: " << hex << instruction << endl;
    IR.write(instruction);
    ifidbuf.set(instruction);
    pc.increment();
}