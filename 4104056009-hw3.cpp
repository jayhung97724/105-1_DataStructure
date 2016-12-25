/*
    hw03
    系級: 資工二
    學號: 4104056009
    Name: 洪浩祐 
    email: jayhung97724@gmail.com
*/
#include <iostream>
#include <queue>
#include <string>
#include <cstdlib>
#include <algorithm>
#define sizeX 11
#define sizeY 11
using namespace std;

struct node{
  int group;
  int x, y; 
  int canWalk;
  int depth;
  string data;
  bool visited;
};

void iniMaze(int num, node* maze);
void makeMap(node* maze);
void printMap(node* maze);
void printDepth(node* maze);
bool changeGroup(int x, int y, int newDepth, node* maze, int newGroup, int *load);
void printPath(node* maze);
void countDepth(node* maze);

int main(){
  double num=1;
  while(num){
    cout << "\n[11*11的迷宮]\n請輸入1~120的整數 (0結束): ";
    cin >> num;
    if(num == 0){
      cout << "Ending program ..." << endl;
      return 0;
    }
    else if((num >= 1 && num <= 120) && (num == (int)num)){
      cout << "Making maze ...\n" << endl;
      node* maze = new node[sizeX*sizeY];
      iniMaze(num, maze);
      makeMap(maze);
      printPath(maze);
      printMap(maze);
      printDepth(maze);
    }
    else 
      cout << "Something went wrong.\nTry again!\n" << endl;
  }
  
  return 0;
}
void iniMaze(int num, node* maze){
  
  cout << "\nOriginal Maze:" << endl;
  int posi = 0;
  
  for(int i=0; i < sizeX; i++){
    for(int j=0; j < sizeY; j++){
      maze[i*sizeY + j].canWalk = 0;
      maze[i*sizeY + j].x = i;
      maze[i*sizeY + j].y = j;
      maze[i*sizeY + j].depth = 0;
      maze[i*sizeY + j].group = -1;
    }
  }
  maze[(sizeX/2)*sizeY+(sizeY/2)].canWalk = 1;
  maze[(sizeX/2)*sizeY+(sizeY/2)].depth = 0;
  maze[(sizeX/2)*sizeY+(sizeY/2)].data = " o ";
  maze[(sizeX/2)*sizeY+(sizeY/2)].visited = 1;
  maze[(sizeX/2)*sizeY+(sizeY/2)].group = -1;
  while(num){
    posi = rand()%(sizeX*sizeY);
    if(maze[posi].canWalk == 0){
      maze[posi].canWalk = 1;
      num--;
    }
  }
  for(int i=0; i < sizeX; i++){
    for(int j=0; j < sizeY; j++)  
      cout << maze[i*sizeY + j].canWalk << " ";     
    cout << endl;
  }
}
void makeMap(node* maze){
  cout << "\nMaking map" << endl;
  int dir[4][2]={{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  int nextX, nextY;
  int *load = new int[4];
  // for (int i = 0; i < 4; i++) 
  load[0] = 0;
  load[1] = 1;
  load[2] = 2;
  load[3] = 3;
  node curNode = maze[60];
  queue<node> Q;
  Q.push(curNode);
  
  while(!Q.empty()){
    curNode = Q.front();
    for(int i=0; i<4; i++){
      nextX = curNode.x + dir[i][0];
      nextY = curNode.y + dir[i][1];
      if((nextX < sizeX) && (nextY < sizeY) && (nextX > -1) && (nextY > -1) && (maze[nextX*sizeY + nextY].canWalk == 1) ){
        if(!maze[nextX*sizeY + nextY].visited || changeGroup(nextX, nextY, curNode.depth + 1, maze, curNode.group, load)){
          maze[nextX*sizeY + nextY].visited = 1;
          maze[nextX*sizeY + nextY].group = (curNode.depth == 0)?i:curNode.group;
          maze[nextX*sizeY + nextY].data = " " + to_string(maze[nextX*sizeY + nextY].group) + " ";
          maze[nextX*sizeY + nextY].depth = curNode.depth + 1;
          load[maze[nextX*sizeY + nextY].group]++;
          Q.push(maze[nextX*sizeY + nextY]);
        }
      }
    }
    Q.pop();
  }
  for(int i=0; i < sizeX; i++){
    for(int j=0; j < sizeY; j++){
      if(!maze[i*sizeY + j].canWalk)
        maze[i*sizeY + j].data = " - ";
      else if(maze[i*sizeY + j].canWalk && !maze[i*sizeY + j].visited)
        maze[i*sizeY + j].data = "iso";
    }
  }
  for (int i = 0; i < 4; i++){
    load[i] -= i;
    cout << "load[" << i << "]: " << load[i] << endl;
  }
}
void printMap(node* maze){
  cout << "\n5. Map: " << endl;
  for(int i=0; i < sizeX; i++){
    for(int j=0; j < sizeY; j++)  
      cout << maze[i*sizeY + j].data;     
    cout << endl;
  }
}
void printDepth(node* maze){
  cout << "\n6. Depth: " << endl;
  for(int i=0; i < sizeX; i++){
    for(int j=0; j < sizeY; j++){
      cout << " " << maze[i*sizeY + j].depth;
      if(maze[i*sizeY + j].depth < 10) cout << " ";
    }
    cout << endl;
  }
}
bool changeGroup(int x, int y, int newDepth, node* maze, int newGroup, int *load){
  if (((x == 5 ) && (y == 5)) || (maze[x*sizeY + y].depth < newDepth)) return 0;
  else if(maze[x*sizeY + y].visited && (maze[x*sizeY + y].depth > newDepth) && (maze[x*sizeY + y].group != newGroup)){
    load[maze[x*sizeY + y].group]--;
    return 1;
  }
  else if (maze[x*sizeY + y].visited && (maze[x*sizeY + y].depth == newDepth) && (load[maze[x*sizeY + y].group] >= (load[newGroup]+1)) && (maze[x*sizeY + y].group != newGroup)){
    load[maze[x*sizeY + y].group]--;
    return 1;
  }
  else return 0;
}
void printPath(node* maze){
  int maxDepth[4] = {0};
  int NumOfG[5] = {0};
  int avgDepth[2] = {0, 1};
  cout << "\n1. Reported Path: \n";
  for (int group = 0; group < 4; group++)
  {
    cout << "\nPath" << group << " Hub links: ";
    for (int i = 0; i < sizeX*sizeY; i++)
    {
      if (maze[i].group == group && maze[i].depth != 0)
      {
        cout << "<" << maze[i].x << ", " << maze[i].y <<">\t";
        NumOfG[group]++;
        avgDepth[0] += maze[i].depth;
        avgDepth[1] ++;
        if (maze[i].depth > maxDepth[group])
          maxDepth[group] = maze[i].depth;
      }
    }
    cout << "\nPath" << group << " Hub數=" << NumOfG[group] << ", Max Depth=" << maxDepth[group] << endl;
  }
  cout << "\n2. Isolated Nodes: ";
  for (int i = 0; i < sizeX*sizeY; i++)
  {
    if (maze[i].data == "iso")
    {
      cout << "<" << maze[i].x << ", " << maze[i].y <<">\t";
      NumOfG[4]++;
    }
  }
  cout << "\n\tiso 個數=" << NumOfG[4] << endl;
  cout << "\n3. Average Depth: " << (double)avgDepth[0]/avgDepth[1] << endl;
  sort(maxDepth, maxDepth + 4);
  cout << "\n4. Max Depth: " << maxDepth[3] << endl;
}
