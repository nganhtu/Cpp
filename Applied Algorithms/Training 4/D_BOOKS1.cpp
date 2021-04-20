#include <bits/stdc++.h>
#define ull unsigned long long

using namespace std;

bool check(ull k, vector<ull> book, ull value)
{
    ull cnt = 1, i = 0, curr = 0;
    while (i < book.size())
    {
        if (curr + book[i] < value)
        {
            curr += book[i];
            ++i;
        }
        else
        {
            curr = 0;
            ++cnt;
        }
    }
    if (cnt <= k)
    {
        return true;
    }
    return false;
}

int countSlash(string s)
{
    int res = 0;
    for (char c : s)
    {
        if (c == '/')
        {
            ++res;
        }
    }
    return res;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int tc;
    cin >> tc;
    for (int i = 0; i < tc; ++i)
    {
        ull m, k;
        cin >> m >> k;
        vector<ull> book(m);
        for (int j = 0; j < m; ++j)
        {
            cin >> book[j];
        }

        ull min = 0, max = 5e9;
        while (min + 1 < max)
        {
            ull mid = (min + max) / 2;
            if (check(k, book, mid))
            {
                max = mid;
            }
            else
            {
                min = mid;
            }
        }
        string res = "";
        int curr = 0;
        for (int j = m - 1; j >= 0; --j)
        {
            if (curr + book[j] > min)
            {
                curr = book[j];
                if (j != m - 1)
                {
                    res = " / " + res;
                }
            }
            else
            {
                curr += book[j];
                if (j != m - 1)
                {
                    res = " " + res;
                }
            }
            res = to_string(book[j]) + res;
        }
        if (countSlash(res) < k - 1)
        {
            int misssingSlash = k - 1 - countSlash(res);
            int it = 1;
            for (int j = 0; j < misssingSlash; ++j)
            {
                while (!(res[it] == ' ' &&
                         res[it - 1] >= '0' &&
                         res[it - 1] <= '9' &&
                         res[it + 1] >= '0' &&
                         res[it + 1] <= '9'))
                {
                    ++it;
                }
                res.insert(it, " /");
            }
        }
        cout << res << endl;
    }

    return 0;
}
