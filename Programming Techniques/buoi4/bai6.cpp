void bfs(vector<list<int>> adj)
{
    queue<int> Q;
    vector<bool> visited(adj.size());
    Q.push(1); // Bắt đầu từ đỉnh số 1

    while (!Q.empty())
    {
        int u = Q.front();
        if (!adj[u].empty())
        {
            int v = adj[u].front();
            adj[u].pop_front();
            if (!visited[v])
            {
                Q.push(v);
                visited[v] = true;
            }
        }
        else
        {
            cout << u << endl;
            Q.pop();
        }
    }
}
