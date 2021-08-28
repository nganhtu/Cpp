// 75/100, time limit exceeded

#include <bits/stdc++.h>

using namespace std;

int knapsac(vector<int> a, vector<int> c, int wLeft, int cost, int posi)
{
    if (wLeft < a[posi] || posi == a.size())
    {
        return cost;
    }
    return max(
        knapsac(a, c, wLeft, cost, posi + 1),
        knapsac(a, c, wLeft - a[posi], cost + c[posi], posi + 1));
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n, b;
    cin >> n >> b;
    vector<int> a(n), c(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> a[i] >> c[i];
    }

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (a[i] > a[j])
            {
                swap(a[i], a[j]);
                swap(c[i], c[j]);
            }
        }
    }
    cout << knapsac(a, c, b, 0, 0);

    return 0;
}
