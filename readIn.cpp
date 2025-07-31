#include <bits/stdc++.h>
#include <vector>
#include <algorithm>

class GraphPoint {
public:
    int x;
    int y;
    std::vector<GraphPoint> connections = std::vector<GraphPoint>();
    GraphPoint(int x, int y) : x(x), y(y) {}
    void addConection(GraphPoint& p) {
        this->connections.push_back(p);
    }
    void removeConection(GraphPoint& p) {
        this->connections.erase(std::remove_if(this->connections.begin(), this->connections.end(),
                                               [&p](const GraphPoint& gp) { return gp.x == p.x && gp.y == p.y; }),
                                this->connections.end());
    }
};

struct point {
    int x;
    int y;

    bool operator!=(const point& other) const {
        return (x != other.x) || (y != other.y);
    }
};
#include <iostream>
#include <algorithm>

bool onSegment(point p, point q, point r) {
    return (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y));
}

int orientation(point p, point q, point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) {
        return 0;  // collinear
    }
    return (val > 0) ? 1 : 2;  // clockwise or counterclockwise
}

bool doIntersect(point p1, point q1, point p2, point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4) {
        return true;
    }

    // Special cases
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

point intersection(point p1, point q1, point p2, point q2) {
    double A1 = q1.y - p1.y;
    double B1 = p1.x - q1.x;
    double C1 = A1 * p1.x + B1 * p1.y;

    double A2 = q2.y - p2.y;
    double B2 = p2.x - q2.x;
    double C2 = A2 * p2.x + B2 * p2.y;

    double determinant = A1 * B2 - A2 * B1;

    if (determinant == 0) {
        return {-1, -1};  // Lines are parallel
    }

    double x = (B2 * C1 - B1 * C2) / determinant;
    double y = (A1 * C2 - A2 * C1) / determinant;
    point inter = {(int)std::round(x), (int)std::round(y)};
    return inter;
}
std::string removeChar(const std::string& str, char ch) {
    std::string result = str; // Create a copy of the original string
    result.erase(std::remove(result.begin(), result.end(), ch), result.end());
    return result; // Return the modified string
}
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    for (char c : str) {
        if (c == delimiter) {
            result.push_back(token);
            token.clear(); // Clear the token for the next substring
        } else {
            token += c; // Append character to the current token
        }
    }
    result.push_back(token); // Add the last token
    return result;
}

int main() {

    // Open the text file named 
    // "textFile.txt"
    std::ifstream f("dotter/points.txt");

    // Check if the file is 
    // successfully opened
    if (!f.is_open()) {
        std::cerr << "Error opening the file!";
        return 1;
    }
    std::string s;
    int line_count = 0;
    std::vector<std::vector<point>> lines;
    while (getline(f, s)) {
        std::vector<std::string> temporaryVector;
        temporaryVector = split(removeChar(removeChar(removeChar(removeChar(s, '['), ']'),'('), ')'), ',');
        std::vector<point> line;
        for (int i = 0; i < temporaryVector.size(); i += 2) {
            int x = std::stoi(temporaryVector[i]);
            int y = std::stoi(temporaryVector[i + 1]);
            point p = {x, y};
            line.push_back(p);
        }
        lines.push_back(line);
    }
    int totalPoints = 0;
    for (int i = 0; i < lines.size(); i++) {
        totalPoints += lines[i].size();
    }

    // Close the file
    f.close();
    std::cout << "read text file\n plotting main points\n";
    std::vector<GraphPoint> graph;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            GraphPoint temp = GraphPoint(lines[i][j].x, lines[i][j].y);
            if (j > 0) {
                temp.addConection(graph.back());
                graph.back().addConection(temp);
            }
            graph.push_back(temp);
        }
    }

    /*
    intersectingPoints = []
    for i in range(len(lines)):
        for j in range(i, len(lines)):
            for k in range(len(lines[i]) - 1):
                for l in range(len(lines[j]) - 1):
                    if doIntersect(lines[i][k], lines[i][k + 1], lines[j][l], lines[j][l + 1]) and (lines[i][k] != lines[j][l] and lines[i][k + 1] != lines[j][l + 1]) and (lines[i][k+1] != lines[j][l] and lines[i][k] != lines[j][l + 1]):
                        point = intersection(lines[i][k], lines[i][k + 1], lines[j][l], lines[j][l + 1])
                        print(point)
                        if point != None:
                            intersectingPoints.append(point)
                            cv2.circle(image, (round(point[0]), round(point[1])), 3, (0, 255, 255), -1)
    */
    std::cout << "main points graphed\nfinding intersections";
    std::vector<point> intersectingPoints;
    std::vector<GraphPoint> tempIntersectionGraph;
    std::vector<std::vector<int>> intersectionConnections;
    for (int i = 0; i < lines.size(); i++) {
        for (int j = i; j < lines.size(); j++) {
            for (int k = 0; k < lines[i].size() -1; k++) {
                for (int l = 0; l < lines[j].size() -1; l++) {
                    if (doIntersect(lines[i][k], lines[i][k+1], lines[j][l], lines[j][l+1]) && ((lines[i][k] != lines[j][l]) && (lines[i][k + 1] != lines[j][l + 1])) && ((lines[i][k+1] != lines[j][l]) && (lines[i][k] != lines[j][l + 1]))) {
                        point p = intersection(lines[i][k], lines[i][k + 1], lines[j][l], lines[j][l + 1]);
                        if (p.x == -1 && p.y == -1) {
                            continue;
                        }
                        intersectingPoints.push_back(p);
                        /*GraphPoint tempNode = GraphPoint(p.x, p.y);
                        int requiredNodesIndeces[4] = {i*k+k, j*l+l, i*k+k+1, j*l+l+1};
                        for (int m = 0; m < 4; m++) {
                            graph[requiredNodesIndeces[m]].addConection(tempNode);
                            tempNode.addConection(graph[requiredNodesIndeces[m]]);
                            graph[requiredNodesIndeces[m]].removeConection(graph[requiredNodesIndeces[(m+2)%4]]);
                        }
                        tempIntersectionGraph.push_back(tempNode);*/
                        std::vector<int> tempConnection = {i*(lines[0].size()-1)+k, j*(lines[0].size()-1)+l, i*(lines[0].size()-1)+k+1, j*(lines[0].size()-1)+l+1};
                        intersectionConnections.push_back(tempConnection);
                    }
                }
            }
        }
    }
    std::cout << "found " << intersectingPoints.size() << " intersections\n";
    for (int n = 0; n < intersectingPoints.size(); n++) {
        int i = intersectionConnections[n][0] / (lines[0].size() - 1);
        int j = intersectionConnections[n][1] / (lines[0].size() - 1);
        int k = intersectionConnections[n][2] / (lines[0].size() - 1);
        int l = intersectionConnections[n][3] / (lines[0].size() - 1);
        point p = intersectingPoints[n];
        GraphPoint tempNode = GraphPoint(p.x, p.y);
        int requiredNodesIndeces[4] = {i, j, k, l};
        for (int m = 0; m < 4; m++) {
            graph[requiredNodesIndeces[m]].addConection(tempNode);
            tempNode.addConection(graph[requiredNodesIndeces[m]]);
            graph[requiredNodesIndeces[m]].removeConection(graph[requiredNodesIndeces[(m+2)%4]]);
        }
        tempIntersectionGraph.push_back(tempNode);
    }
    //graph.insert(graph.end(), tempIntersectionGraph.begin(), tempIntersectionGraph.end());
    std::cout << "intersections found\n";
    std::cout << "total points: " << graph.size() << intersectingPoints.size() << "\n";
    return 0;
}