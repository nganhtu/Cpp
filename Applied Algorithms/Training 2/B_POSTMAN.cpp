#include <bits/stdc++.h>
#define ull unsigned long long

using namespace std;

int n, k;
map<int, int> cust;
ull res;

void solve()
{
    ull res = 0;
    int trash = 0;
    map<int, int>::iterator it = cust.begin();
    for (map<int, int>::iterator i = cust.begin(); i != cust.end(); ++i)
    {
        if (i->second > 2 * k)
        {
            int j = i->second - 2 * k;
            j /= k;
            res += (ull)j * (ull)2 * (ull)abs(i->first);
            i->second -= j * k;
        }
    }

    while (it->first < 0)
    {
        if (trash == 0)
        {
            if (it->second >= k)
            {
                it->second -= k;
                res += (ull)(2 * abs(it->first));
            }
            else
            {
                trash = k - it->second;
                it->second = 0;
                res += (ull)(2 * abs(it->first));
                it++;
            }
        }
        else
        {
            if (it->second >= trash)
            {
                it->second -= trash;
                trash = 0;
            }
            else
            {
                trash -= it->second;
                it->second == 0;
                it++;
            }
        }
    }

    ull step = 0;
    map<int, int>::iterator tmp = it;
    while (tmp != cust.end())
    {
        step += (ull)tmp->second;
        tmp++;
    }
    step %= (ull)k;
    if (step == 0)
    {
        res -= (ull)(2 * it->first);
    }
    while (it != cust.end())
    {
        if (it->second < step)
        {
            step -= it->second;
            it->second = 0;
            it++;
        }
        else
        {
            it->second -= step;
            step = 0;
        }
        if (step == 0)
        {
            step = k;
            res += (ull)(2 * it->first);
        }
    }

    cout << res;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n >> k;
    for (int i = 0; i < n; ++i)
    {
        pair<int, int> tmp;
        cin >> tmp.first >> tmp.second;
        cust.insert(tmp);
    }

    solve();

    return 0;
}
