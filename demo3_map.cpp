#include <winsock2.h>
#include <windows.h>
#include "cpp-httplib/httplib.h" // Include the httplib header
#include <iostream>
#include <vector>
#include <queue> 
#include <climits>
#include <string>

using namespace std;

// Define the structure to store edge information
struct Edge {
    int node;
    int weight;
};

// Define structure to store location information
struct Location {
    double lat;
    double lng;
    string title;
};

// List of locations (coordinates on the map)
vector<Location> locations = {
    {21.00698859515113, 105.84313676418928, "Toà C1"},
    {21.006430887292453, 105.84238676189455, "Toà C2"},
    {21.00664363939205, 105.843828639979, "Toà C3"},
    {21.006182680961018, 105.84409883484199, "Toà C4"},
    {21.00576871268214, 105.84404766835202, "Toà C5"},
    {21.005930685731766, 105.84506419268367, "Toà C6"},
    {21.005581689563275, 105.84510439052353, "Toà C7"},
    {21.005303993104107, 105.84506017289968, "Toà C8"},
    {21.00584062229857, 105.84195287982332, "Toà C9"},
    {21.00542165665716, 105.84414678286305, "Toà C10"},
    {21.004269379801435, 105.84207411426786, "Toà D4"}
};

// Dijkstra's Algorithm to find the shortest path
void dijkstra(int start, vector<vector<Edge>>& graph, vector<int>& dist, vector<int>& parent) {
    int n = graph.size();
    dist[start] = 0; // Distance from start to itself is 0

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, start}); // Push start node into the priority queue

    while (!pq.empty()) {
        int u = pq.top().second; // Get the node with the smallest distance
        int d = pq.top().first;  // The current distance
        pq.pop();

        if (d > dist[u]) continue;

        for (const Edge& edge : graph[u]) {
            int v = edge.node;
            int weight = edge.weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

// Handle HTTP request to find the shortest path
void handle_request(const httplib::Request& req, httplib::Response& res) {
    // Set CORS headers
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");

    // Extract start and end location from request parameters
    string start_str = req.get_param_value("start");
    string end_str = req.get_param_value("end");

    if (start_str.empty() || end_str.empty()) {
        res.set_content("{\"error\": \"Start and end parameters must be provided.\"}", "application/json");
        return;
    }

    int start = -1;
    int end = -1;

    // Find the indices of start and end locations
    for (int i = 0; i < locations.size(); ++i) {
        if (locations[i].title == start_str) {
            start = i;
        }
        if (locations[i].title == end_str) {
            end = i;
        }
    }

    // Check if the start or end location is invalid
    if (start == -1 || end == -1) {
        res.set_content("{\"error\": \"Invalid start or end point.\"}", "application/json");
        return;
    }

    // Create an adjacency list to represent the graph
    vector<vector<Edge>> graph(locations.size());
    // Example: Add some edges (distances in meters, assuming you can compute the distance between locations)
    graph[0].push_back({1, 10});
    graph[0].push_back({2, 15});
    graph[0].push_back({3, 5});
    graph[1].push_back({0, 10});
    graph[1].push_back({2, 5});
    graph[2].push_back({0, 15});
    graph[2].push_back({1, 5});
    graph[2].push_back({3, 10});
    graph[3].push_back({0, 5});
    graph[3].push_back({2, 10});

    // Initialize distances and parent arrays for Dijkstra's algorithm
    vector<int> dist(locations.size(), INT_MAX);
    vector<int> parent(locations.size(), -1);

    // Run Dijkstra's algorithm to find the shortest path
    dijkstra(start, graph, dist, parent);

    // Check if a path exists
    if (dist[end] == INT_MAX) {
        res.set_content("{\"error\": \"No path found between the given points.\"}", "application/json");
        return;
    }

    // Retrieve the shortest path
    vector<Location> path_locations;
    for (int v = end; v != -1; v = parent[v]) {
        path_locations.push_back(locations[v]);
    }

    // Reverse the path since we retrieved it from end to start
    reverse(path_locations.begin(), path_locations.end());

    // Convert the result into JSON format
    string result = "{\"path\": [";
    for (size_t i = 0; i < path_locations.size(); ++i) {
        result += "{\"lat\": " + std::to_string(path_locations[i].lat) +
                  ", \"lng\": " + std::to_string(path_locations[i].lng) +
                  ", \"title\": \"" + path_locations[i].title + "\"}";
        if (i < path_locations.size() - 1) result += ", ";
    }
    result += "], \"distance\": " + std::to_string(dist[end]) + "}";

    res.set_content(result, "application/json");
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return 1;
    }

    httplib::Server svr;
    svr.Get("/getShortestPath", handle_request);  // Register the API to handle requests

    std::cout << "Server is running at http://localhost:8080...\n";
    svr.listen("localhost", 8080);  // Start the server at port 8080

    // Cleanup Winsock after usage
    WSACleanup();

    return 0;
}
