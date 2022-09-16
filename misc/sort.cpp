#include <cmath>
#include <iostream>
#include <vector>

int a[] = {3, 44, 38, 5, 47, 151, 36, 26, 27, 2, 46, 61, 19, 50, 50, 48};
int n = sizeof(a) / sizeof(*a);

void bubbleSort() {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (a[i] > a[j]) {
                std::swap(a[i], a[j]);
            }
        }
    }
}

void selectionSort() {
    for (int i = 0; i < n - 1; ++i) {
        int min = INT_MAX, pos = -1;
        for (int j = i + 1; j < n; ++j) {
            if (a[j] < min) {
                min = a[j];
                pos = j;
            }
        }
        if (a[i] > a[pos]) {
            std::swap(a[i], a[pos]);
        }
    }
}

void insertionSort() {
    for (int i = 1; i < n; ++i) {
        int j = i;
        while (j > 0 && a[j - 1] > a[i]) {
            --j;
        }
        int tmp = a[i];
        for (int k = i; k > j; --k) {
            a[k] = a[k - 1];
        }
        a[j] = tmp;
    }
}

void mergeSort(int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(left, mid);
        mergeSort(mid + 1, right);
        std::vector<int> tmp(n, 0);
        int curr = left;
        int i = left;
        int j = mid + 1;
        while (i <= mid || j <= right) {
            if (i > mid) {
                tmp[curr++] = a[j++];
            } else if (j > right) {
                tmp[curr++] = a[i++];
            } else if (a[i] > a[j]) {
                tmp[curr++] = a[j++];
            } else {
                tmp[curr++] = a[i++];
            }
        }
        for (int k = left; k <= right; ++k) {
            a[k] = tmp[k];
        }
    }
}

void heapify(int root, int end) {
    int ptr = root, left = 2 * root + 1, right = 2 * root + 2;
    if (left < end && a[left] > a[ptr]) {
        ptr = left;
    }
    if (right < end && a[right] > a[ptr]) {
        ptr = right;
    }
    if (ptr != root) {
        std::swap(a[root], a[ptr]);
        heapify(ptr, end);
    }
}

void heapSort() {
    for (int i = n - 1; i >= 0; --i) {
        heapify(i, n);
    }
    for (int i = n - 1; i >= 0; --i) {
        std::swap(a[0], a[i]);
        heapify(0, i);
    }
}

void quickSort(int left, int right) {
    if (left >= right) return;
    int pivot = a[left + rand() % (right - left + 1)];
    int i = left, j = right;
    while (i <= j) {
        while (a[i] < pivot) ++i;
        while (a[j] > pivot) --j;
        if (i <= j) {
            std::swap(a[i++], a[j--]);
        }
    }
    quickSort(left, j);
    quickSort(i, right);
}

void radixSort() {
    int BASE = 10, exp = 1, max = INT_MIN;
    for (int i : a)
        if (max < i) max = i;
    while (pow(BASE, exp) <= max * BASE) {
        std::vector<std::vector<int>> tmp(BASE, std::vector<int>());
        for (int i : a)
            tmp[i % (int)pow(BASE, exp) / (int)pow(BASE, exp - 1)].push_back(i);
        int curr = 0;
        for (std::vector<int> i : tmp)
            for (int j : i)
                a[curr++] = j;
        ++exp;
    }
}

void printArray() {
    if (n <= 0) {
        return;
    }
    std::cout << "[";
    for (int i = 0; i < n - 1; ++i) {
        std::cout << a[i] << ", ";
    }
    std::cout << a[n - 1] << "]" << std::endl;
}

int main() {
    printArray();
    selectionSort();
    printArray();

    return 0;
}
