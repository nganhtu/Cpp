#include <bits/stdc++.h>

using namespace std;

int solve(string s, int pos, int bal, int remO, int remC)
{
    if (bal < 0)
    {
        return 0;
    }
    if (pos == s.length())
    {
        if (bal == 0)
        {
            return 1;
        }
        return 0;
    }
    if (remO < 0 || remC < 0)
    {
        return 0;
    }
    if (s[pos] == '(')
    {
        return solve(s, pos + 1, bal + 1, remO, remC);
    }
    if (s[pos] == ')')
    {
        return solve(s, pos + 1, bal - 1, remO, remC);
    }
    return solve(s, pos + 1, bal + 1, remO - 1, remC) + solve(s, pos + 1, bal - 1, remO, remC - 1);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    string s;
    cin >> s;
    int sum = 0, sub = 0;
    for (char c : s)
    {
        if (c == '(')
        {
            --sub;
        }
        else if (c == ')')
        {
            ++sub;
        }
        else
        {
            ++sum;
        }
    }
    int remO = (sum + sub) / 2, remC = (sum - sub) / 2;
    cout << solve(s, 0, 0, remO, remC);

    return 0;
}
