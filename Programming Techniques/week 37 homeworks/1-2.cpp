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
        return 3;
        break;
    case 3:
        return 6;
        break;
    case 4:
        return 10;
        break;
    default:
        break;
    }

    ll sum = 10, arr[4] = {1, 2, 3, 4};
    for (int i = 4; i < n; ++i)
    {
        arr[i % 4] = arr[(i - 1) % 4] - arr[(i - 2) % 4] + arr[(i - 3) % 4] + arr[(i - 4) % 4];
        sum += arr[i % 4];
    }

    return sum;
}

int main()
{
    ll n;
    cout << "Nhap n: ";
    cin >> n;
    cout << "Tong " << n << " phan tu dau tien la " << solve(n) << endl;

    return 0;
}
