#include <bits/stdc++.h>

using namespace std;

int gg = 1;

void solve(vector<int> &t, vector<pair<int, int>> noti)
{
    int curr = t[0];
    for (int i = 1; i < t.size(); ++i)
    {
        if (curr != t[i])
        {
            ++gg;
            curr = t[i];
        }
    }
    for (int i = 0; i < noti.size(); ++i)
    {
        int posi = noti[i].first - 1, code = noti[i].second;
        if (posi == 0)
        {
            // 1 -> 2
            if (t[0] == t[1] && code != t[1])
            {
                ++gg;
            }
            // 2 -> 1
            else if (t[0] != t[1] && code == t[1])
            {
                --gg;
            }
        }
        else if (posi == t.size() - 1)
        {
            // 1 -> 2
            if (t[t.size() - 1] == t[t.size() - 2] && code != t[t.size() - 2])
            {
                ++gg;
            }
            // 2 -> 1
            else if (t[t.size() - 1] != t[t.size() - 2] && code == t[t.size() - 2])
            {
                --gg;
            }
        }
        else
        {
            // 1 -> 3
            if (t[posi - 1] == t[posi] &&
                t[posi] == t[posi + 1] &&
                code != t[posi])
            {
                gg += 2;
            }
            // 3 -> 1
            else if (t[posi - 1] != t[posi] &&
                     t[posi - 1] == t[posi + 1] &&
                     t[posi - 1] == code)
            {
                gg -= 2;
            }
            // 2 -> 3
            else if (t[posi - 1] != t[posi + 1] &&
                     (t[posi] == t[posi - 1] || t[posi] == t[posi + 1]) &&
                     code != t[posi - 1] &&
                     code != t[posi + 1])
            {
                ++gg;
            }
            // 3 -> 2
            else if (t[posi - 1] != t[posi + 1] &&
                     t[posi] != t[posi - 1] &&
                     t[posi] != t[posi + 1] &&
                     (code == t[posi - 1] || code == t[posi + 1]))
            {
                --gg;
            }
        }

        t[posi] = code;
        cout << gg << endl;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n;
    cin >> n;
    vector<int> t(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> t[i];
    }
    int q;
    cin >> q;
    vector<pair<int, int>> noti(q);
    for (int i = 0; i < q; ++i)
    {
        cin >> noti[i].first >> noti[i].second;
    }

    solve(t, noti);

    return 0;
}
