#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <ctime>
#include <sys/time.h>
#define INTERPOLATION 0
#define BINARY 1
#define SEQUENTIAL 2
using namespace std;

//Sunanth Sakthivel, CS350.
//PATH CHOSEN: Search (interpolation, binary, and sequential search).

//this function will dynamically allocate an array of the passed in number.
//dynamically allocated array is then loaded with random numbers using rand().
//Finally the array is sorted using a built in sort because interpolation,
//binary and sequential searches require the arrays to be presorted. 
void generate(int* & array, int number)
{
    int j;
    array = new int [number];
    for (int i = 0; i < number; ++i)
    {
	//this ensures that a random number is chosen between the range 0-(number*10)
	//and added to array. 
        j = 1 + (int)((number*10) * (rand()/(RAND_MAX + 1.0)));
        array[i] = j;
    }
    sort(array, array+number);	//sort the random array.
}

//this function performs the binary search recursively, index is returned if match found.
int binarysearch(int array[], int low, int high, int key, int & compare)
{
    if (low <= high)
    {
        int mid = (high-low)/2 + low;	//find the middle value using high and low index.
	//if middle value happens to be key the return value, otherwise divide and search 
	//recursively.
        if(array[mid] == key)
        {
	    ++compare; //one comparison made.
            return mid;
        }
        if (array[mid] > key)
        {
	    compare+=2;	//at this point 2 comparisons made.
            return binarysearch(array, low, mid-1, key, compare); //divide search.
        }
	compare+=2;	//at this point 2 comparisons made.
        return binarysearch(array, mid+1, high, key, compare); //divide search.
    }
    return -1;	//this means no match was found.
}

//this function performs a linear or sequential search for a passed in key and returns index if found.
int sequentialsearch(int array[], int size, int key, int & compare)
{
    int i;
    int flag = -1;
    //traverse the array starting from beginning and if key is found break and return. 
    for (i = 0; i < size && flag == -1; ++i)
    {
	++compare;	//one comparison made per loop iteration. 
        if (array[i] == key)
        {
            flag = i;
        }
    }
    return flag;
}

//this function performs a interpolation search recursively. Returns index if key is found. 
int interpolationsearch(int array[],int low, int high, int key, int & compare)
{
    if(key >= array[low] && key <= array[high] && high >= low)
    {
	int pos;
	//if the high index equals the low index, perform first if statement to avoid flt pt excep.
	if(high == low)
	{
	 	pos = 0;
	}
	else
	{
		//formula to find where we will be dividing (not in middle, but based on key). 
        	pos = low + (((double)(high-low)/(array[high]-array[low]))*(key-array[low]));
		//if above result gives an overflow then rearrange formula to ensure no overflow.
		if (pos < 0)
		{
			pos = low + (((key-array[low]) * (high-low)) / (array[high] - array[low]));
		}
	}
        if(array[pos] == key)
        {
	    ++compare;	//one comparison made.
            return pos;	//return index if found.
        }
        if(array[pos] < key)
        {
            compare+=2;	//two comparisons made at this point.
            return interpolationsearch(array, pos+1, high, key, compare); //divide search.
        }
	else
	{
	    compare+=2;	//two comparisons made at this point.
            return interpolationsearch(array, low, pos-1, key, compare); //divide search.
	}
    }
    return -1;	//indicates the key was not found.
}

int main()
{
    srand(time(NULL));	//start seed for random generator.
    int size = 8192;	//this is the starting size of N or the array.
    int key;	//this stores the key to be found in the searches.
    int* array;	//this will be the array that will by dynamically allocated into random sorted arrays.
    struct timeval tv1, tv2;	//these structs will be used to store times.
    double dur;	//this will be used to store the time duration of the search.
    int ROW = 6;	//ROW represents the number of times the array size will be sent through a multiplier.
    int COLUMN = 3;	//COLUMN represents either interpolation(0), binary(1), or sequential(2) search.
    int HEIGHT = 50;	//HEIGHT represents the number of trial runs that will be made per N size. 
    double results[ROW][COLUMN][HEIGHT];	//This array will store all the trial run times for the searches.
    double average[ROW][COLUMN];	//this array will store all the average times for the searches.
    int compare = 0;	//used to store the number of comparisons made in search.
    double compares[ROW][COLUMN][HEIGHT];	//this will store all the comparisons made for the searches.
    double compareavg[ROW][COLUMN];	//this will store all the average number of comparisons for the searches.
    int multiplier = 2;	//this is the multiplier that will be used to multiply the size per ROW iterations. 
    int i;	//used in nested for loops below.
    int j;	//used in nested for loops below.

    //this for loop will iterate ROW times to multiply the size of the array. The nested for within, will 
    //iterate HEIGHT times to perform that many number of trial runs for the particular N size. The values
    //for each trial run is stored in its respective array to be used later for average calculations.
    for (j = 0; j < ROW; ++j)
    {
        for(i = 0; i < HEIGHT; ++i)
        {
	    generate(array,size);	//generate the random array
	    int x = (int)(size * (rand() / (RAND_MAX + 1.0)));
	    key = array[x];	//choose a random index to look for a random key within the searches. 
	    
	    cout << "Key to search is: " << key << endl;
	    cout << "------------------------------------------------" << endl;
	    gettimeofday(&tv1, NULL);
	    cout << "Found at index: " << interpolationsearch(array,0,size-1,key,compare) << endl;
	    gettimeofday(&tv2, NULL);
	    //get the time it took to perform search.
	    dur = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
	    results[j][INTERPOLATION][i] = dur;	//store duration into 3d array.
	    compares[j][INTERPOLATION][i] = compare; //store comaparisons into 3d array.
            compare = 0;	//reset compare
	    cout << "Interpolation search: " << dur << " Seconds" << endl;
	    cout << "------------------------------------------------" << endl;
	    gettimeofday(&tv1, NULL);
	    cout << "Found at index: " << binarysearch(array,0,size-1,key,compare) << endl;
	    gettimeofday(&tv2, NULL);
	    //get the time it took to perform search.
	    dur = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
	    results[j][BINARY][i] = dur; //store duration into array.
            compares[j][BINARY][i] = compare; //store comparisons into array.
            compare = 0; //reset compare
	    cout << "Binary search: " << dur << " Seconds" << endl;
	    cout << "------------------------------------------------" << endl;
	    gettimeofday(&tv1, NULL);
	    cout << "Found at index: " << sequentialsearch(array,size,key,compare) << endl;
	    gettimeofday(&tv2, NULL);
	    //get the time it took to perform search.
	    dur = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
	    results[j][SEQUENTIAL][i] = dur; //store duration into array.
            compares[j][SEQUENTIAL][i] = compare; //store comparisons into array.
            compare = 0; //reset compare.
	    cout << "Sequential search: " << dur << " Seconds" << endl;
	    cout << "------------------------------------------------" << endl;
	    delete [] array;	//deallocate array to get ready to reallocate new array on next iteration.
	    array = NULL;
         }
         size = size * multiplier;	//multiply the size of array by multiplier.
     }   
     if(array)
     {
	 delete[]array;
     } 
     double sum = 0;
     double avgcomp = 0;
     //this for loop will add up all the trial runs and compute the average runtime and average number
     //of comparisons and store into respective arrays.
     for(j = 0; j < ROW; ++j)
     {
	    for(i = 0; i < HEIGHT; ++i)
	    {
		 sum += results[j][INTERPOLATION][i];
		 avgcomp += compares[j][INTERPOLATION][i];
	    }
	    average[j][INTERPOLATION] = sum/HEIGHT;
            compareavg[j][INTERPOLATION] = avgcomp/HEIGHT;
	    sum = 0;
            avgcomp = 0;
	    for(i = 0; i < HEIGHT; ++i)
	    {
		 sum += results[j][BINARY][i];
		 avgcomp += compares[j][BINARY][i];
	    }
	    average[j][BINARY] = sum/HEIGHT;
            compareavg[j][BINARY] = avgcomp/HEIGHT;
	    sum = 0;
            avgcomp = 0;
	    for(i = 0; i < HEIGHT; ++i)
	    {
		 sum += results[j][SEQUENTIAL][i];
		 avgcomp += compares[j][SEQUENTIAL][i];
	    }
	    average[j][SEQUENTIAL] = sum/HEIGHT;
	    compareavg[j][SEQUENTIAL] = avgcomp/HEIGHT;
	    sum = 0;
            avgcomp = 0;
    }
    //this for loop will display all the average results of the runtime and number of comparisons based on the
    //size of the array when performing searches. 
    cout << "-----------------------------------------------------------------------" << endl;
    int count = 8192;
    for (i = 0; i < ROW; ++i)
    {
	cout << "Interpolation average for N = " << count << " is: " << average[i][INTERPOLATION] << " seconds" << endl;
	cout << "Binary search average for N = " << count << " is: " << average[i][BINARY] << " seconds" << endl;
 	cout << "Sequential search average for N = " << count << " is: " << average[i][SEQUENTIAL] << " seconds" << endl;
	cout << "__________________________" << endl;
	count *= multiplier;
    }
    cout << "------------------------------------------------------------------------" << endl;
    count = 8192;
    for (i = 0; i < ROW; ++i)
    {
	cout << "Interpolation average compares for N = " << count << " is: " << compareavg[i][INTERPOLATION] << endl;
	cout << "Binary search average compares for N = " << count << " is: " << compareavg[i][BINARY] << endl;
 	cout << "Sequential search average compares for N = " << count << " is: " << compareavg[i][SEQUENTIAL] << endl;
	cout << "__________________________" << endl;
	count *= multiplier;
    }

    return 0;
}

