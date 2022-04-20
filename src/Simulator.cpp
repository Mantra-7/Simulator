#include "../include/Structures.hpp"

int instructions = 0;
int arith = 0;
int logic = 0;
int dataa = 0;
int control = 0;
int halt = 0;
int stall = -4;
int datastall = 0;
int controlstall = 0;
int cc=1;

void update_stats(Processor pc)
{
    if(!pc.EX.stall) 
    {
        instructions++;
        if(pc.EX.idexbuf.arithmatic) arith++;
        if(pc.EX.idexbuf.logical) logic++;
        if(pc.EX.exmebuf.load)  dataa++;
        if(pc.EX.exmebuf.store) dataa++;
        if(pc.EX.idexbuf.jump) control++;
        if(pc.EX.idexbuf.beqz) control++;
        if(pc.EX.exmebuf.halt) halt++;
    }
    else 
    {
        cout<<"stall here: "<<cc-1<<endl;
        stall++;
    }

    if(pc.dataHaz) 
    {
        cout<<"datahaz in "<<pc.dataHaz<<endl;
        datastall++;
    }
}

void print_stats(ofstream &fout)
{
    fout<<"Total number of instructions executed: "<<instructions<<endl;
    fout<<"Number of instructions in each class"<<endl;
    fout<<"Arithmetic instructions              : "<<arith<<endl;
    fout<<"Logical instructions                 : "<<logic<<endl;
    fout<<"Data instructions                    : "<<dataa<<endl;
    fout<<"Control instructions                 : "<<control<<endl;
    fout<<"Halt instructions                    : "<<halt<<endl;
    fout<<"Cycles Per Instruction               : "<<(float)(cc-1)/instructions<<endl;
    fout<<"Total number of stalls               : "<<stall<<endl;
    fout<<"Data stalls (RAW)                    : "<<datastall<<endl;
    fout<<"Control stalls                       : "<<controlstall<<endl;
}

int main()
{
    ifstream icache("input/ICacheData.txt");
    ifstream dcache("input/DCacheData.txt");
    ifstream rf("input/RFData.txt");
    ofstream dCacheOutput("output/ODCache.txt");
    ofstream rfOutput("output/ORF.txt");
    ofstream stats("output/Output.txt");

    Processor processor(icache, dcache, rf);

    while(!processor.halt)
    {
        cout<<cc++<<endl<<"-------------------------------------------"<<endl;
        processor.run();
        update_stats(processor);
        cout<<"-------------------------------------------"<<endl;
    }

    processor.D$.output(dCacheOutput);
    processor.rf.output(rfOutput);
    print_stats(stats);
}
