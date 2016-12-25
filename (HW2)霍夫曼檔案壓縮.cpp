#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<sstream>
#include<bitset>
#define secret "777888999"              
using namespace std;
struct charactor
{
	char crt;
	int cnt;
	charactor *left;
	charactor *right;
	string code;
}ch[256],charr[256];

void compress(string, string);
void decompress(string,string);

bool countChar(string);
int arrangeChar();
charactor* makeHuffman(int);
void getCode(charactor *);
unsigned char binaryStringToChar(string,int);

int main()
{
	string check="0";
	do
	{
		cout<<"1 :Compress Text File"<<endl;
		cout<<"2 :Decompress Text File"<<endl; 
		cout<<"3 :Exit"<<endl;
		cin>>check;
		string input,output;
		if(check=="1")
		{
			cout<<"Enter the file need compress :"<<endl; 
			cin>>input;
			cout<<"Enter the file name after compress :"<<endl;
			cin>>output; 
			compress(input,output);
		}
		else if(check=="2")
		{
			cout<<"Enter the file need decompress :"<<endl;
			cin>>input;
			cout<<"Enter the file name after decompress :"<<endl;
			cin>>output; 
			decompress(input,output);
		}
	}while(check!="3");
	return 0;
}
void compress(string input,string output)
{
	if(countChar(input))                                  
	{
		int item=arrangeChar();                           
		charactor *root=makeHuffman(item);
		getCode(root);                                   
		fstream fout,fin;
		fout.open(output.c_str(),ios::out|ios::binary);
		fout<<secret<<'\n';                               
		for(int i=0;i<256;i++)                           
		{
			fout<<ch[i].cnt<<" ";
		}
		fout<<'\n';    
		fin.open(input.c_str(),ios::in|ios::binary);
		if(!fin) 
		{
			cout<<"Can't open the file!"<<endl;
			return;
		} 
		char c;
		string len8="";
		unsigned char uc;
		while(fin.get(c))
		{
			len8+=ch[int(c)+128].code;
			while(len8.length()>=8)
			{
				uc=binaryStringToChar(len8,0);
				len8=len8.substr(8,len8.length()-8);
				fout.put(uc);
				//cout<<"=========Compressing============"<<endl;
			}
		}
		int len=len8.length();
		for(int i=len;i<8;i++)                          
		{
			len8+="1";
		}
		uc=binaryStringToChar(len8,0);                   
		fout.put(uc);
		fout<<(8-len);                                 
		fin.close();
		fout.close();
		cout<<"Compress Success!"<<endl;                
		
		do
		{
			cout<<"Press 1 to see Words Count"<<endl;
			cout<<"Press 2 to see Huffman Code"<<endl;
			cout<<"Press 3 to Exit"<<endl;
			cin>>len8;
			if(len8=="1")
			{
				cout<<"Words Count"<<endl;
				for(int i=0;i<256;i++)
					cout<<ch[i].crt<<" : "<<ch[i].cnt<<endl;
			}
			else if(len8=="2")
			{
				cout<<"Huffman Code"<<endl;
				for(int i=0;i<256;i++)
					if(ch[i].cnt)
						cout<<ch[i].crt<<": ("<<ch[i].cnt<<") ->"<<ch[i].code<<endl;
			}
		}while(len8!="3");
	}
	else 
		cout<<"Compress Failed"<<endl;
}
void decompress(string input,string output)
{
	fstream fin,fout;
	fin.open(input.c_str(),ios::in|ios::binary);
	if(!fin)
	{
		cout<<"Can't open the file!"<<endl;
		return;
	}
	string code,in;
	getline(fin,in);
	if(in==secret)
	{
		int cnt,i=0;
		getline(fin,in);
		stringstream ss;
		ss<<in;
		while(ss>>cnt)
		{
			ch[i].cnt=cnt;
			ch[i].crt=(char)(i+128);
			ch[i].left=NULL;
			ch[i].right=NULL;
			i++;
		}
		int item=arrangeChar();
		charactor *root=makeHuffman(item);
		getCode(root);
		
		int maxcode=0;
		for(i=0;i<256;i++)
		{
			if(ch[i].code.length()>maxcode)
				maxcode=ch[i].code.length();
		}
		fout.open(output.c_str(),ios::out|ios::binary);
		charactor *node=root;		
		code="";                                      
		char c;
		while(fin.get(c))
		{
			bitset<8>bt(c); 
			code+=bt.to_string();
			while(code.length()>maxcode&&code.length()>16)
			{
				i=0;
				while(node->left!=NULL&&node->right!=NULL)
				{
					if(code[i]=='0')
						node=node->left;
					else if(code[i]=='1')
						node=node->right;
					i++;
				}
				fout.put(node->crt);
				node=root;	
				code=code.substr(i,code.length()-i);
			}
		}
		int num=c-48;    
		while(code.length()>8+num)
		{
			i=0;
			while(node->left!=NULL&&node->right!=NULL)
			{
				if(code[i]=='0')
					node=node->left;
				else if(code[i]=='1')
					node=node->right;
				i++;
			}
			fout.put(node->crt); 
			node=root;	
			code=code.substr(i,code.length()-i);
		}
		
		fout.close();	 
		cout<<"Decompress Success!"<<endl;
	}
	else 
		cout<<"We can't decompress the file!"<<endl;
	fin.close();
}
unsigned char binaryStringToChar(string len8,int start)
{
	unsigned char change=0,mask=1;
	for(int i=7+start;i>=start;i--)  
	{
		if(len8.at(i)=='1')
		{
			change|=mask;
		}
		mask<<=1;
	}
	return change;
}
bool countChar(string input) 
{
	for(int i=0;i<256;i++)
	{
		ch[i].crt=(char)i-128;
		ch[i].cnt=0;
	}
	fstream fin;
	fin.open(input.c_str(),ios::in|ios::binary);
	char c;
	while(fin.get(c))
	{
		ch[(int)c+128].cnt++;
	}
	return true;
}
int arrangeChar()
{
	int j=0;
	for(int i=0;i<256;i++)
	{
		if(ch[i].cnt!=0)
		{
			charr[j].crt=ch[i].crt;
			charr[j].cnt=ch[i].cnt;
			charr[j].left=NULL;
			charr[j].right=NULL;
			charr[j++].code="";
		}
	}
	return j;
}
charactor* makeHuffman(int item)
{
	charactor **index=new charactor*[item];
	charactor *root=new charactor();
	for(int j=0;j<item;j++)
	{
		*(index+j)=(charr+j);
	}
	for(int i=item;i>1;i--)
	{	
		for(int j=i-1;j>=0;j--)
		{
			for(int k=0;k<j;k++)
			{
				if((*(index+k))->cnt>(*(index+k+1))->cnt)
				{
					charactor *tmp=*(index+k);
					*(index+k)=*(index+k+1);
					*(index+k+1)=tmp;
				}
			}
		}
		(*(index+0))->code="0"; 
		(*(index+1))->code="1";
		
		charactor *parent=new charactor();
		parent->left=*(index+0);
		parent->right=*(index+1);
		parent->cnt=parent->left->cnt+parent->right->cnt;
		
		*(index+1)=parent;       
		*(index+0)=*(index+i-1);
		
		root=parent;
	}
	return root;
}
void getCode(charactor *root)
{
	charactor **queue=new charactor*[10000];
	int head=-1,tail=0;
	*(queue+(tail))=root;
	while(head!=tail)
	{
		int cnt=0;
		head=(head+1)%10000;
		if((*(queue+head))->left!=NULL)
		{
			tail=(tail+1)%10000;
			*(queue+tail)=(*(queue+head))->left;
			(*(queue+(tail)))->code+=(*(queue+head))->code;

			tail=(tail+1)%10000;
			*(queue+tail)=(*(queue+head))->right;
			(*(queue+(tail)))->code+=(*(queue+head))->code;
		}
		else
		{
			string s="";
			for(int i=0;i<((*(queue+head))->code).length();i++)
			{
				s+=((*(queue+head))->code)[((*(queue+head))->code).length()-i-1];
			}
			ch[((int)(*(queue+head))->crt)+128].code=s;
		}
	}
}
