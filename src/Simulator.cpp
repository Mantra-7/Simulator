#include "../include/Structures.hpp"

int main()
{
    ifstream icache("input/ICacheData.txt");
    ifstream dcache("input/DCacheData.txt");

    Processor processor(icache, dcache);
    
    int cc=1;
    while(!processor.halt)
    {
        cout<<cc++<<endl<<"-------------------------------------------"<<endl;
        processor.run();
        cout<<"-------------------------------------------"<<endl;
    }
}
