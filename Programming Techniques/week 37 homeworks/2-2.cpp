#include <iostream>

#define ll long long

using namespace std;

pair<ll, ll> solve(ll n)
{
    switch (n)
    {
    case 1:
        return {1, 2};
        break;
    case 2:
        return {3, 6};
        break;
    case 3:
        return {6, 12};
        break;
    case 4:
        return {10, 20};
        break;
    default:
        break;
    }

    pair<ll, ll> sum = {10, 20};
    ll x[4] = {1, 2, 3, 4}, y[4] = {2, 4, 6, 8};
    for (int i = 4; i < n; ++i)
    {
        x[i % 4] = y[(i - 1) % 4] + x[(i - 2) % 4] + y[(i - 3) % 4] + x[(i - 4) % 4];
        sum.first += x[i % 4];
        y[i % 4] = x[(i - 1) % 4] + y[(i - 2) % 4] + x[(i - 3) % 4] + y[(i - 4) % 4];
        sum.second += y[i % 4];
    }

    return sum;
}

int main()
{
    ll n, sumX = 0, sumY = 0;
    cout << "Nhap n: ";
    cin >> n;
    pair<ll, ll> res = solve(n);
    cout << "Tong " << n << " phan tu dau tien cua day X la " << res.first << endl
         << "Tong " << n << " phan tu dau tien cua day Y la " << res.second << endl;

    return 0;
}
