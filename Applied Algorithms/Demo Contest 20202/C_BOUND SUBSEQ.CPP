#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n, m, M;
    cin >> n >> m >> M;
    vector<int> a(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> a[i];
    }
    int left = 0, right = 0, cnt = 0, sum = a[0];
    while (right < n)
    {
        // cout << "left = " << left << " right = " << right << endl;
        if (sum < m)
        {
            ++right;
            sum += a[right];
        }
        else if (sum > M)
        {
            if (left == right)
            {
                ++left;
                ++right;
                sum = a[left];
            }
            else
            {
                sum -= a[left];
                ++left;
            }
        }
        else
        {
            ++cnt;
            // cout << "bingo!" << endl;
            int leftTmp = left + 1, sumTmp = sum - a[left];
            while (leftTmp <= right && sumTmp >= m)
            {
                // cout << "leftTmp = " << leftTmp << " right = " << right << endl;
                ++cnt;
                // cout << "bingo!" << endl;
                sumTmp -= a[leftTmp];
                ++leftTmp;
            }
            ++right;
            sum += a[right];
        }
    }
    cout << cnt;

    return 0;
}
