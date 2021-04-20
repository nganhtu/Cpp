#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> a[i];
    }
    fflush(stdin);
    vector<pair<int, int>> r;
    r.push_back({a[0], 0});
    bool up = true;
    int tmp = 0;
    while (a[tmp] == a[tmp + 1] && tmp < n - 1)
    {
        ++tmp;
    }
    if (tmp == n - 1)
    {
        cout << "0 0";
    }
    else
    {
        if (a[tmp] > a[tmp + 1])
        {
            up = false;
        }
        while (tmp < n - 1)
        {
            if (up)
            {
                while (a[tmp] <= a[tmp + 1] && tmp < n - 1)
                {
                    ++tmp;
                }
                r.push_back({a[tmp], tmp});
                up = false;
            }
            else
            {
                while (a[tmp] >= a[tmp + 1] && tmp < n - 1)
                {
                    ++tmp;
                }
                r.push_back({a[tmp], tmp});
                up = true;
            }
        }

        int high = 0, low = 0;
        for (int i = 1; i < r.size() - 1; ++i)
        {
            if (r[i].first > r[i - 1].first)
            {
                high = max(high, min(r[i].second - r[i - 1].second, r[i + 1].second - r[i].second));
            }
            else
            {
                low = max(low, min(r[i].second - r[i - 1].second, r[i + 1].second - r[i].second));
            }
        }
        cout << high << " " << low;
    }
    return 0;
}
