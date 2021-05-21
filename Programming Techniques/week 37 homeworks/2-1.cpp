#include <iostream>

#define ll long long

using namespace std;

ll solveX(ll);

ll solveY(ll);

int main()
{
    ll n, sumX = 0, sumY = 0;
    cout << "Nhap n: ";
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        sumX += solveX(i);
        sumY += solveY(i);
    }
    cout << "Tong " << n << " phan tu dau tien cua day X la " << sumX << endl
         << "Tong " << n << " phan tu dau tien cua day Y la " << sumY << endl;

    return 0;
}

ll solveX(ll n)
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

    return solveY(n - 1) + solveX(n - 2) + solveY(n - 3) + solveX(n - 4);
}

ll solveY(ll n)
{
    switch (n)
    {
    case 1:
        return 2;
        break;
    case 2:
        return 4;
        break;
    case 3:
        return 6;
        break;
    case 4:
        return 8;
        break;
    default:
        break;
    }

    return solveX(n - 1) + solveY(n - 2) + solveX(n - 3) + solveY(n - 4);
}
