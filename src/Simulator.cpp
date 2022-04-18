#include "../include/Structures.hpp"

int main()
{
    ifstream icache("input/ICacheData.txt");
    ifstream dcache("input/DCacheData.txt");

    Processor processor(icache, dcache);
    
    for(int i = 0; i < 30; i+=2)
    {
        processor.run();
    }
}
