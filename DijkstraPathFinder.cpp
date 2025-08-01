#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <bits/stdc++.h>
#include <queue>
#include <unordered_map>
#include <limits>

const int INF = std::numeric_limits<int>::max();

class Node {
public:
    std::string name;
    std::vector<std::string> connections;
    int x;
    int y;
    Node(std::string n, int x, int y, std::vector<std::string> c) : x(x), y(y), name(n), connections(c) {}
};

int countChar(std::string str, std::string substr) {
    int total = 0; 
    size_t pos = str.find(substr); 

    while (pos != std::string::npos) {
        total++;
        pos = str.find(substr, pos + substr.length());
    }

    return total;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    for (char c : str) {
        if (c == delimiter) {
            if (!token.empty()) tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

std::vector<std::string> dijkstraPath(std::vector<Node>& ParsedPoints, std::string startName, std::string endName) {
    std::vector<std::vector<std::pair<int, int>>> graph(ParsedPoints.size());
    std::unordered_map<std::string, int> nameToIndex;
    for (int i = 0; i < ParsedPoints.size(); i++) {
        nameToIndex[ParsedPoints[i].name] = i;
    }
    for (int i = 0; i < ParsedPoints.size(); i++) {
        for (const auto& connName : ParsedPoints[i].connections) {
            if (nameToIndex.find(connName) != nameToIndex.end()) {
                int j = nameToIndex[connName];
                int weight = static_cast<int>(sqrt(pow(ParsedPoints[i].x - ParsedPoints[j].x, 2) + pow(ParsedPoints[i].y - ParsedPoints[j].y, 2)));
                graph[i].push_back({j, weight});
            }
        }
    }
    int start = nameToIndex[startName];
    int end = nameToIndex[endName];
    int n = ParsedPoints.size();
    std::vector<int> dist(n, INF);
    std::vector<bool> visited(n, false);
    std::vector<std::string> path;
    std::vector<std::string> currentPath;
    dist[start] = 0;

    // min-heap of (distance, node)
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> pq;
    pq.push(std::make_pair(0, start));

    std::vector<int> prev(n, -1);


    while (!pq.empty()) {
        int current = pq.top().second;
        int current_dist = pq.top().first;
        pq.pop();

        if (visited[current]) continue;
        visited[current] = true;

        for (const auto& edge : graph[current]) {
            int neighbor = edge.first;
            int weight = edge.second;
            if (!visited[neighbor] && current_dist != INF) {
                int new_dist = current_dist + weight;
                if (new_dist < dist[neighbor]) {
                    dist[neighbor] = new_dist;
                    prev[neighbor] = current; // Track previous node
                    pq.push(std::make_pair(new_dist, neighbor));
                }
            }
        }
    }

    // Reconstruct path from end to start
    if (dist[end] == INF) {
        // No path found
        return {};
    }
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(ParsedPoints[at].name);
        if (at == start) break; // Stop when start is reached
    }
    std::reverse(path.begin(), path.end());
    return path;
}

Node findNodeByName(const std::vector<Node>& nodes, const std::string& name) {
    for (const auto& node : nodes) {
        if (node.name == name) {
            return node;
        }
    }
    throw std::runtime_error("Node not found"+name);
}

std::vector<int> dijkstraFind(std::vector<Node>& ParsedPoints, std::string startName, std::string endName) {
    std::vector<std::vector<std::pair<int, int>>> graph(ParsedPoints.size());
    std::unordered_map<std::string, int> nameToIndex;
    for (int i = 0; i < ParsedPoints.size(); i++) {
        nameToIndex[ParsedPoints[i].name] = i;
    }
    for (int i = 0; i < ParsedPoints.size(); i++) {
        for (const auto& connName : ParsedPoints[i].connections) {
            if (nameToIndex.find(connName) != nameToIndex.end()) {
                int j = nameToIndex[connName];
                int weight = static_cast<int>(sqrt(pow(ParsedPoints[i].x - ParsedPoints[j].x, 2) + pow(ParsedPoints[i].y - ParsedPoints[j].y, 2)));
                graph[i].push_back({j, weight});
            }
        }
    }
    int start = nameToIndex[startName];
    int end = nameToIndex[endName];
    int n = ParsedPoints.size();
    std::vector<int> dist(n, INF);
    std::vector<bool> visited(n, false);
    dist[start] = 0;

    // min-heap of (distance, node)
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> pq;
    pq.push(std::make_pair(0, start));

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
                    pq.push(std::make_pair(new_dist, neighbor));
                }
            } else if (neighbor == end) {
                pq.push(std::make_pair(current_dist + weight, neighbor));
                break;
            }
        }
    }

    return dist;
}

int main(int argc, char* argv[]) {
    std::string p1 = "5"; // Default start point
    std::string p2 = "131"; // Default end point
    if (argc >= 3) {
        p1 = argv[1];
        p2 = argv[2];
    }
    std::ifstream f("dotter/graph.json");

    // Check if the file is 
    // successfully opened
    if (!f.is_open()) {
        std::cerr << "Error opening the file!";
        return 1;
    }
    std::vector<std::array<std::string, 2>> unParsedPoints;
    std::vector<Node> ParsedPoints;
    std::vector<std::string> names;
    std::string s;
    bool isFirst = true;
    while (getline(f, s)) {
        if (isFirst) {
            isFirst = false;
            continue;
        }
        std::string name = "";
        for (int i = 1; s[i] != '"'; i++) {
            name += s[i];
        }
        std::string data = "";
        for (int i = 3+name.size(); s[i] != '}'; i++) {
            data += s[i];
        }
        //std::string[2] temp = {name, data};
        unParsedPoints.push_back(std::array<std::string, 2>{name, data});
        names.push_back(name);
    }
    f.close();
    for (int i = 0; i < unParsedPoints.size(); i++) {
        std::string x = unParsedPoints[i][1].substr(unParsedPoints[i][1].find("\"x\":")+4, unParsedPoints[i][1].find(",") - unParsedPoints[i][1].find("\"x\":")-4);
        std::string y = unParsedPoints[i][1].substr(unParsedPoints[i][1].find("\"y\":")+4, unParsedPoints[i][1].find(",", 10) - unParsedPoints[i][1].find("\"y\":")-4);
        std::string connectionsString = unParsedPoints[i][1].substr(
            unParsedPoints[i][1].find("[")+1,
            unParsedPoints[i][1].find("]") - unParsedPoints[i][1].find("[")-1
        );
        std::vector<std::string> connections;
        for (auto conn : split(connectionsString, ',')) {
            // Remove quotes and whitespace
            conn.erase(std::remove(conn.begin(), conn.end(), '"'), conn.end());
            conn.erase(std::remove_if(conn.begin(), conn.end(), ::isspace), conn.end());
            if (!conn.empty()) connections.push_back(conn);
        }
        ParsedPoints.push_back(Node(unParsedPoints[i][0], std::stoi(x), std::stoi(y), connections));
    }
    /*std::cout << "enter name of start point: ";
    std::string p1, p2;
    std::cin >> p1;
    std::cout << "enter name of end point: ";
    std::cin >> p2;*/
    // Implement Dijkstra's algorithm here to find the shortest path from p1 to p2
    std::cout << "Starting Dijkstra's algorithm from " << p1 << " to " << p2 << std::endl;
    //std::cout << findNodeByName(ParsedPoints, "5");
    std::vector<int> shortestDistance = dijkstraFind(ParsedPoints, p1, p2);
    std::cout << "Shortest distance from " << p1 << " to " << p2 << " is: ";
    if (shortestDistance[std::distance(ParsedPoints.begin(), std::find_if(ParsedPoints.begin(), ParsedPoints.end(), [&](const Node& n){ return n.name == p2; }))] == INF) {
        std::cout << "No path found." << std::endl;
    } else {
        std::cout << shortestDistance[std::distance(ParsedPoints.begin(), std::find_if(ParsedPoints.begin(), ParsedPoints.end(), [&](const Node& n){ return n.name == p2; }))] << std::endl;
    }
    std::vector<std::string> path;
    path = dijkstraPath(ParsedPoints, p1, p2);

    std::cout << "Path found, writing to file";

    std::ofstream outFile("dotter/bestPath.json");
    outFile << "{\n";
    for (int i = 0; i < path.size(); i++) {
    Node currentNode = findNodeByName(ParsedPoints, path[i]);
    outFile << "\"" << currentNode.name << "\": {\"x\":" << currentNode.x << ", \"y\":" << currentNode.y << ", \"connections\": [";
    if (i < path.size() - 1) {
        outFile << "\"" << path[i+1] << "\"";
    }
    outFile << "]}";
    if (i == path.size() - 1) {
        outFile << "\n}";
    } else {
        outFile << ",\n";
    }
}
    outFile.close();
    std::cout << "Path written to dotter/bestPath.json\nexiting\n";

    return 0;
}
