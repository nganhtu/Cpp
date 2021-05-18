#include <iostream>
using namespace std;
#define pt 6
int main()
{
    int a[pt] = {2, -9, 3, -25, 12, 5};
    int i = 0;
    while (i < pt)
    {
        if (a[i] < 0)
            cout << a[i] << " ";
        i++;
    }
}
