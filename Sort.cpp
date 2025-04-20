#include <iostream>
#include <omp.h>
using namespace std;

// ===== MERGE SORT FUNCTIONS =====
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort(arr, left, mid);

            #pragma omp section
            mergeSort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

// ===== BUBBLE SORT FUNCTION =====
void bubbleSort(int arr[], int n) {
    bool swapped;

    for (int i = 0; i < n - 1; ++i) {
        swapped = false;

        #pragma omp parallel for shared(arr, swapped)
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }
}

// ===== MAIN FUNCTION =====
int main() {
    int choice, n;

    cout << "Enter number of elements: ";
    cin >> n;

    int* arr = new int[n];
    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; ++i)
        cin >> arr[i];

    cout << "\nChoose sorting algorithm:\n";
    cout << "1. Merge Sort\n";
    cout << "2. Bubble Sort\n";
    cout << "Enter choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            #pragma omp parallel
            {
                #pragma omp single
                mergeSort(arr, 0, n - 1);
            }
            cout << "\nArray sorted using Merge Sort:\n";
            break;

        case 2:
            bubbleSort(arr, n);
            cout << "\nArray sorted using Bubble Sort:\n";
            break;

        default:
            cout << "Invalid choice!\n";
            delete[] arr;
            return 1;
    }

    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;

    delete[] arr;
    return 0;
}
