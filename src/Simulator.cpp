#include "../include/Structures.hpp"

int main()
{
    ifstream icache("../input/ICacheData.txt");
    ifstream dcache("../input/DCacheData.txt");
    Processor processor(icache, dcache);
    
    while(!processor.halt)
    {
        processor.run();
    }
}
