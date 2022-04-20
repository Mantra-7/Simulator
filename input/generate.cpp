#include<bits/stdc++.h>
using namespace std;

int main(int argc, char const *argv[])
{
    if(argc!=3)
    {
        cout<<"Usage: ./generate <icache_input> <dcache_input>"<<endl;
        cout<<"put 0 if you want 0 in all"<<endl;
        return 0;
    }
    ifstream icache_input(argv[1]);
    ifstream dcache_input(argv[2]);
    ofstream icache("../input/ICacheData.txt");
    ofstream dcache("../input/DCacheData.txt");
    
    if(icache_input.is_open())
    {
        string s;
        int c=0;
        while(getline(icache_input,s))
        {
            c++;
            icache<<s.substr(0,2)<<"\n";
            icache<<s.substr(2,2)<<"\n";
        }

        while(c<128)
        {
            icache<<"0000\n";
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
    

}