#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
using namespace std;

struct node {
  double times;
  char symbol;
  struct node *lcd, *rcd;
  bool visited;
  string code;
};

void countTimes(fstream *fp, double *frequency);  // 計算出現次數
int SelSort_and_Add_Node(double *frequency, node *nodes);  // 回傳有效點個數
node* creatTree(node *nodes, int index);  //  建樹
string* getHuffmanCode(node *root, int index, string *code);
void preorder(node *ptr, string *code);
void compress(fstream *fp, fstream *outfp, string *code);
void decompress(fstream *infp, fstream *outfp, string *code);

int main(int argc, char const *argv[])
{
  double *frequency = new double[128];
  char infilename[50];
  char outfilename[50];
  node *nodes = new node[255];  // 建立一系列nodes 並給予times = 0(給之後做判斷)
  for (int i = 0; i < 255; i++)
    (nodes + i)->times = 0;
  cout << "Enter the name of the file you want to compress: ";
  cin >> infilename;
  cout << "\nEnter the name you want for compressed outfile : ";
  cin >> outfilename;
  cout << "Opening " << infilename << "..." << endl;
  fstream *outfp = new fstream();
  fstream *infp = new fstream();
  infp->open(infilename);

  countTimes(infp, frequency); //計算出現次數
  infp->close(); //關閉檔案
  int index = SelSort_and_Add_Node(frequency, nodes); // 取得有用的node數 並排序node
  node *root = creatTree(nodes, index); // 取得Huffman Tree
  string* code = new string[128]; // 用來存取每個symbol 對應的Huffman Code
  getHuffmanCode(root, index, code);

  cout << "\nWriting compression file: " << outfilename << "..." << endl;
  infp->open(infilename);
  outfp->open(outfilename, ios::out);
  compress(infp, outfp, code); // 呼叫壓縮函式
  infp->close();
  outfp->close();

  cout << "\nFor decompression ..." << endl;
  cout << "Enter the name of the file you want to decompress: ";
  cin >> infilename;
  cout << "\nEnter the name you want for decompressed outfile : ";
  cin >> outfilename;
  infp->open(infilename);
  outfp->open(outfilename, ios::out);
  decompress(infp, outfp, code); // 呼叫解壓縮函式
  infp->close();
  outfp->close();
  // system("pause");
  return 0;
}
void countTimes(fstream *fp, double *frequency){
  cout << "Frequency Analytics: " << endl;
  for (int i = 0; i < 128; i++)
    *(frequency + i) = 0;
  char c;
  while(fp->get(c))
    *(frequency + (int)c) += 1;
  for (int i = 32; i < 127; i++)
    cout << (char)i << ":" << *(frequency + i) << endl;
}
int SelSort_and_Add_Node(double *frequency, node *nodes){
  // Add Node and delete char that shows up 0 times
  int index = 0;
  for (int i = 0; i < 128; i++)
  {
    if (*(frequency + i)!=0)
    {
      // *().elements || ()->elements !
      (nodes + index)->symbol = (char)i;
      (nodes + index)->times = *(frequency + i);
      (nodes + index)->visited = false;
      (nodes + index)->lcd = NULL;
      (nodes + index)->rcd = NULL;
      // cout << (nodes + index)->symbol <<": "<< (nodes + index)->times << endl;
      index++;
    }
  }
  cout << "\nThere are "<< index << " different symbols.\n" << endl;

  // === Selection Sort ... ===
  double tmpTimes;
  char tmpSymbol;
  for (int i = 0; i < index; i++)
  {
    for (int j = i+1; j < index; j++)
    {
      if ((nodes + j)->times < (nodes + i)->times)
      {
        // Switch Times
        tmpTimes = (nodes + i)->times;
        (nodes + i)->times = (nodes + j)->times;
        (nodes + j)->times = tmpTimes;
        // Switch Symbol
        tmpSymbol = (nodes + i)->symbol;
        (nodes + i)->symbol = (nodes + j)->symbol;
        (nodes + j)->symbol = tmpSymbol;
      }
    }
    cout << (nodes + i)->symbol <<": "<< (nodes + i)->times << endl;
  }
  return index;
}
node* creatTree(node *nodes, int index){
  cout << "\n================\nMaking Huffman Tree.\n================\n" << endl;
  for (int i = 0; i < index; i++)
  {
    if (!((nodes + i)->visited) && ((nodes + (i+1))->times!=0))
    {
      node *parentNode = new node();
      parentNode->lcd = (nodes + i);
      parentNode->rcd = (nodes + (i+1));
      parentNode->times = (nodes + i)->times + (nodes + (i+1))->times;
      // parentNode->symbol = '';
      (nodes + i)->visited = true; // 避免重複新增
      (nodes + i)->code = "0";
      (nodes + (i+1))->visited = true; // 避免重複新增
      (nodes + (i+1))->code = "1";

      int tmp = index;
      for (int j = i+2; j < index; j++)
      { // 尋找欲插入的位置
        if (parentNode->times < (nodes + j)->times)
        {
          tmp = j;
          break;
        }
      }
      // 將欲插入位置之後的nodes 往後移一個，並把合成parentNode 點放回陣列
      for (int k = index; k > tmp; k--)
        *(nodes + k) = *(nodes + (k-1));
      *(nodes + tmp) = *parentNode;
      index++;
      i++;
    }
  }
  return (nodes + index-1);
}
string* getHuffmanCode(node *root, int index, string *code){
  cout << "\n================\nPreorder visiting the Huffman Tree.\n================\n\nHuffman Code are as following:" << endl;
  preorder(root, code);
  for (int i = 0; i < 128; i++)
  {
    if (*(code + i) != "")
      cout << (char)i << ": " << *(code + i) << endl;
  }
  return code;
}
void preorder(node *ptr, string *code){
  // 前序拜訪 ！！！
  if (ptr && (ptr->lcd) && (ptr->rcd))
  {
    (*ptr->lcd).code = (*ptr).code + (*ptr->lcd).code;
    preorder(ptr->lcd, code);
    (*ptr->rcd).code = (*ptr).code + (*ptr->rcd).code;
    preorder(ptr->rcd, code);
  }
  else
    *(code + (int)ptr->symbol) = ptr->code;
}
void compress(fstream *infp, fstream *outfp, string *code){
  char a,b;
  int curlength;
  string curString = "";
  while(infp->get(a)){
    curString = curString + code[(int)a];
    if (curString.length() == 8){
      outfp->put((unsigned char)stoul(curString,nullptr,2));
      curString = "";
      curlength = curString.length();
    }
    else if (curString.length() < 8)
    {
      infp->get(a);
      curString = curString + code[(int)a];
      curlength = curString.length();
      if (curlength < 8)
      {
        infp->get(a);
        curString = curString + code[(int)a];
      }
      outfp->put((unsigned char)stoul(curString.substr(0,8),nullptr,2));
      curString = curString.substr(8, curlength-8);
      curlength = curString.length();
    }
    else{
      curlength = curString.length();
      outfp->put((unsigned char)stoul(curString.substr(0,8),nullptr,2));
      curString = curString.substr(8,curlength-8);
      curlength = curString.length();
    }
  }
  curlength = curString.length();
  for (int i = curlength; i <= 8 ; i++)
    curString += "1";
  curlength = curString.length();
  outfp->put((unsigned char)stoul(curString.substr(0,8),nullptr,2));
  cout << "Compression succeeded!" << endl;
}
void decompress(fstream *infp, fstream *outfp, string *code){
  char a;
  string curString;
  cout << "\nDecompressing succeeded\n" << endl;
  while(infp->get(a)){
    curString += bitset<8>((int)(unsigned char)a).to_string(); //to binary string
  }
  double length = curString.length();
  char symbol;
  int bitlength;
  string bit = "";
  for (double i = 0; i < length-8; i++)
  {
    bit += curString.substr(i,1);
    for (int j = 0; j < 128; j++)
    {
      if (bit == *(code + j))
      {
        symbol = (char)j;
        outfp->put(symbol);
        bit = "";
        break;
      }
    }
  }
  for (double i = length-8; i < length; i++)
  {
    bit += curString.substr(i,1);
    for (int j = 0; j < 128; j++)
    {
      if (bit == *(code + j))
      {
        symbol = (char)j;
        outfp->put(symbol);
        bit = "";
        break;
      }
    }
    if (bit.length() == 0)
      break;
  }
}
