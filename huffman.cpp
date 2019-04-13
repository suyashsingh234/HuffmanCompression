#include<iostream>
#include<fstream>
#include<math.h>
#include<queue>
#include<map>
#include<bitset>
using namespace std;
struct node
{
    int freq=0;
    char value='-';
    struct node* left=nullptr;
    struct node* right=nullptr;
}*root=nullptr;
struct node* getnode(int freq,char ch)
{
    struct node* newnode= new node;
    newnode->freq=freq;
    newnode->value=ch;
    newnode->left=nullptr;
    newnode->right=nullptr;
    return newnode;
}
void getcode(string chartocode[],int codelen, struct node* curr,string code)
{
    if(curr)
    {
        if(!curr->left && !curr->right)
        {
            //int remaining=codelen-code.size();
            //cout<<curr->value<<" "<<remaining<<endl;
//            for(int i=0;i<remaining;i++)
//                code.append("0");
            chartocode[(int)curr->value]=code;
        }
        else
        {
            getcode(chartocode,codelen,curr->left,code+"0");
            getcode(chartocode,codelen,curr->right,code+"1");
        }
    }
}
struct cmp
{
    bool operator()(struct node* n1,struct node* n2)
    {
        if(n1->freq>n2->freq)
            return true;
        else
            return false;
    }
};
void buildtree(int arr[])
{
    priority_queue<struct node*,vector<node*>,cmp>pq; //min heap
    for(int i=0;i<256;i++)
    {
        if(arr[i]>0)
        {
            struct node* newnode=getnode(arr[i],(char)i);
            pq.push(newnode);
        }
    }
    while(!pq.empty())
    {
        struct node* nullnode,*n1,*n2;
        n1=pq.top();
        pq.pop();
        if(!pq.empty())
        {
            n2=pq.top();
            pq.pop();
            nullnode=getnode(n1->freq+n2->freq,' ');
            nullnode->left=n1;
            nullnode->right=n2;
        }
        else
        {
            nullnode=getnode(n1->freq,' ');
            nullnode->left=n1;
            nullnode->right=nullptr;
        }
        //cout<<n1->value<<" "<<n2->value<<" "<<nullnode->freq<<endl;
        if(pq.size()>0)
            pq.push(nullnode);
        else
            root=nullnode;
    }
}
void encode(string filepath)
{
    int arr[256];
    int noofchar=0;
    for(int i=0;i<256;i++)
        arr[i]=0;
    fstream file;
    file.open(filepath.c_str(),ios::in);
    char ch;
    while(file.get(ch))
    {
        noofchar++;
        arr[(int)ch]++;
    }
    file.close();

    buildtree(arr);

    int uniquechar=0;
    for(int i=0;i<256;i++)
        if(arr[i]!=0)
            uniquechar++;
    int codelen=ceil(log2(uniquechar));

    string chartocode[256];
    getcode(chartocode,codelen,root,"");

    map<string,char>codetochar;
    for(int i=0;i<256;i++)
        if(chartocode[i]!="")
            codetochar[chartocode[i]]=(char)i;

    string stringofbits;
    file.open(filepath,ios::in);
    while(file.get(ch))
    {
        stringofbits.append(chartocode[(int)ch]);
    }
    file.close();
    bitset<100000>bits(stringofbits);
    //cout<<bits;
    fstream binfile;
    binfile.open("binfile.bin",ios::out|ios::binary);
    //writing map
    binfile.write(reinterpret_cast<char*>(&uniquechar),sizeof(int));  //size of map
    //cout<<uniquechar<<endl;
    for(auto x:codetochar)
    {
        int len=x.first.size();
        binfile.write(reinterpret_cast<char*>(&len),sizeof(int)); // size of string
        binfile.write(x.first.c_str(),x.first.size());
        binfile.write(&x.second,sizeof(char)); //character
        //cout<<x.second<<endl;
    }

    int bitpos=0;
    char writevalue;
    bitset<8>charbit(writevalue);
    for(int i=0;stringofbits[i]!='\0';i++)
    {
        charbit[7-bitpos]=stringofbits[i]-'0';
        if(bitpos==7)
        {
            bitpos=0;
            long long int j=charbit.to_ulong();
            writevalue=static_cast<char>(j);
            binfile.write(&writevalue,sizeof(char));
        }
        else
            bitpos++;
    }
    while(bitpos<=7)
    {
        charbit[7-bitpos]=0;
        bitpos++;
    }
    long long int i=charbit.to_ulong();
    writevalue=static_cast<char>(i);
    binfile.write(&writevalue,sizeof(char));
    binfile.close();
    //cout<<codelen;
    //cout<<stringofbits;

}
void decode(string filepath)
{
    map<string,char>codetochar;
    fstream binfile;
    binfile.open(filepath,ios::in|ios::binary);
    int sizeofmap;
    binfile.read(reinterpret_cast<char*>(&sizeofmap),sizeof(int));
    for(int i=0;i<sizeofmap;i++)
    {
        char ch;
        int len;
        binfile.read(reinterpret_cast<char*>(&len),sizeof(int));
        char* code=new char[len];
        binfile.read(&code[0],len);
        string propercode="";
        for(int j=0;j<len;j++)
            propercode.push_back(code[j]);
        //cout<<propercode<<endl;
        binfile.read(&ch,sizeof(char));
        //cout<<code<<endl;
        //cout<<ch<<endl;
        codetochar[propercode]=ch;
        delete[]code;
    }
//    for(auto x:codetochar)
//        cout<<x.first<<endl;
    char ch;
    string findcode="";
    fstream decodedfile;
    decodedfile.open("decodedfile.txt",ios::out);
    while(binfile.read(&ch,sizeof(char)))
    {
        bitset<8>bits(ch);
        for(int i=0;i<8;i++)
        {
            if(bits[7-i]==1)
                findcode.append("1");
            else
                findcode.append("0");
            if(codetochar.find(findcode)!=codetochar.end())
            {
                decodedfile<<codetochar[findcode];
                findcode="";
            }
        }
    }
    decodedfile.close();
    //cout<<findcode;
    binfile.close();
}
int main()
{
    cout<<"Options:"<<endl;
    cout<<"1:Compress file"<<endl;
    cout<<"2:Decode a compressed file"<<endl;
    int option;
    cout<<"Enter option:";
    cin>>option;
    cin.ignore();
    if(option==1)
    {
        cout<<"Enter filepath:";
        string filepath;
        getline(cin,filepath);
        encode(filepath);
    }
    else
    {
        cout<<"Enter filepath:";
        string filepath;
        getline(cin,filepath);
        decode(filepath);
    }
    return 0;
}
