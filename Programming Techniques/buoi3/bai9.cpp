#include <bits/stdc++.h>

using namespace std;

int n, k1, k2, streakCounter = 0;
int day[200] = {0};

void solve(int d)
{
    if (d == n)
    {
        if (streakCounter >= k1 && streakCounter <= k2 || streakCounter == 0)
        {
            for (int i = 0; i < n; ++i)
            {
                cout << day[i];
            }
            cout << endl;
        }
        streakCounter = 0;
    }
    else if (d == 0)
    {
        day[0] = 0;
        solve(1);
        day[0] = 1;
        ++streakCounter;
        solve(1);
        --streakCounter;
    }
    else if (day[d - 1] == 0 || streakCounter < k1)
    {
        day[d] = 1;
        ++streakCounter;
        solve(d + 1);
        --streakCounter;
    }
    else if (streakCounter == k2)
    {
        day[d] = 0;
        int tmp = streakCounter;
        streakCounter = 0;
        solve(d + 1);
        streakCounter = tmp;
    }
    else
    {
        day[d] = 0;
        int tmp = streakCounter;
        streakCounter = 0;
        solve(d + 1);
        streakCounter = tmp;
        day[d] = 1;
        ++streakCounter;
        solve(d + 1);
        --streakCounter;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n >> k1 >> k2;

    solve(0);

    return 0;
}
