#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <queue>
using namespace std;

struct node {
  int row;
  int col;
  int val;
  int depth;
  int visited;
};

node maze[15][15];
const int dir[8][2] = {{0, 1}, {1, 0}, {0, (-1)}, {(-1), 0}, {1, 1}, {1, (-1)}, {(-1), (-1)}, {(-1), 1}};

int top = 0;
void pushStk(node *, node);
node popStk(node *);

void iniMaze();
void bfsMaze();
void printPath();

int main(int argc, char const *argv[])
{
  string cmd;
  do {
    iniMaze();
    bfsMaze();
    printPath();
  }while(cmd == "y" || cmd == "Y");
  return 0;
}

void iniMaze(){
  cout << "Original Maze:\n";
  srand((unsigned) time(NULL));
  for (int i = 0; i < 15; i++){
    for (int j = 0; j < 15; j++){
      maze[i][j].row = i;
      maze[i][j].col = j;
      maze[i][j].val = rand()%3;
      if (i == 0 && j == 0)
        maze[i][j].val = 0;
      cout << setw(3) << maze[i][j].val;
    }
    cout << endl;
  }
}
void bfsMaze(){
  for (int i = 0; i < 15; i++){
    for (int j = 0; j < 15; j++){
      maze[i][j].depth = 0;
      maze[i][j].visited = 0;
    }
  }
  queue<node> Q;
  maze[0][0].visited = 1;
  Q.push(maze[0][0]);
  node curNode, nextNode;
  while(!Q.empty()){
    curNode = Q.front();
    if (curNode.val == 0){
      for (int i = 0; i < 8; i++)
      {
        nextNode.row = curNode.row + dir[i][0];
        nextNode.col = curNode.col + dir[i][1];
        if (nextNode.row < 0 || nextNode.col < 0 || nextNode.row >= 15 || nextNode.col >= 15)
          continue;
        if ((maze[nextNode.row][nextNode.col].val != 1) && (maze[nextNode.row][nextNode.col].visited != 1))
        {
          maze[nextNode.row][nextNode.col].visited = 1;
          maze[nextNode.row][nextNode.col].depth = curNode.depth + 1;
          Q.push(maze[nextNode.row][nextNode.col]);
        }
      }
    }
    else if (curNode.val == 2){
      for (int i = 0; i < 4; i++)
      {
        nextNode.row = curNode.row + dir[i][0];
        nextNode.col = curNode.col + dir[i][1];
        if (nextNode.row < 0 || nextNode.col < 0 || nextNode.row >= 15 || nextNode.col >= 15)
          continue;
        if ((maze[nextNode.row][nextNode.col].val != 1) && (maze[nextNode.row][nextNode.col].visited != 1))
        {
          maze[nextNode.row][nextNode.col].visited = 1;
          maze[nextNode.row][nextNode.col].depth = curNode.depth + 1;
          Q.push(maze[nextNode.row][nextNode.col]);
        }
      }
    }
    Q.pop();
  }
  cout << "BFS done\n\nDepth:\n";
  for (int i = 0; i < 15; i++){
    for (int j = 0; j < 15; j++){
      cout << setw(3) << maze[i][j].depth;
    }
    cout << endl;
  }
  cout << endl;
}
void printPath(){
  if (maze[14][14].depth == 0){
    cout << "\nNo path TAT\n" << endl;
    return ;
  }
  cout << "\nShortest Path is:\n";
  node stk[5];
  top = 0;
  maze[14][14].val = 3;
  pushStk(stk, maze[14][14]);
  node curNode, nextNode;
  while(stk[top - 1].val != 1){
    curNode = popStk(stk);
    for (int i = 7; i >= 0; i--)
    {
      nextNode.row = curNode.row + dir[i][0];
      nextNode.col = curNode.col + dir[i][1];
      if (nextNode.row < 0 || nextNode.col < 0 || nextNode.row >= 15 || nextNode.col >= 15)
        continue;
      if (maze[nextNode.row][nextNode.col].depth == curNode.depth - 1)
      {
        if (maze[nextNode.row][nextNode.col].val == 2 && i >=4)
          continue;
        maze[nextNode.row][nextNode.col].val = 3;
        pushStk(stk, maze[nextNode.row][nextNode.col]);
        break;
      }
    }
  }
  for (int i = 0; i < 15; i++){
    for (int j = 0; j < 15; j++){
      if (maze[i][j].val != 3)
        cout << setw(3) << "*";
      else
        cout << setw(3) << maze[i][j].val;
    }
    cout << endl;
  }
  cout << endl;
}
void pushStk(node *stk, node curNode){
  stk[top++] = curNode;
}
node popStk(node *stk){
  node curNode;
  curNode = stk[--top];
  return curNode;
}