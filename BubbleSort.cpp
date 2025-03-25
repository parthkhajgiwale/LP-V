#include <iostream>
#include <omp.h>

using namespace std;

void sequentialBubbleSort(int *, int);
void parallelBubbleSort(int *, int);
void swap(int &, int &);

void sequentialBubbleSort(int *a, int n) {
    int swapped;
    for (int i = 0; i < n; i++) {
        swapped = 0;
        for (int j = 0; j < n - 1; j++) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped)
            break;
    }
}

void parallelBubbleSort(int *a, int n) {
    int swapped;
    for (int i = 0; i < n; i++) {
        swapped = 0;
        int first = i % 2;
        #pragma omp parallel for shared(a, first)
        for (int j = first; j < n - 1; j++) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped)
            break;
    }
}

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    int *a, n;

    cout << "Enter total number of elements: ";
    cin >> n;
    a = new int[n];

    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    // Sequential Bubble Sort
    double start_time = omp_get_wtime();
    sequentialBubbleSort(a, n);
    double end_time = omp_get_wtime();
    cout << "\nSorted array (Sequential): ";
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << "\nTime taken by sequential algorithm: " << end_time - start_time << " seconds" << endl;

    // Parallel Bubble Sort
    cout << "\nEnter elements again for parallel sorting: ";
    for (int i = 0; i < n; i++) {
        cin >> a[i]; // Re-input for fair comparison
    }

    start_time = omp_get_wtime();
    parallelBubbleSort(a, n);
    end_time = omp_get_wtime();
    cout << "\nSorted array (Parallel): ";
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << "\nTime taken by parallel algorithm: " << end_time - start_time << " seconds" << endl;

    delete[] a; // Free allocated memory
    return 0;
}
