/*
    hw01
    系級: 資工二
    學號: 4104056009
    姓名: 洪浩祐
    email: jayhung97724@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define MAX_STACK_SIZE 255
#define MAX_COL 15
#define MAX_ROW 15

typedef struct {
  short int x;
  short int y;
  short int val;
  short int oval;
  short int next_x;
  short int next_y;
} point;                        //自定義一個名為point 的struct 

point pop();
void push(point p);
void stackEmpty();
void stackFull();
void printPath();
void iniPath();
void buildMaze(int[MAX_ROW][MAX_COL]);
void printMaze();
point pointize(int r,int c);
// function declaration

int top = -1;
int original_maze[MAX_ROW][MAX_COL]={0};

char succ_path[MAX_ROW][MAX_COL];
point path_stack[MAX_STACK_SIZE];

// 2 different directions!
int dir8[8][2]={{1,1},{0,1},{1,0},{1,-1},{-1,1},{0,-1},{-1,0},{-1,-1}};
int dir4[4][2]={{0,1},{1,0},{-1,0},{0,-1}};
// global variable

int main(int argc, char const *argv[])
{
  bool succeeded = false;
  int curx = 0, cury = 0;
  buildMaze(original_maze);
  printMaze();

  do{
    point apoint, outpoint;
    apoint = pointize(curx,cury);

    if ((apoint.x == 14) && (apoint.y == 14))
    {
      apoint.next_x = -1;
      apoint.next_y = -1;
      succeeded = true;
      push(apoint);
    }

    // 8 directions
    if ((apoint.val == 0) && (!succeeded))
    {
      int nohope = 0;
      for (int i = 0; i < 8; i++)
      {
        apoint.next_x = apoint.x + dir8[i][0];
        apoint.next_y = apoint.y + dir8[i][1];

        if (((original_maze[apoint.next_x][apoint.next_y] != 0) && (original_maze[apoint.next_x][apoint.next_y] != 2)) 
          || (apoint.next_y > MAX_ROW-1) || (apoint.next_x > MAX_COL-1) || (apoint.next_x < 0) || (apoint.next_y < 0))
        {
          nohope++;
        }  
        else 
        {
          //change the val of apoint to 3 if visited
          original_maze[apoint.x][apoint.y] = 3;
          push(apoint);
          curx = apoint.next_x;
          cury = apoint.next_y;
          break;
        }
        if (nohope == 8)
        {
          // change the val of apoint to 4 for dead entry ; pop the previous point to check again
          original_maze[apoint.x][apoint.y] = 4;
          outpoint = pop();
          curx = outpoint.x;
          cury = outpoint.y;
          original_maze[curx][cury] = outpoint.oval;
        }
      }
    }
    // 4 directions
    else if (apoint.val == 2 && (!succeeded))
    {
      int nohope = 0;
      for (int i = 0; i < 4; i++)
      {
        apoint.next_x = apoint.x + dir4[i][0];
        apoint.next_y = apoint.y + dir4[i][1];

        if (((original_maze[apoint.next_x][apoint.next_y] != 0) && (original_maze[apoint.next_x][apoint.next_y] != 2)) 
          || (apoint.next_y > MAX_ROW-1) || (apoint.next_x > MAX_COL-1) || (apoint.next_x < 0) || (apoint.next_y < 0))
        {
          nohope++;
        }  
        else 
        {
          //change the val of apoint to 3 if visited
          original_maze[apoint.x][apoint.y] = 3;
          push(apoint);
          curx = apoint.next_x;
          cury = apoint.next_y;
          break;
        }
        if (nohope == 4)
        {
          // change the val of apoint to 4 for dead entry ; pop the previous point to check again
          original_maze[apoint.x][apoint.y] = 4;       
          outpoint = pop();
          curx = outpoint.x;
          cury = outpoint.y;
          original_maze[curx][cury] = outpoint.oval;
        }
      }
    }
  } while (!succeeded);
  if (succeeded)
    printPath();
  else
    stackEmpty();
  return 0;
}
// ====================================================================================
void iniPath(){
  for (int i = 0; i < MAX_ROW; i++)
  {
    for (int j = 0; j < MAX_COL; j++)
      succ_path[i][j] = '*';
  }
}
void buildMaze(int original_maze[MAX_ROW][MAX_COL]){
  unsigned int seed = (unsigned int)time(NULL);
  srand(seed);
  int i,j;
  do
  {
    for(i=0;i<MAX_ROW;i++)
      for(j=0;j<MAX_COL;j++)
      {
        original_maze[i][j]=rand()%3;
      }
  }while(original_maze[0][0]==1||original_maze[MAX_ROW-1][MAX_COL-1]==1); 
  //If entry and exit are == 1,rebuild the maze. 
}
point pointize(int r,int c){
  // we use row, col to get positions/ store 
  point tempo;
  tempo.x = r;
  tempo.y = c;
  tempo.val = original_maze[r][c];
  tempo.oval = original_maze[r][c];
  return tempo;  
}
point pop(){
  if (top == -1)
    stackEmpty();
  return path_stack[top--];
}
void push(point p){
  if (top >= MAX_STACK_SIZE-1)
    stackFull();
  path_stack[++top] = p;
}
void stackFull(){
  printf("Path Stack is full, cannot add point!\n ending program...\n\n");
  exit(EXIT_FAILURE);
}
void stackEmpty(){
  printf("CANNOT find path TAT\nending program...\n\n");
  exit(EXIT_FAILURE);
}
void printMaze(){
  for (int i = 0; i < MAX_ROW; i++)
  {
    for (int j = 0; j < MAX_COL; j++)
      printf("%d ", original_maze[i][j]);
    printf("\n");
  }
  printf("\nRunning\n\n...\n\n...\n\n...\n\n\n");
}
void printPath(){
  iniPath();
  for (int i = 0; i <= top; i++)
    succ_path[path_stack[i].x][path_stack[i].y]='3';
  for (int i = 0; i < MAX_ROW; i++)
  {
    for (int j = 0; j < MAX_COL; j++)
      printf("%c ", succ_path[i][j]);
    printf("\n");
  }
  printf("\nI succeeded!!!\n");
}
