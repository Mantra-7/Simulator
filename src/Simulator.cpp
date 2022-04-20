#include "../include/Structures.hpp"

int instructions = 0;
int arith = 0;
int logic = 0;
int data = 0;
int control = 0;
int halt = 0;
int stall = 0;
int datastall = 0;
int controlstall = 0;


void update_stats(Processor pc)
{
    if(!pc.EX.stall) instructions++;
    if(pc.EX.idexbuf.arithmatic) arith++;
    if(pc.EX.idexbuf.logical) logic++;
    if(pc.EX.idexbuf.load) data++;
    if(pc.EX.idexbuf.store) data++;
    if(pc.EX.idexbuf.jump) control++;
    if(pc.EX.idexbuf.beqz) control++;
    if(pc.EX.idexbuf.halt) halt++;

    if(pc.dataHaz) datastall++;
}

void print_stats()
{
    cout<<"Total number of instructions executed: "<<instructions<<endl;
    cout<<"Number of instructions in each class"<<endl;
    cout<<"Arithmetic instructions              : "<<arith<<endl;
    cout<<"Logical instructions                 : "<<logic<<endl;
    cout<<"Data instructions                    : "<<data<<endl;
    cout<<"Control instructions                 : "<<control<<endl;
    cout<<"Halt instructions                    : "<<halt<<endl;
    cout<<"Cycles Per Instruction               : "<<instructions/halt<<endl;
    cout<<"Total number of stalls               : "<<stall<<endl;
    cout<<"Data stalls (RAW)                    : "<<datastall<<endl;
    cout<<"Control stalls                       : "<<controlstall<<endl;
}

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
        update_stats(processor);
        cout<<"-------------------------------------------"<<endl;
    }

    print_stats();
}
