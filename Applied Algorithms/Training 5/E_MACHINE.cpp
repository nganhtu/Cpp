#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n;
    cin >> n;
    vector<pair<long long, long long>> a(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> a[i].first >> a[i].second;
    }

    long long maxs[2000005] = {0};

    for (int i = 0; i < n; ++i)
    {
        maxs[a[i].second] = max(maxs[a[i].second], a[i].second - a[i].first);
    }
    for (int i = 1; i < n; ++i)
    {
        maxs[i] = max(maxs[i - 1], maxs[i]);
    }

    int res = -1;
    for (int i = 0; i < n; ++i)
    {
        if (res < a[i].second - a[i].first + maxs[a[i].first - 1])
        {
            res = a[i].second - a[i].first + maxs[a[i].first - 1];
        }
    }
    cout << res;

    return 0;
}
