#include<bits/stdc++.h>
using namespace std;

void processLine(string);
map<string,char> reg;
bool invalid = false;
ofstream res;

int main(int argc, char const *argv[])
{
    for(int i=0;i<16;i++)
    {
        reg["R"+to_string(i)]= (i>9 ? 'a'+i-10:'0'+i);
    }

    if(argc!=3)
    {
        cout<<"Usage: ./msasm <inputFile> <outputFile>"<<endl;
        return 1;
    }

    ifstream fin(argv[1]);
    if(!fin.is_open())
    {
        cout<<"Error: File not found"<<endl;
        return 1;
    }

    res.open(argv[2]);
    if(!res.is_open())
    {
        cout<<"Error: File not found"<<endl;
        return 1;
    }
    
    string line;
    while(!invalid && getline(fin, line))
    {
        processLine(line);
    }
}


void processADD(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='0';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    res<<inst<<endl;
}

void processSUB(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='1';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    res<<inst<<endl;
}

void processMUL(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='2';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    res<<inst<<endl;
}

void processINC(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='3';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    inst+="xx";
    res<<inst<<endl;
}

void processAND(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='4';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    res<<inst<<endl;
}

void processOR(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='5';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    res<<inst<<endl;
}

void processXOR(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='6';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    res<<inst<<endl;
}

void processNOT(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='7';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    inst+="x";
    res<<inst<<endl;
}

void processLOAD(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='8';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    getline(ss,r,',');
    while(!r.empty() && r.front()==' ') r=r.substr(1);
    while(!r.empty() && r.back()==' ') r.pop_back();
    if(r.empty())
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid=true;
        return;
    }
    int x=stoi(r);
    if(x<0 || x>15)
    {
        cout<<"Error: Invalid immediate value"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    inst+=(x<10 ? '0'+x : 'a'+x-10);

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    res<<inst<<endl;
}

void processSTORE(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='9';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    getline(ss,r,',');
    while(!r.empty() && r.front()==' ') r=r.substr(1);
    while(!r.empty() && r.back()==' ') r.pop_back();
    if(r.empty())
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid=true;
        return;
    }
    int x=stoi(r);
    if(x<0 || x>15)
    {
        cout<<"Error: Invalid immediate value"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    inst+=(x<10 ? '0'+x : 'a'+x-10);

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    res<<inst<<endl;
}

void processJMP(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='a';

    string r;

    getline(ss,r,',');
    while(!r.empty() && r.front()==' ') r=r.substr(1);
    while(!r.empty() && r.back()==' ') r.pop_back();

    if(r.empty())
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    int x=stoi(r);
    if(x<0 || x>255)
    {
        cout<<"Error: Invalid immediate value"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    string hexa;
    hexa+=(x%16<10 ? '0'+x%16 : 'a'+x%16-10);
    x/=16;
    hexa+=(x%16<10 ? '0'+x%16 : 'a'+x%16-10);
    inst+=hexa[1];
    inst+=hexa[0];

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    inst+="x";
    res<<inst<<endl;
}

void processBEQZ(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='b';

    string r;
    if(getline(ss,r,','))
    {
        if(r.back()==',')
            r.pop_back();
        while(!r.empty() && r.front()==' ') r=r.substr(1);
        while(!r.empty() && r.back()==' ') r.pop_back();
        if(r.empty())
        {
            cout<<"Error: Less Arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid=true;
            return;
        }

        if(reg.find(r)==reg.end())
        {
            cout<<"Error: Invalid register"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
        inst += reg[r];
    }
    else
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }

    getline(ss,r,',');
    while(!r.empty() && r.front()==' ') r=r.substr(1);
    while(!r.empty() && r.back()==' ') r.pop_back();
    if(r.empty())
    {
        cout<<"Error: Less Arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid=true;
        return;
    }
    int x=stoi(r);
    if(x<0 || x>255)
    {
        cout<<"Error: Invalid immediate value"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    string hexa;
    hexa+=(x%16<10 ? '0'+x%16 : 'a'+x%16-10);
    x/=16;
    hexa+=(x%16<10 ? '0'+x%16 : 'a'+x%16-10);
    inst+=hexa[1];
    inst+=hexa[0];

    if(getline(ss,r,','))
    {
        cout<<"Error: Too many arguments"<<endl;
        cout<<"in line: "<<line<<endl;
        invalid = true;
        return;
    }
    res<<inst<<endl;
}

void processHLT(string line)
{
    stringstream ss(line);
    string opcode;
    ss>>opcode;
    
    string inst;
    inst+='f';

    string r;
    if(getline(ss,r,','))
    {
        while(!r.empty() && r.back()==' ') 
        {
            r.pop_back();
        }
        while(!r.empty() && r.front()==' ') r=r.substr(1);

        
        if(!r.empty())
        { 
            cout<<"Error: Too many arguments"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
    }
    inst+="xxx";
    res<<inst<<endl;
}

void processLine(string line)
{
    stringstream ss(line);
    string token;

    if(ss>>token)
    {
        if(token=="ADD") processADD(line);
        else if(token=="SUB") processSUB(line);
        else if(token=="MUL") processMUL(line);
        else if(token=="INC") processINC(line);
        else if(token=="AND") processAND(line);
        else if(token=="OR") processOR(line);
        else if(token=="XOR") processXOR(line);
        else if(token=="NOT") processNOT(line);
        else if(token=="LOAD") processLOAD(line);
        else if(token=="STORE") processSTORE(line);
        else if(token=="JMP") processJMP(line);
        else if(token=="BEQZ") processBEQZ(line);
        else if(token=="HLT") processHLT(line);
        else
        {
            cout<<"Error: Invalid instruction"<<endl;
            cout<<"in line: "<<line<<endl;
            invalid = true;
            return;
        }
    }
}