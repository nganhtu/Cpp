#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int testcase;
    cin >> testcase;
    for (int i = 0; i < testcase; ++i)
    {
        const double PI = acos(-1);
        int n, f;
        cin >> n >> f;
        vector<int> powr2(n);
        for (int j = 0; j < n; ++j)
        {
            cin >> powr2[j];
            powr2[j] *= powr2[j];
        }
        double min = 0, max = 1e8, mid;
        while (min + 1e-7 < max)
        {
            mid = (min + max) / 2;
            int cnt = 0;
            for (int k = 0; k < n; ++k)
            {
                cnt += floor(powr2[k] / mid);
            }
            if (cnt < f + 1)
            {
                max = mid;
            }
            else
            {
                min = mid;
            }
        }
        cout << fixed << setprecision(6) << min * PI << endl;
    }
    return 0;
}
