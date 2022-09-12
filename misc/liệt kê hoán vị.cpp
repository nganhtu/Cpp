#include <bits/stdc++.h>

using namespace std;

void solve() {
}

int nextStat(vector<int> &perm, int n) {
    if (n < 2) {
        return -1;
    }
    int p = n - 2, q = n - 1;
    while (perm[p] > perm[p + 1] && p >= 0) {
        --p;
    }
    if (p == -1) {
        return -1;
    }
    while (perm[q] < perm[p]) {
        --q;
    }
    swap(perm[p], perm[q]);
    queue<int> tail;
    for (int i = n - 1; i > p; --i) {
        tail.push(perm[i]);
    }
    for (int i = p + 1; i < n; ++i) {
        perm[i] = tail.front();
        tail.pop();
    }
    return 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    vector<int> perm = {0, 1, 2, 3, 4, 5};

    for (int i = 0; i < 6; ++i) {
        cout << perm[i] << " ";
    }
    cout << endl;

    do {
        for (int i : perm) {
            cout << i << " ";
        }
        cout << endl;
    } while (nextStat(perm, 6) != -1);

    solve();

    return 0;
}
