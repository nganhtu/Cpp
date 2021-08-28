// 50/100, time limit exceeded

#include <bits/stdc++.h>

using namespace std;

int minCost = 4e8;

void solve(vector<vector<int>> arr, vector<bool> pass, int cnt, int cost, int pos)
{
    if (cnt == 0)
    {
        if (pos == 1)
        {
            minCost = (cost < minCost) ? cost : minCost;
        }
        return;
    }
    if (cost >= minCost)
    {
        return;
    }
    for (int i = 0; i < arr.size(); ++i)
    {
        if (arr[i][0] == pos && !pass[arr[i][1]])
        {
            pass[arr[i][1]] = true;
            solve(arr, pass, cnt - 1, cost + arr[i][2], arr[i][1]);
            pass[arr[i][1]] = false;
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> arr(m, vector<int>(3));
    for (int i = 0; i < m; ++i)
    {
        cin >> arr[i][0] >> arr[i][1] >> arr[i][2];
    }

    sort(arr.begin(), arr.end());
    vector<bool> pass(n + 1, false);
    int cnt = n, cost = 0;
    solve(arr, pass, cnt, cost, 1);
    cout << minCost;

    return 0;
}
