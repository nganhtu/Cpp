#include <iostream>

int a[] = {2, 3, 5, 19, 26, 27, 36, 38, 44, 46, 47, 48, 50, 50, 61, 151};
int key = 19;

int n = sizeof(a) / sizeof(*a);

int binarySearch() {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (a[mid] > key) {
            right = mid - 1;
        } else if (a[mid] < key) {
            left = mid + 1;
        } else {
            return mid;
        }
    }
    return -1;
}

int main() {
    std::cout << binarySearch();

    return 0;
}
