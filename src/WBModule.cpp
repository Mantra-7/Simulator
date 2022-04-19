#include "../include/Structures.hpp"

void WBModule::run()
{
    //cout<<"WBModule: "<<"load "<<mewbbuf.load<<" wtr "<<mewbbuf.writeToReg<<endl;
    if(!mewbbuf.valid)
    {
        stall = true;
        return;
    }
    
    stall = false;
    if(mewbbuf.load)
    {
        cout<<"WB: writing from load at "<<(int)mewbbuf.dest<<" val is "<<(int)LMD.read()<<endl;
        RF.write(mewbbuf.dest, LMD.read());
    }
    
    if(mewbbuf.writeToReg)
    {
        cout<<"WB: Writing from alu at "<<(int)mewbbuf.dest<<" val is "<<mewbbuf.alu_result<<endl;
        RF.write(mewbbuf.dest, mewbbuf.alu_result);
    }
}