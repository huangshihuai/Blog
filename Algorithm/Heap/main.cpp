/**
 * left child: (parent * 2 + 1)
 * right child: (parent * 2 + 2)
 * prent index: floor((child - 1) / 2)
 * Index starts from 0
 */
#include <iostream> 
#include <functional>

using namespace std; 

typedef std::function<bool(int l, int r)> t_f_compare;

void buildHeap(int arr[], int len, t_f_compare);
void sortHeap(int arr[], const int len, t_f_compare f_compare);
void heapify(int arr[], const int len, const int index, t_f_compare f_compare);
void printArray(const int arr[], int len);

/**
 * Adjustment heap
 */
void heapify(int arr[], const int len, const int index, t_f_compare f_compare) {
    // TODO: root data
    int largest = index;
    int left = index * 2 + 1;
    int right = left + 1;
    if (left < len && f_compare(arr[left], arr[largest])) {
        largest = left;
    }
    if (right < len && f_compare(arr[right], arr[largest])) {
        largest = right;
    }
    if (index != largest) {
        std::swap(arr[index], arr[largest]);
        heapify(arr, len, largest, f_compare);
    }
}

/**
 * build heap
 *    [12, 11, 13, 5, 6, 7]
 *              ^
 *    <-[last parent node]
 */
void buildHeap(int arr[], int len, t_f_compare f_compare) {
    // TODO: last parent node
    int parent = (len - 2) / 2;

    // TODO: backtracking to the first node
    for (int index = int((len - 2) / 2); index >= 0; --index) {
        heapify(arr, len, index, f_compare);
    }
}

void sortHeap(int arr[], const int len, t_f_compare f_compare) {
    for (int index = len - 1; index > 0; index--) {
        std::cout << "sort num: " << len - index << std::endl;
        std::swap(arr[0], arr[index]);
        heapify(arr, index, 0, f_compare);
        printArray(arr, len);
    }
}
  
/* A utility function to print array of size n */
void printArray(const int arr[], int n) 
{ 
    for (int i=0; i<n; ++i) {
        cout << arr[i] << " ";
    } 
    cout << "\n"; 
}

// Driver program 
int main() 
{ 
    int arr[] = {12, 11, 13, 5, 6, 7}; 
    int n = sizeof(arr)/sizeof(arr[0]); 
    // TODO: Sort from small to large
    t_f_compare max_compare = [](int l, int r) { return l > r ? true : false; };
    // TODO: Sort from big to small
    t_f_compare min_compare = [](int l, int r) { return l > r ? false : true; };
    printArray(arr, n); 
    buildHeap(arr, n, min_compare);
    printArray(arr, n); 
    std::cout << "sort heap\n";
    sortHeap(arr, n, min_compare);
} 
