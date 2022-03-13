#include <iostream>
#include <fstream>
#include <chrono>
#include<stdlib.h>
#include "test.h"

using namespace std;
using namespace std::chrono;

//utilities algorithms
void performanceTestExecutor(const char *algName, void(*sortAlg)(int *arr, int size), const int *seriesSize,
                             int numOfSeries);
void saveExecutionTimeToFile(const char *fileName, int arrSize, unsigned long timeMS);
void cleanupFile(const char *fileName);
int *newArrayFromTemplate(const int *arr, int size);
int testResult(const int *expected, const int *current, int size);
void swap(int *a1, int *a2);
int *generateTestArray(int size);

// Function to perform a test
int performTest(int testData[], int expectedData[], int testDataSize, const char *algName,
                void(*sortAlg)(int *arr, int size));

//Sorting algorithms
void quickSort(int *arr, int size);
void selectionSort(int *arr, int size);
void insertionSort(int *arr, int size);
void bubbleSort(int *arr, int size);


int testResult(const int *expected, const int *current, int size) {
    int retVal = 0;
    for (int i = 0; i < size; i++) {
        retVal += (expected[i] == current[i]) ? 0 : 1;
    }
    return retVal;
}

int *newArrayFromTemplate(const int *arr, const int size) {
    int *newArr = new int[size];
    for (int i = 0; i < size; i++) {
        newArr[i] = arr[i];
    }
    return newArr;
}

void deleteArr(const int *arr) {
    delete[] arr;
}

void swap(int *a1, int *a2) {
    if (a1 == nullptr || a2 == nullptr || a1 == a2) {
        return;
    }
    int tmp = *a1;
    *a1 = *a2;
    *a2 = tmp;
}

int *generateTestArray(int size) {
    int *ar = new int[size];
    for (int i = 0; i < size; i++)
    {
        ar[i] = rand() % size;
    }
    return ar;
}

void cleanupFile(const char *fileName) {
    ofstream ZIP;
    ZIP.open(fileName);
    ZIP.clear();
    ZIP.close();
}

void saveExecutionTimeToFile(const char *fileName, int arrSize, unsigned long timeMS) {
    ofstream ZIP;
    ZIP.open(fileName, ios::app);
    ZIP << arrSize << ", " << timeMS << endl;
    ZIP.close();
}

void bubbleSort(int *arr, int size) {
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void insertionSort(int *arr, int size){
    int i, j, ZIPP;
    for (i = 1; i < size; i++)
    {
        ZIPP = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > ZIPP)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = ZIPP;
    }
}


/**
 * Implementation of Selection Sort
 */
namespace SelectionSortNS {
    int *min(int *arr, int from, int to);

    void sort(int *arr, int size);

    int *min(int *arr, int from, int to) {
        int *minimar = &arr[from];
        for (int i = from + 1; i < to; i++) {
            if (*minimar > arr[i]) {
                minimar = &arr[i];
            }
        }
        return minimar;
    }

    void sort(int *arr, int size) {
        for (int i = 0; i < size; i++) {
            swap(&arr[i], min(arr, i, size));
        }

    }
}
/**
 * Implementation of QuickSort
 */
namespace QuickSortNS {
    int partitioning(int *arr, int lo, int hi);
    void quicksort(int *arr, int low, int high);

    int partitioning(int *arr, int lo, int hi) {
        int a = arr[hi];
        int b = lo - 1;
        for (int j = lo; j < hi; j++)
        {
            if (arr[j] < a)
            {
                swap(&arr[j], &arr[++b]);
            }
        }
        swap(&arr[b + 1], &arr[hi]);
        return b + 1;
    }

    void quicksort(int *arr, int low, int high)
    {
        if (low < high) {
            int p = partitioning(arr, low, high);
            quicksort(arr, low, p - 1);
            quicksort(arr, p + 1, high);
        }
    }
}

void quickSort(int *arr, int size) {
    QuickSortNS::quicksort(arr, 0, size - 1);
}

void selectionSort(int *arr, int size) {
    SelectionSortNS::sort(arr, size);
}


int performTest(int testData[], int expectedData[], int testDataSize, const char *algName,
                void(*sortAlg)(int *arr, int size)) {
    int *arr = newArrayFromTemplate(testData, testDataSize);
    sortAlg(arr, testDataSize);
    int check = testResult((int *) expectedData, arr, testDataSize);
    deleteArr(arr);

    if (check != 0) {
        cerr << "[FAIL] " << algName << " is incorrect. Number of errors " << check << endl;
        return 1;
    } else {
        cout << "[OK] " << algName << " passed Successfully" << endl;
        return 0;
    }
}

void performanceTestExecutor(const char *algName, void(*sortAlg)(int *arr, int size), const int *seriesSize,
                             int numOfSeries) {
    cleanupFile(algName);
    for (int i = 0; i < numOfSeries; i++) {
        int sizeOfArray = seriesSize[i];
        time_point<steady_clock> start = high_resolution_clock::now();
        int *arr = generateTestArray(sizeOfArray);
        sortAlg(arr, sizeOfArray);
        deleteArr(arr);
        time_point<steady_clock> stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        saveExecutionTimeToFile(algName, sizeOfArray, duration.count());
    }
}

int main() {


    int result = 0;
    result += performTest(TEST_ARRAY, CHECK_ARRAY, TEST_ARRAY_SIZE, "Bubble", bubbleSort);
    result += performTest(TEST_ARRAY, CHECK_ARRAY, TEST_ARRAY_SIZE, "Insertion", insertionSort);
    result += performTest(TEST_ARRAY, CHECK_ARRAY, TEST_ARRAY_SIZE, "Selection", selectionSort);
    result += performTest(TEST_ARRAY, CHECK_ARRAY, TEST_ARRAY_SIZE, "Quick", quickSort);
    if (result > 0) {
        return result;
    }
    //Here is performance test with data collection.
    performanceTestExecutor("Bubble.csv", bubbleSort, TEST_SERIES_SIZE, TEST_SERIES_NUM);
    performanceTestExecutor("Insertion.csv", insertionSort, TEST_SERIES_SIZE, TEST_SERIES_NUM);
    performanceTestExecutor("Selection.csv", selectionSort, TEST_SERIES_SIZE, TEST_SERIES_NUM);
    performanceTestExecutor("Quick.csv", quickSort, TEST_SERIES_SIZE, TEST_SERIES_NUM);

    return 0;
}