#include <bits/stdc++.h>

using namespace std;

int nextStat(vector<int> &perm, int n)
{
    if (n < 2)
    {
        return -1;
    }
    int p = n - 2, q = n - 1;
    while (perm[p] > perm[p + 1] && p >= 0)
    {
        --p;
    }
    if (p == -1)
    {
        return -1;
    }
    while (perm[q] < perm[p])
    {
        --q;
    }
    swap(perm[p], perm[q]);
    queue<int> tail;
    for (int i = n - 1; i > p; --i)
    {
        tail.push(perm[i]);
    }
    for (int i = p + 1; i < n; ++i)
    {
        perm[i] = tail.front();
        tail.pop();
    }
    return 0;
}

int calc(vector<vector<int>> cost, vector<int> perm, int n)
{
    int res = 0;
    for (int i = 0; i < n; ++i)
    {
        int tmp = cost[perm[i]][perm[(i + 1) % n]];
        if (tmp == -1)
        {
            return -1;
        }
        res += tmp;
    }
    return res;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> cost(n, vector<int>(n, -1));
    for (int uwu = 0; uwu < m; ++uwu)
    {
        int i, j, c;
        cin >> i >> j >> c;
        cost[i - 1][j - 1] = c;
    }

    int res = -1;
    vector<int> perm(n);
    for (int i = 0; i < n; ++i)
    {
        perm[i] = i;
    }
    do
    {
        int tmp = calc(cost, perm, n);
        if (res < tmp)
        {
            res = tmp;
        }
    } while (nextStat(perm, n) != -1);

    if (res == -1)
    {
        cout << "WTF";
    }
    else
    {
        cout << res;
    }

    return 0;
}
