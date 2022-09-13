#include <iostream>

int a[] = {2, 3, 5, 19, 26, 27, 36, 38, 44, 46, 47, 48, 50, 50, 61, 151};
int key = 19;

int n = sizeof(a) / sizeof(*a);

int binarySearch(int left, int right) {
    if (left > right) {
        return -1;
    }
    int mid = (left + right) / 2;
    if (a[mid] > key) {
        return binarySearch(left, mid - 1);
    }
    if (a[mid] < key) {
        return binarySearch(mid + 1, right);
    }
    return mid;
}

int main() {
    std::cout << binarySearch(0, n - 1);

    return 0;
}
