#include <bits/stdc++.h>

using namespace std;

int t, l, c, a[1000][1000], b[1000][1000];

void solve()
{
    int res = 0;
    for (int h = -l + 1; h < l; ++h)
    {
        for (int v = -c + 1; v < c; ++v)
        {
            int cnt = 0;
            for (int i = max(0, -h); i < min(l, l - h); ++i)
            {
                for (int j = max(0, -v); j < min(c, c - v); ++j)
                {
                    if (a[i][j] == 1 && b[i + h][j + v] == 1)
                    {
                        cnt++;
                    }
                }
            }
            if (res < cnt)
            {
                res = cnt;
            }
        }
    }
    cout << res << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> t;
    for (int uwu = 0; uwu < t; ++uwu)
    {
        cin >> l >> c;
        for (int i = 0; i < l; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                cin >> a[i][j];
            }
        }
        for (int i = 0; i < l; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                cin >> b[i][j];
            }
        }

        solve();
    }

    return 0;
}
