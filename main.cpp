#include <future>
#include <chrono>
#include <iostream>
#include <random>

using namespace std;

bool make_thread = false;

void merge(int* arr, int l, int m, int r)
{
    int nl = m - l + 1;
    int nr = r - m;

    int* left = new int[nl];
    int* right = new int[nr];

    for (int i = 0; i < nl; i++)
        left[i] = arr[l + i];
    for (int j = 0; j < nr; j++)
        right[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;  

    while (i < nl && j < nr) {

        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < nl) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < nr) {
        arr[k] = right[j];
        j++;
        k++;
    }
    delete[] left;
    delete[] right;
}

void mergeSort(int* arr, int l, int r) {
    if (l >= r)
        return;
    int m = (l + r - 1) / 2;
    if (make_thread && (r-l) > 10000) {
        auto f = async(launch::async, [&]() { mergeSort(arr, l, m); });
        auto f2 = async(launch::async, [&]() { mergeSort(arr, m + 1, r); });
        merge(arr, l, m, r);
    }
    else {
      
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main() {
    time_t start, end;

    //сортировка однопоточная
    time(&start);
    const int arrsize = 3000000;
    int* arr = new int[arrsize];
    for (int i = 0; i < arrsize; i++) {
        arr[i] = rand()/100;
    }
    cout << "array constructed" << endl;
    cout << endl;
    mergeSort(arr, 0, arrsize-1);
    cout << "array sorted by multi threads" << endl;
    time(&end);
    double seconds = difftime(end, start);
    printf("The time: %f seconds\n", seconds);

    //Сортировка многопоточная
    time(&start);
    make_thread = true;
    for (int i = 0; i < arrsize; i++) {
        arr[i] = rand() / 100;
    }
    cout << "array constructed" << endl;
    cout << endl;
    mergeSort(arr, 0, arrsize - 1);
    cout << "array sorted by solo thread" << endl;
    time(&end);
    seconds = difftime(end, start);
    printf("The time: %f seconds\n", seconds);

    delete[] arr;
	return 0;
}
