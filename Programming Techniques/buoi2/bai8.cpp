#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

int main()
{
    int val1, val2;
    cin >> val1 >> val2;
    vector<vector<int>> a = {
        {1, 3, 7},
        {2, 3, 4, val1},
        {9, 8, 15},
        {10, val2},
    };

    for (unsigned int i = 0; i < a.size() - 1; ++i)
    {
        for (unsigned int j = i + 1; j < a.size(); ++j)
        {
            if ([](vector<int> arr) -> int {
                    int res = 0;
                    for (int i : arr)
                    {
                        res += i;
                    }
                    return res;
                }(a[i]) < [](vector<int> arr) -> int {
                    int res = 0;
                    for (int i : arr)
                    {
                        res += i;
                    }
                    return res;
                }(a[j]))
            {
                vector<int> tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
        }
    }

    for (const auto &v : a)
    {
        for (int it : v)
        {
            cout << it << ' ';
        }
        cout << endl;
    }
    return 0;
}
