#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
void print_vector(const vector<int> &a)
{
    for (int v : a)
        cout << v << ' ';
    cout << endl;
}

void delete_even(vector<int> &a)
{
    for (int i = a.size() - 1; i >= 0; --i)
    {
        if (a[i] % 2 == 0)
        {
            a.erase(a.begin() + i);
        }
    }
}

void sort_decrease(vector<int> &a)
{
    sort(a.begin(), a.end(), greater<int>());
}

vector<int> merge_vectors(const vector<int> &a, const vector<int> &b)
{
    vector<int> res(0);
    unsigned int itA = 0, itB = 0;
    while (itA < a.size() || itB < b.size())
    {
        if (itA == a.size())
        {
            res.push_back(b[itB++]);
        }
        else if (itB == b.size())
        {
            res.push_back(a[itA++]);
        }
        else if (a[itA] > b[itB])
        {
            res.push_back(a[itA++]);
        }
        else
        {
            res.push_back(b[itB++]);
        }
    }

    return res;
}

int main()
{
    int m, n, u;
    std::vector<int> a, b;

    std::cin >> m >> n;
    for (int i = 0; i < m; i++)
    {
        std::cin >> u;
        a.push_back(u);
    }
    for (int i = 0; i < n; i++)
    {
        std::cin >> u;
        b.push_back(u);
    }

    delete_even(a);
    cout << "Odd elements of a: ";
    print_vector(a);

    delete_even(b);
    cout << "Odd elements of b: ";
    print_vector(b);

    sort_decrease(a);
    cout << "Decreasingly sorted a: ";
    print_vector(a);

    sort_decrease(b);
    cout << "Decreasingly sorted b: ";
    print_vector(b);

    vector<int> c = merge_vectors(a, b);
    cout << "Decreasingly sorted c: ";
    print_vector(c);

    return 0;
}
