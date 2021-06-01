#include <bits/stdc++.h>

using namespace std;

int dp[610][610];
int H[250];
int W[250];
int n;

int sol(int w, int h)
{
    int res = w * h;
    if (dp[w][h] != -1)
    {
        return dp[w][h];
    }
    for (int i = 0; i < n; i++)
    {
        if (w - W[i] >= 0 && h - H[i] >= 0)
        {
            res = min(res, sol(w - W[i], h) + sol(W[i], h - H[i]));
            res = min(res, sol(w, h - H[i]) + sol(w - W[i], H[i]));
        }
    }
    dp[w][h] = res;
    return dp[w][h];
}

int main()
{
    int w, h;
    cin >> w >> h;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> W[i] >> H[i];
    }
    memset(dp, -1, sizeof(dp));
    cout << sol(w, h) << endl;
}
