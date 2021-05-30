#include <bits/stdc++.h>

using namespace std;

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

    return 0;
}
