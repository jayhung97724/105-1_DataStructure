#include <iostream>
#include <string>
#include <string.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

struct info {
  string date;
  string place;
  int num;
  int sum;
  int average;
};
void filterData(string fileName);
int countAverageReturnIndex(string fileName, info *infoArray);
int countSum(info *infoArray, info *sumArray, int index);
void copyForSorting(info *infoArray, int index, info *newArray);
void printArray(info *array, int index);
void bubbleSort(info *array, int index);
void quickSort(info *array, int left, int right);
void mergeSort(info *array, int lowerBound, int upperBound);
void merge(info *array, int lowerBound, int upperBound, int mid);
void swap(info *array, int i, int j);
double diffMiliSec(clock_t start, clock_t end);
void writeFile(int index, int sumIndex, info *sumArray, info *bubbleArray, info *mergeArray, info *quickArray, double *time);
int main(int argc, char const *argv[])
{
  
  clock_t bt1, bt2, mt1, mt2, qt1, qt2;
  double *time = new double[3];
  string infileName = "2015taiwan.txt";
  filterData(infileName);

  info *infoArray = new info[30000];
  info *sumArray = new info[100];
  int index = 0;
  int sumIndex = 0;
  index = countAverageReturnIndex("filtered2015taiwan.txt", infoArray);
  sumIndex = countSum(infoArray, sumArray, index);
  quickSort(sumArray, 0, sumIndex - 1);
  cout << "\nAverage evaluated. " << index << " pieces of info in total\n";
  cout << "\nSum of purple days evaluated. " << sumIndex << " stops in total\n";
  
  info *bubbleArray = new info[30000];
  info *mergeArray  = new info[30000];
  info *quickArray  = new info[30000];

  cout << "\nCopying data for sorting...";
  copyForSorting(infoArray, index, bubbleArray);
  copyForSorting(infoArray, index, mergeArray);
  copyForSorting(infoArray, index, quickArray);
  cout << "\nBubble Sorting...";
  bt1 = clock();
  bubbleSort(bubbleArray, index);
  bt2 = clock();
  time[0] = diffMiliSec(bt1, bt2);
  cout << "\nBubble Sorting done.\n"; 
  
  cout << "\nMerge Sorting...";
  mt1 = clock();
  mergeSort(mergeArray, 0, index - 1);
  mt2 = clock();
  time[1] = diffMiliSec(mt1, mt2);
  cout << "\nMerge Sorting done.\n";
  
  cout << "\nQuick Sorting...";
  qt1 = clock();
  quickSort(quickArray, 0, index - 1);
  qt2 = clock();
  time[2] = diffMiliSec(qt1, qt2);
  cout << "\nQuick Sorting done.\n";
  writeFile(index, sumIndex, sumArray, bubbleArray, mergeArray, quickArray, time);

  delete [] quickArray;
  delete [] mergeArray;
  delete [] bubbleArray;
  delete [] infoArray;
  delete [] sumArray;
  return 0;
}
void filterData(string fileName){
  cout << "\nOpening " << fileName << " to read in data..." << endl;
  fstream infp, outfp;
  infp.open(fileName);
  outfp.open("filtered"+fileName, ios::out);

  cout << "\nFiltering data ...\n";
  string firstLine;
  getline(infp, firstLine);

  string filteredLine;
  char *newLine = new char[256];
  char *token;
  bool ignored;
  int col;
  while(infp.getline(newLine, 256)){
    token = strtok (newLine,",");
    col = 0;
    ignored = false;
    filteredLine="";
    while (token != NULL)
    {
      if (col < 2){
        string tmp(token);
        filteredLine = filteredLine + tmp + ",";
      }
      if (col == 2 && strcmp(token,"PM2.5") != 0){
        ignored = true;
        break;
      }
      else if(col == 2 && strcmp(token,"PM2.5") == 0)
        filteredLine = filteredLine + "PM2.5";
      if (col > 2)
      {
        stringstream ss;
        ss << atoi(token);
        string tmp = ss.str();
        filteredLine = filteredLine + "," + tmp;
        ss.str("");
      }
      token = strtok (NULL, ",");
      col++;
    }
    if (!ignored)
      outfp << filteredLine << "\n";
  }
  infp.close();
  outfp.close();
  cout << "\n\"filtered" << fileName << "\" was written." << endl;
}
int countAverageReturnIndex(string fileName, info *infoArray){
  cout << "\nOpening \"" << fileName << "\" to count average..." << endl;
  fstream infp;
  infp.open(fileName);

  char *newLine = new char[256];
  char *token;
  int col;
  int index = 0;
  string tmp;
  while(infp.getline(newLine, 256)){
    token = strtok (newLine,",");
    col = 0;
    while (token != NULL)
    {
      switch(col) {
        case 0: tmp.assign(token);
                infoArray[index].date = tmp;
                tmp.clear();
                break;
        case 1: tmp.assign(token);
                infoArray[index].place = tmp;
                tmp.clear();  
                break;
        case 2: break;

        default:  infoArray[index].sum += atoi(token);
                  infoArray[index].num ++;
      }
      token = strtok (NULL, ",");
      col++;
    }
    infoArray[index].average = infoArray[index].sum / infoArray[index].num;
    // cout << index << "\t:" << infoArray[index].date << "," << infoArray[index].place << ": " << infoArray[index].average << endl;
    if(infoArray[index].average > 0)
     index ++;
  }
  infp.close();
  return index;
}
int countSum(info *infoArray, info *sumArray, int index){
  int sumIndex=0;
  sumArray[sumIndex].place = infoArray[0].place;
  for (int i = 0; i < index; i++)
  {
    if (infoArray[i].average >= 60)
    {
      if (infoArray[i].place == sumArray[sumIndex].place)
        sumArray[sumIndex].average ++;
      else {
        sumIndex++;
        sumArray[sumIndex].place = infoArray[i].place;
        sumArray[sumIndex].average ++;
      }
    }
  }
  return sumIndex++;
}
void copyForSorting(info *infoArray, int index, info *newArray){
  // cout << "\nCopying ...\n";
  for (int i = 0; i < index; i++)
    *(newArray + i) = *(infoArray + i);
}
void printArray(info *array, int index){
  for (int i = 0; i < index; i++){
    cout << i << "\t:" << array[i].place << "," << array[i].date << ": " << array[i].average << endl;
  }
}
void swap(info *array, int i, int j){
  info tmp;
  tmp      = array[i];
  array[i] = array[j];
  array[j] = tmp;
}
void bubbleSort(info *array, int index){
  for (int i = 0 ; i < (index - 1); i++)
  {
    for (int j = 0 ; j < index - i - 1; j++)
    {
      if (array[j].average < array[j+1].average)
        swap(array, j, j+1);
    }
  }
}
void mergeSort(info *array, int lowerBound, int upperBound)
{
    int mid;
    if (upperBound > lowerBound)
    {
        mid = ( lowerBound + upperBound) / 2;
        mergeSort(array, lowerBound, mid);
        mergeSort(array, mid + 1, upperBound);
        merge(array, lowerBound, upperBound, mid);
    }
}
void merge(info *array, int lowerBound, int upperBound, int mid)
{
    info* leftArray = NULL;
    info* rightArray = NULL;
    int i, j, k;
    int n1 = mid - lowerBound + 1;
    int n2 = upperBound - mid;
    leftArray = new info[n1];
    rightArray = new info[n2];
    for (i = 0; i < n1; i++)
        leftArray[i] = array[lowerBound + i];
    for (j = 0; j < n2; j++)
        rightArray[j] = array[mid + 1 + j];

    i = 0;
    j = 0;
    k = lowerBound;

    while (i < n1 && j < n2)
    {
        if (leftArray[i].average >= rightArray[j].average)
        {
            array[k] = leftArray[i];
            i++;
        }
        else
        {
            array[k] = rightArray[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        array[k] = leftArray[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        array[k] = rightArray[j];
        j++;
        k++;
    }
    delete [] leftArray;
    delete [] rightArray;
}
void quickSort(info *array, int left, int right){
      int i = left, j = right;
      int pivot = array[(left + right) / 2].average;
 
      /* partition */
      while (i <= j) {
        while(array[i].average > pivot)
          i++;
        while(array[j].average < pivot)
          j--;
        if(i <= j){
          swap(array, i, j);
          i++;
          j--;
        }
      };
      /* recursion */
      if (left < j)
            quickSort(array, left, j);
      if (i < right)
            quickSort(array, i, right);
}
double diffMiliSec(clock_t start, clock_t end){
  double ticks = end - start;
  double diffms = ticks / ( CLOCKS_PER_SEC / 1000 );
  return diffms;
}
void writeFile(int index, int sumIndex, info *sumArray, info *bubbleArray, info *mergeArray, info *quickArray, double *time){
  string sortedFileName = "Outcome.txt";
  cout << "\nOpening \"" << sortedFileName << "\" to write.\n";
  fstream outfp;
  outfp.open(sortedFileName, ios::out);

  outfp << "Bubble:\n===================\n";
  for (int i = 0; i < index; i++)
    outfp << i << "\t:" << bubbleArray[i].date << "," << bubbleArray[i].place << ": " << bubbleArray[i].average << endl;
  outfp << "\nMerge:\n===================\n";
  for (int i = 0; i < index; i++)
    outfp << i << "\t:" << mergeArray[i].date << "," << mergeArray[i].place << ": " << mergeArray[i].average << endl;
  outfp << "\nQuick:\n===================\n";
  for (int i = 0; i < index; i++)
    outfp << i << "\t:" << quickArray[i].date << "," << quickArray[i].place << ": " << quickArray[i].average << endl;

  outfp << "\n\t===================\n";
  outfp << "Bubble Sort done in: " << time[0] << " milliseconds.\n";
  outfp << "Merge Sort done in: " << time[1] << " milliseconds.\n";
  outfp << "Quick Sort done in: " << time[2] << " milliseconds.\n";

  outfp << "\nPurple Days Result:\n";
  for (int i = 0; i < sumIndex; i++)
    outfp << sumArray[i].place << ": " << sumArray[i].average << endl;
  outfp.close();
  cout << "\nGo check \"" << sortedFileName << "\".\n";
}