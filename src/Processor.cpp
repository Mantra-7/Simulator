#include "../include/Structures.hpp"

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