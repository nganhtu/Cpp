#include <iostream>

#define ll long long

using namespace std;

ll solve(ll n)
{
    ll arr[3] = {0, 0, 1};
    for (int i = 0; i < n; ++i)
    {
        arr[0] += arr[1];
        arr[1] = arr[2];
        arr[2] = arr[0];
    }

    return arr[0] + arr[1] + arr[2];
}

int main()
{
    ll n;
    cout << "Nhap n: ";
    cin >> n;
    cout << "Sau " << n << " thang, so cap tho la " << solve(n) << endl;

    return 0;
}
