#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int a = 1234;
    int *p = &a;
    printf("%d %d %d\n", p, *p, &p);

    return 0;
}
