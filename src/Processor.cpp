#include "Structures.hpp"

Processor::Processor()
{
    IFModule IF(pc, I$, IR, IFID1);
    IDRFModule IDRF(rf, D$, IFID2, IDEX1, halt);
    EXModule EX(alu, pc, rf, IDEX2, EXME1);
    MEModule MEM(D$, EXME2, MEWB1, LMD);
    WBModule WB(rf, MEWB2, LMD);
}

void Processor::run()
{
    IF.run();
    IDRF.run();
    EX.run();
    MEM.run();
    WB.run();

    IFID2 = IFID1;
    IDEX2 = IDEX1;
    EXME2 = EXME1;
    MEWB2 = MEWB1;
}