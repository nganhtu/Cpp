#include <bits/stdc++.h>

using namespace std;

bool cmp(pair<int, int> a, pair<int, int> b)
{
    if (a.second == b.second)
    {
        return a.first > b.first;
    }
    return a.second > b.second;
}

void solve()
{
    vector<pair<int, int>> arr;
    for (int a, b; cin >> a >> b;)
    {
        arr.push_back({a, b});
    }
    sort(arr.begin(), arr.end(), cmp);

    for (pair<int, int> p : arr)
    {
        cout << p.first << " " << p.second << endl;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    solve();

    return 0;
}
