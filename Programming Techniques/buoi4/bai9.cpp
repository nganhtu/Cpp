#include <map>
vector<int> dijkstra(const vector<vector<pair<int, int>>> &adj)
{
	int size = adj.size();
	vector<int> cost(size);
	for (int i = 0; i < size; i++)
	{
		cost[i] = 1000000000;
	}
	//
	vector<bool> passed(size);
	map<int, int> Q;
	cost[0] = 0;
	Q.insert({0, cost.at(0)});

	while (!Q.empty())
	{
		int pos;
		int min_cost = 1000000000;
		for (const auto &e : Q)
		{
			if (!passed[e.first])
				if (e.second < min_cost)
				{
					pos = e.first;
					min_cost = e.second;
				}
		}
		for (int i = 0; (unsigned)i < adj[pos].size(); i++)
		{
			int point = adj[pos][i].first;
			int p_p = adj[pos][i].second;
			if (!passed[point])
			{
				cost.at(point) = min(cost.at(pos) + p_p, cost.at(point));
				Q[point] = cost.at(point);
			}
		}
		passed[pos] = true;
		Q.erase(pos);
	}
	return cost;
}
