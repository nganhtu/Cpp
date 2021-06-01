#include <iostream>
using namespace std;
int f(int a[], int n)
{
    int flag = 1;
    for (int i = 0; i < n - 1; i++)
        if (a[i] > a[i + 1])
        {
            flag = 0;
            break;
        }
    return flag;
}
int main()
{
    int a[] = {-9, -1, 0, 3, 4, 10, 91};
    cout << f(a, 7);
    return 0;
}
