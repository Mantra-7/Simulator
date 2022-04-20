#include "../include/Structures.hpp"

int instructions = 0;
int arith = 0;
int logic = 0;
int dataa = 0;
int control = 0;
int halt = 0;
int stall = -4;
int datastall = 0;
int cc=1;

void update_stats(Processor pc, IDEXBuffer idex)
{
    if(!pc.EX.stall) 
    {
        instructions++;
        if(idex.arithmatic) arith++;
        if(idex.logical) logic++;
        if(idex.load)  dataa++;
        if(idex.store) dataa++;
        if(idex.jump) control++;
        if(idex.beqz) control++;
        if(idex.halt) halt++;
    }
    else 
    {
        //cout<<"stall here: "<<cc-1<<endl;
        stall++;
    }

    if(pc.dataHaz) 
    {
        //cout<<"datahaz in "<<pc.dataHaz<<endl;
        datastall++;
    }
}

void print_stats(ofstream &fout)
{
    fout<<dec<<"Total number of instructions executed: "<<instructions<<endl;
    fout<<dec<<"Number of instructions in each class"<<endl;
    fout<<dec<<"Arithmetic instructions              : "<<arith<<endl;
    fout<<dec<<"Logical instructions                 : "<<logic<<endl;
    fout<<dec<<"Data instructions                    : "<<dataa<<endl;
    fout<<dec<<"Control instructions                 : "<<control<<endl;
    fout<<dec<<"Halt instructions                    : "<<halt<<endl;
    fout<<dec<<"Cycles Per Instruction               : "<<(float)(cc-1)/instructions<<endl;
    fout<<dec<<"Total number of stalls               : "<<stall<<endl;
    fout<<dec<<"Data stalls (RAW)                    : "<<datastall<<endl;
    fout<<dec<<"Control stalls                       : "<<stall-datastall<<endl;
}

int main()
{
    ifstream icache("input/ICache.txt");
    ifstream dcache("input/DCache.txt");
    ifstream rf("input/RF.txt");
    ofstream dCacheOutput("output/ODCache.txt");
    ofstream rfOutput("output/ORF.txt");
    ofstream stats("output/Output.txt");

    Processor processor(icache, dcache, rf);

    while(!processor.halt)
    {
        IDEXBuffer id = processor.EX.idexbuf;
        cc++;
        //cout<<cc++<<endl<<"-------------------------------------------"<<endl;
        processor.run();
        update_stats(processor, id);
        //cout<<"-------------------------------------------"<<endl;
    }

    processor.D$.output(dCacheOutput);
    processor.rf.output(rfOutput);
    print_stats(stats);
}
