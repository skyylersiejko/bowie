#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>

class Node {
public:
    std::string name;
    std::vector<std::string> connections;
    int x;
    int y;
    int distance = INT32_MAX;

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

Node findNodeByName(const std::vector<Node>& nodes, const std::string& name) {
    for (const auto& node : nodes) {
        if (node.name == name) {
            return node;
        }
    }
    throw std::runtime_error("Node not found"+name);
}

int main() {
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
        std::string connectionsString = unParsedPoints[i][1].substr(unParsedPoints[i][1].find("[")+1, unParsedPoints[i][1].find("]") - unParsedPoints[i][1].find("[")-1);
        std::vector<std::string> connections;
        int start = 0;
        for (int j = 0; j < countChar(connectionsString, ",")+1; j++) {
            std::string currentName = connectionsString.substr(start+1, connectionsString.find(",")-2);
            start = connectionsString.find(",")+2;
            if (currentName == "" && std::find(connections.begin(), connections.end(), currentName) != connections.end()) break;
            connections.push_back(currentName);
        }
        ParsedPoints.push_back(Node(unParsedPoints[i][0], std::stoi(x), std::stoi(y), connections));
    }
    std::cout << "enter name of start point: ";
    std::string p1, p2;
    std::cin >> p1;
    std::cout << "enter name of end point: ";
    std::cin >> p2;

    // Implement Dijkstra's algorithm here to find the shortest path from p1 to p2
    std::cout << "Starting Dijkstra's algorithm from " << p1 << " to " << p2 << std::endl;
    Node* startNode = nullptr;
    for (auto& node : ParsedPoints) {
        if (node.name == p1) {
            startNode = &node;
            break;
        }
    }
    if (!startNode) {
        std::cerr << "Start node not found!" << std::endl;
        return 1;
    }
    std::vector<std::string> visited;
    std::vector<std::string> toVisit = names;
    int currentDistance = 0;
    bool solving = true;
    int iterations = 0;
    while (solving) {
        std::cout << "Iteration " << iterations++ << ": at node " << startNode->name << " with current distance " << currentDistance << std::endl;
        if (iterations > 1000) {
            std::cout << "Too many iterations, stopping." << std::endl;
            break;
        }
        if (startNode->name == p2) {
            solving = false;
            std::cout << "Path found!\n Distance=" << currentDistance << std::endl;
        } else {
            std::vector<int> distances;
            for (int i = 0; i < startNode->connections.size(); i++) {
                if (std::find(visited.begin(), visited.end(), startNode->connections[i]) != visited.end()) {
                    distances.push_back(INT32_MAX);
                    continue;
                } else {
                    Node n = findNodeByName(ParsedPoints, startNode->connections[i]);
                    n.distance = currentDistance + static_cast<int>(sqrt(pow(n.x - startNode->x, 2) + pow(n.y - startNode->y, 2)));
                    distances.push_back(n.distance);
                }
            }
            int minIndex = std::min_element(distances.begin(), distances.end()) - distances.begin();
            
            std::string nextNodeName = startNode->connections[minIndex];
            visited.push_back(startNode->name);
            toVisit.erase(std::remove(toVisit.begin(), toVisit.end(), startNode->name), toVisit.end());
            startNode = nullptr;
            for (auto& node : ParsedPoints) {
                if (node.name == nextNodeName) {
                    startNode = &node;
                    break;
                }
            }
            currentDistance = distances[minIndex];
        }
    }

    return 0;
}
