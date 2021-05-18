#include <iostream>
#include <vector>
#include <list>
using namespace std;

void dfs(vector<list<int>> adj)
{
    stack<int> S;
    vector<bool> visited(adj.size());
    S.push(1);

    while (!S.empty())
    {
        int u = S.top();
        if (!visited[u])
        {
            visited[u] = true;
            cout << u << endl;
        }
        if (!adj[u].empty())
        {
            int v = adj[u].front();
            adj[u].pop_front();
            if (!visited[v])
            {
                S.push(v);
            }
        }
        else
        {
            S.pop();
        }
    }
}
