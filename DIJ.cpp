#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

/*
 graph: adjacency list where graph[u] is a vector of (neighbor, weight) pairs
 start: index of the starting node (0-based)
 returns: vector of shortest distances from start to each node
*/
vector<int> dijkstra(const vector<vector<pair<int,int>>>& graph, int start) {
    int n = graph.size();
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);
    dist[start] = 0;

    // min-heap of (distance, node)
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push(make_pair(0, start));

    while (!pq.empty()) {
        int current = pq.top().second;
        int current_dist = pq.top().first;
        pq.pop();

        if (visited[current]) continue;
        visited[current] = true;

        // relax edges
        for (const auto& edge : graph[current]) {
            int neighbor = edge.first;
            int weight = edge.second;
            if (!visited[neighbor] && current_dist != INF) {
                int new_dist = current_dist + weight;
                if (new_dist < dist[neighbor]) {
                    dist[neighbor] = new_dist;
                    pq.push(make_pair(new_dist, neighbor));
                }
            }
        }
    }

    return dist;
}

int main() {
    // Example graph (same structure as Python version):
    // Nodes: 0=A,1=B,2=C,3=D,4=E,5=F
    vector<vector<pair<int,int>>> graph(6);
    graph[0] = { {1,5}, {2,1} };               // A -> B (5), C (1)
    graph[1] = { {0,5}, {2,2}, {3,1} };        // B -> A, C, D
    graph[2] = { {0,1}, {1,2}, {3,4}, {4,8} }; // C -> A,B,D,E
    graph[3] = { {1,1}, {2,4}, {4,3}, {5,6} }; // D -> B,C,E,F
    graph[4] = { {2,8}, {3,3} };               // E -> C,D
    graph[5] = { {3,6} };                      // F -> D

    int start = 0; // starting from A

    vector<int> distances = dijkstra(graph, start);

    cout << "Shortest distances from node " << start << ":\n";
    for (int i = 0; i < distances.size(); ++i) {
        cout << "  " << i << ": ";
        if (distances[i] == INF) cout << "unreachable\n";
        else cout << distances[i] << "\n";
    }

    return 0;
}