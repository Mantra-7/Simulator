#include "../include/Structures.hpp"

void Processor::run()
{
    IF.run();
    IDRF.run();
    if(prevBranchRes && !branch_resolved)
    {
        //cout<<"flush"<<endl;
        flushIFID();
    }
    prevBranchRes = branch_resolved;
    EX.run();
    MEM.run();
    int prevdh = dataHaz;
    WB.run();
    //cout<<"datahaz bef is "<<dataHaz<<endl;
    if(prevdh && !dataHaz)
    {
        IDRF.run();
        IFID2 = IFID1;
        IFID2.valid = true;
    }

    if(!dataHaz && !prevdh) IFID2 = IFID1;
    IDEX2 = IDEX1;
    EXME2 = EXME1;
    MEWB2 = MEWB1;
    LMD2 = LMD1;
}

void Processor::flushIFID()
{
    IFID1.valid = false;
    pc.decrement();
}