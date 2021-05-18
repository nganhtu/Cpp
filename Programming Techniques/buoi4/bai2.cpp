// #include <iostream>
// #include <cmath>
// #include <iomanip>
// #include <utility>
// using namespace std;
// using Point = pair<double, double>;

double area(Point a, Point b, Point c)
{
    double ab = sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2)),
           bc = sqrt(pow(b.first - c.first, 2) + pow(b.second - c.second, 2)),
           ca = sqrt(pow(c.first - a.first, 2) + pow(c.second - a.second, 2)),
           p = (ab + bc + ca) / 2;
    // Heron
    return sqrt(p * (p - ab) * (p - bc) * (p - ca));
}

// int main() {
//     cout << setprecision(2) << fixed;
//     cout << area({1, 2}, {2.5, 10}, {15, -5.25}) << endl;
//     return 0;
// }
