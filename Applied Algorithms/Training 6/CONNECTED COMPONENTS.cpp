#include <bits/stdc++.h>

#define maxn 10005

using namespace std;

int n, m;
vector<int> edge[maxn];
int visit[maxn];

void dfs(int u)
{
    visit[u] = 1;
    for (int i = 0; i < edge[u].size(); ++i)
    {
        int v = edge[u][i];
        if (visit[v])
        {
            continue;
        }
        dfs(v);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n >> m;
    for (int i = 1; i <= m; ++i)
    {
        int u, v;
        cin >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }

    int count = 0;
    for (int i = 1; i <= n; ++i)
    {
        if (visit[i] == 0)
        {
            count++;
            dfs(i);
        }
    }
    cout << count;

    return 0;
}
