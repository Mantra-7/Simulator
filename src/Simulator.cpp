#include "../include/Structures.hpp"

int main()
{
    cout<<"huh, lmao"<<endl;
    ifstream icache("../input/ICacheData.txt");
    ifstream dcache("../input/DCacheData.txt");
    Processor processor(icache, dcache);
    
    while(!processor.halt)
    {
        processor.run();
    }
}
