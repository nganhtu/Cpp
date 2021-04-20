#include <bits/stdc++.h>
#define ull long long

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int tc;
    cin >> tc;
    for (int uwu = 1; uwu <= tc; ++uwu)
    {
        int n;
        cin >> n;
        ull res = 0, tmp;
        priority_queue<ull> a;
        priority_queue<ull, vector<ull>, greater<ull>> b;
        for (int i = 0; i < n; ++i)
        {
            cin >> tmp;
            a.push(tmp);
        }
        for (int i = 0; i < n; ++i)
        {
            cin >> tmp;
            b.push(tmp);
        }
        while (n--)
        {
            res += a.top() * b.top();
            a.pop();
            b.pop();
        }
        cout << "Case #" << uwu << ": " << res << endl;
    }

    return 0;
}
