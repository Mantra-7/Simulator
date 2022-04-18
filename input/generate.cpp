#include<bits/stdc++.h>
using namespace std;

int main()
{
    ofstream fdout("DCacheData.txt");
    ifstream fin("../Assembler/output");
    ofstream fiout("ICacheData.txt");
    
    for(int i=0;i<256;i++)
    {
        fdout<<"00\n";
    }

    string s;
    int c=0;
    while(getline(fin,s))
    {
        c++;
        fiout<<s<<"\n";
    }

    while(c<128)
    {
        fiout<<"0000\n";
        c++;
    }

}