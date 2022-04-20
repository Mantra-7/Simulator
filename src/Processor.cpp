#include "../include/Structures.hpp"

void Processor::run()
{
    IF.run();
    IDRF.run();
    if(prevBranchRes && !branch_resolved)
    {
        cout<<"flush"<<endl;
        flushIFID();
    }
    prevBranchRes = branch_resolved;
    EX.run();
    MEM.run();
    flag prevdh = dataHaz;
    WB.run();
    if(prevdh && !dataHaz)
    {
        IDRF.run();
        IFID2 = IFID1;
        IFID2.valid = true;
    }

    if(!dataHaz && !prevDataHaz) IFID2 = IFID1;
    IDEX2 = IDEX1;
    EXME2 = EXME1;
    MEWB2 = MEWB1;
}

void Processor::flushIFID()
{
    IFID1.valid = false;
    pc.decrement();
}