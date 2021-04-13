#include <bits/stdc++.h>

using namespace std;

void solve()
{
    int aDeg, bDeg;
    cin >> aDeg;
    vector<unsigned int> a(aDeg + 1);
    for (int i = 0; i <= aDeg; ++i)
    {
        cin >> a[i];
    }
    cin >> bDeg;
    vector<unsigned int> b(bDeg + 1);
    for (int i = 0; i <= bDeg; ++i)
    {
        cin >> b[i];
    }
    unsigned int res = 0;
    for (int i = aDeg + bDeg; i >= 0; --i)
    {
        unsigned int tmp = 0;
        for (int j = 0; j <= i; ++j)
        {
            if (j <= aDeg && i - j <= bDeg)
            {
                tmp += a[j] * b[i - j];
            }
        }
        res ^= tmp;
    }
    cout << res;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    solve();

    return 0;
}
