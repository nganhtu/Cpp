#include <iostream>

#define ll long long

using namespace std;

ll solve(ll n)
{
    switch (n)
    {
    case 1:
        return 1;
        break;
    case 2:
        return 2;
        break;
    case 3:
        return 3;
        break;
    case 4:
        return 4;
        break;
    default:
        break;
    }

    return solve(n - 1) - solve(n - 2) + solve(n - 3) + solve(n - 4);
}

int main()
{
    ll n;
    cout << "Nhap n: ";
    cin >> n;
    cout << "Phan tu thu " << n << " la " << solve(n) << endl;
    ll sum = 0;
    for (int i = 1; i <= n; ++i)
    {
        sum += solve(i);
    }
    cout << "Tong " << n << " phan tu dau tien la " << sum << endl;

    return 0;
}
