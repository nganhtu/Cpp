#include <iostream>
using namespace std;

//# viết hàm arr_sum
template <typename T>
T arr_sum(T *a, int aLen, T *b, int bLen)
{
    T res = 0;
    for (int i = 0; i < aLen; ++i)
    {
        res += a[i];
    }
    for (int i = 0; i < bLen; ++i)
    {
        res += b[i];
    }

    return res;
}

int main()
{
    int val;
    cin >> val;

    {
        int a[] = {3, 2, 0, val};
        int b[] = {5, 6, 1, 2, 7};
        cout << arr_sum(a, 4, b, 5) << endl;
    }
    {
        double a[] = {3.0, 2, 0, val * 1.0};
        double b[] = {5, 6.1, 1, 2.3, 7};
        cout << arr_sum(a, 4, b, 5) << endl;
    }

    return 0;
}
