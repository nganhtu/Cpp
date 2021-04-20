#include <bits/stdc++.h>
#define ull unsigned long long

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    ull n, m;
    cin >> n >> m;
    priority_queue<ull> tree;
    for (int i = 0; i < n; ++i)
    {
        ull tmp;
        cin >> tmp;
        tree.push(tmp);
    }

    ull res = 0, treeCnt = 0, currHigh = -1, currLow = -1;
    while (!tree.empty() && res < m)
    {
        currLow = tree.top();
        tree.pop();
        if (currHigh == -1)
        {
            currHigh = currLow;
        }
        else
        {
            if (m - res < (currHigh - currLow) * treeCnt)
            {
                ull tmp = (m - res) / treeCnt;
                if (tmp * treeCnt < m - res)
                {
                    ++tmp;
                }
                currHigh -= tmp;
                break;
            }
            else
            {
                res += (currHigh - currLow) * treeCnt;
                currHigh = currLow;
            }
        }
        ++treeCnt;
    }

    cout << currHigh;

    return 0;
}
