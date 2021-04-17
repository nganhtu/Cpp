#include <bits/stdc++.h>

using namespace std;

struct Session
{
    int port;
};

int main()
{
    Session a = {5};
    cout << a.port;

    void *b = (void *)&a;

    cout << ((Session *)b)->port;

    return 0;
}
