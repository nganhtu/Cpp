#include <bits/stdc++.h>

using namespace std;

struct Edge
{
    int v;
    int w;
}

int
find_shortest_path(int start, int des, vector<vector<Edge>> a)
{
    vector<long long> d(n + 1, 0);
    for (int i = 0; i <= n; ++i)
    {
        d[i] = MAX;
    }
    d[start] = 0;
    priority_queue<pair<long long, int>> vertexQueue;
    vertexQueue.push({-0, start});
    while (!vertexQueue.empty())
    {
        pair<long long, int> p = vertexQueue.top();
        long long distance = -p.first;
        int minVertex = p.second;
        if (d[minVertex] < distance)
        {
            continue;
        }
        for (Edge e : a[minVertex])
        {
            int v = e.v, w = e.w;
            if (d[v] > d[minVertex] + w)
            {
                d[v] = d[minVertex] + w;
                vertexQueue.push({-d[v], v});
            }
        }
    }

    return d[des];
}
