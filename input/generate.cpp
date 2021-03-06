#include<bits/stdc++.h>
using namespace std;

int main(int argc, char const *argv[])
{
    if(argc!=4)
    {
        cout<<"Usage: ./generate <icache_input> <dcache_input> <register_input>"<<endl;
        cout<<"put 0 if you want 0 in all"<<endl;
        return 0;
    }
    ifstream icache_input(argv[1]);
    ifstream dcache_input(argv[2]);
    ifstream rf_input(argv[3]);
    ofstream icache("../input/ICacheData.txt");
    ofstream dcache("../input/DCacheData.txt");
    ofstream rf("../input/RFData.txt");
    
    if(icache_input.is_open())
    {
        string s;
        int c=0;
        while(getline(icache_input,s))
        {
            c+=2;
            icache<<s.substr(0,2)<<"\n";
            icache<<s.substr(2,2)<<"\n";
        }

        while(c<256)
        {
            icache<<"00\n";
            c++;
        }
    }

    int c=0;
    if(dcache_input.is_open())
    {
        string s;
        while(getline(dcache_input,s))
        {
            c++;
            dcache<<s<<"\n";
        }
    }
    while(c<256)
    {
        dcache<<"00\n";
        c++;
    }
    
    c=0;
    if(rf_input.is_open())
    {
        string s;
        while(getline(rf_input,s))
        {
            c++;
            rf<<s<<"\n";
        }
    }
    while(c<16)
    {
        rf<<"00\n";
        c++;
    }

}