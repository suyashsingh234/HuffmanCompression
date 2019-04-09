#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
using namespace std;
struct node
{
    char ch;
    int freq;
    struct node* left;
    struct node* right;
};
node* createnode(char ch,int freq)
{
    struct node* newnode=new node;
    newnode->ch=ch;
    newnode->freq=freq;
    newnode->left=nullptr;
    newnode->right=nullptr;
    return newnode;
}
void levelorder(struct node *curr,int lvl)
{
    if(curr)
    {
        if(lvl==1)
            cout<<curr->freq<<" ";
        else
        {
            levelorder(curr->left,lvl-1);
            levelorder(curr->right,lvl-1);
        }
    }
}
void encodefile(struct node* curr,string chartocode[],string code)
{
    if(curr)
    {
        if(!curr->left && !curr->right)
        {
            chartocode[(int)curr->ch]=code;
        }
        else
        {
            encodefile(curr->left,chartocode,code+'0');
            encodefile(curr->right,chartocode,code+'1');
        }
    }
}
int main()
{
    int arr[256];
    for(int i=0;i<256;i++)
        arr[i]=0;
    ifstream fin;
    fin.open("file.txt",ios::in);
    char ch;
    while(fin.get(ch))
    {
        arr[(int)ch]++;
    }
    vector<node*>coll;
    for(int i=0;i<256;i++)
    {
        if(arr[i]>0)
        {
            struct node* node=createnode((char)i,arr[i]);
            coll.push_back(node);
        }
    }
//    for(auto x:coll)
//        cout<<x->ch<<endl;
    struct node* root;
    while(!coll.empty())
    {
        int min=1000000;
        struct node* node_min,*node_second_min;
        for(auto x:coll)
        {
            if(x->freq<=min)
            {
                node_min=x;
                min=x->freq;
            }
        }
        coll.erase(find(coll.begin(),coll.end(),node_min));
        min=1000000;
        for(auto x:coll)
        {
            if(x->freq<=min)
            {
                node_second_min=x;
                min=x->freq;
            }
        }
        coll.erase(find(coll.begin(),coll.end(),node_second_min));
        struct node* nullnode=createnode(' ',node_min->freq+node_second_min->freq);
        nullnode->left=node_min;
        nullnode->right=node_second_min;
        if(coll.size()>=1)
            coll.push_back(nullnode);
        else
            root=nullnode;
    }
    //cout<<root->freq<<endl;
    fin.close();
    string chartocode[256];
    for(int i=0;i<256;i++)
        chartocode[i]="-1";
//    for(int i=1;i<=3;i++)
//    {
//        levelorder(root,i);
//        cout<<endl;
//    }
    encodefile(root,chartocode,"");
    for(int i=0;i<256;i++)
    {
        if(chartocode[i]!="-1")
        {
            cout<<(char)i<<" encoded as "<<chartocode[i]<<endl;
        }
    }
////////////////////////////////
    return 0;
}
