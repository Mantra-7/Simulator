#include "../include/Structures.hpp"

int main()
{
    ifstream icache("input/ICacheData.txt");
    ifstream dcache("input/DCacheData.txt");
    ifstream rf("input/RFData.txt");

    Processor processor(icache, dcache, rf);
    
    int cc=1;
    while(!processor.halt)
    {
        cout<<cc++<<endl<<"-------------------------------------------"<<endl;
        processor.run();
        cout<<"-------------------------------------------"<<endl;
    }
}
