#include <bits/stdc++.h>

using namespace std;

bool nextIsExist = true;

void printVector(vector<int> a)
{
    for (int i : a)
    {
        cout << i;
    }
    cout << endl;
}

void next(vector<int> &a)
{
    stack<int> tail;
    int n = a.size(), i = n - 1;
    for (i; i >= 1; --i)
    {
        if (a[i] > a[i - 1])
        {
            break;
        }
        else
        {
            tail.push(a[i]);
        }
    }
    if (i == 0 && a[1] <= a[0])
    {
        nextIsExist = false;
        return;
    }
    swap(a[i], a[i - 1]);
    for (int j = n - 1; j > i; --j)
    {
        a[j] = tail.top();
        tail.pop();
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int t;
    cin >> t;
    while (t--)
    {
        int n, h;
        cin >> n >> h;
        vector<int> a(n, 0);
        for (int i = n - 1; i >= n - h; --i)
        {
            a[i] = 1;
        }
        do
        {
            printVector(a);
            next(a);
        } while (nextIsExist);
    }

    return 0;
}
