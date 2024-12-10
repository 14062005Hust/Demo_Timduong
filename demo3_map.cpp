// #include <iostream>
// #include <vector>
// #include <queue>
// #include <climits>
// #include "cpp-httplib/httplib.h" // Thêm thư viện httplib

// using namespace std;

// // Định nghĩa cấu trúc để lưu thông tin của các đỉnh
// struct Edge {
//     int node;
//     int weight;
// };

// // Thuật toán Dijkstra để tìm đường đi ngắn nhất
// void dijkstra(int start, vector<vector<Edge>>& graph, vector<int>& dist, vector<int>& parent) {
//     int n = graph.size();
//     dist[start] = 0; // Khoảng cách đến điểm xuất phát là 0

//     priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
//     pq.push({0, start}); // Đưa điểm xuất phát vào hàng đợi

//     while (!pq.empty()) {
//         int u = pq.top().second; // Lấy đỉnh u với khoảng cách nhỏ nhất
//         int d = pq.top().first;  // Khoảng cách của u
//         pq.pop();

//         if (d > dist[u]) continue;

//         for (const Edge& edge : graph[u]) {
//             int v = edge.node;
//             int weight = edge.weight;

//             if (dist[u] + weight < dist[v]) {
//                 dist[v] = dist[u] + weight;
//                 parent[v] = u;
//                 pq.push({dist[v], v});
//             }
//         }
//     }
// }

// // API để trả về đường đi ngắn nhất
// void handle_request(const httplib::Request& req, httplib::Response& res) {
//     int start = std::stoi(req.get_param_value("start"));
//     int end = std::stoi(req.get_param_value("end"));

//     // Đồ thị (dưới dạng danh sách kề)
//     vector<vector<Edge>> graph(6); // Thêm một đỉnh mới, nên thay size là 6

//     // Các cạnh ban đầu
//     graph[0].push_back({1, 10});
//     graph[0].push_back({2, 5});
//     graph[1].push_back({2, 2});
//     graph[1].push_back({3, 1});
//     graph[2].push_back({1, 3});
//     graph[2].push_back({3, 9});

//     // Thêm đỉnh mới (ví dụ: đỉnh 4 nối với đỉnh 2 và 3)
//     graph[4].push_back({2, 3});
//     graph[4].push_back({3, 4});

//     vector<int> dist(6, INT_MAX); // Cập nhật số lượng đỉnh
//     vector<int> parent(6, -1);   // Cập nhật số lượng đỉnh
    
//     dijkstra(start, graph, dist, parent);

//     // Tạo đường đi ngắn nhất
//     vector<int> path;
//     if (dist[end] != INT_MAX) {
//         for (int v = end; v != -1; v = parent[v]) {
//             path.push_back(v);
//         }
//     }

//     // Chuyển đường đi ngắn nhất thành JSON
//     string result = "{\"path\": [";
//     for (size_t i = 0; i < path.size(); ++i) {
//         result += std::to_string(path[path.size() - 1 - i]);
//         if (i < path.size() - 1) result += ", ";
//     }
//     result += "], \"distance\": " + std::to_string(dist[end]) + "}";

//     res.set_content(result, "application/json");
// }

// int main() {
//     httplib::Server svr;
//     svr.Get("/getShortestPath", handle_request);  // Đăng ký API để xử lý yêu cầu

//     std::cout << "Server is running at http://localhost:8080...\n";
//     svr.listen("localhost", 8080);  // Khởi động server tại cổng 8080

//     return 0;
// }

#include <winsock2.h>
#include <windows.h>
#include "cpp-httplib/httplib.h" // Thêm thư viện httplib
#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Định nghĩa cấu trúc để lưu thông tin của các đỉnh
struct Edge {
    int node;
    int weight;
};

// Thuật toán Dijkstra để tìm đường đi ngắn nhất
void dijkstra(int start, vector<vector<Edge>>& graph, vector<int>& dist, vector<int>& parent) {
    int n = graph.size();
    dist[start] = 0; // Khoảng cách đến điểm xuất phát là 0

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, start}); // Đưa điểm xuất phát vào hàng đợi

    while (!pq.empty()) {
        int u = pq.top().second; // Lấy đỉnh u với khoảng cách nhỏ nhất
        int d = pq.top().first;  // Khoảng cách của u
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

// API để trả về đường đi ngắn nhất
void handle_request(const httplib::Request& req, httplib::Response& res) {
    int start = std::stoi(req.get_param_value("start"));
    int end = std::stoi(req.get_param_value("end"));

    // Đồ thị (dưới dạng danh sách kề)
    vector<vector<Edge>> graph(5);
    graph[0].push_back({1, 10});
    graph[0].push_back({2, 5});
    graph[1].push_back({2, 2});
    graph[1].push_back({3, 1});
    graph[2].push_back({1, 3});
    graph[2].push_back({3, 9});

    vector<int> dist(5, INT_MAX);
    vector<int> parent(5, -1);

    dijkstra(start, graph, dist, parent);

    // Tạo đường đi ngắn nhất
    vector<int> path;
    if (dist[end] != INT_MAX) {
        for (int v = end; v != -1; v = parent[v]) {
            path.push_back(v);
        }
    }

    // Chuyển đường đi ngắn nhất thành JSON
    string result = "{\"path\": [";
    for (size_t i = 0; i < path.size(); ++i) {
        result += std::to_string(path[path.size() - 1 - i]);
        if (i < path.size() - 1) result += ", ";
    }
    result += "], \"distance\": " + std::to_string(dist[end]) + "}";

    res.set_content(result, "application/json");
}

int main() {
    // Khởi tạo Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return 1;
    }

    httplib::Server svr;
    svr.Get("/getShortestPath", handle_request);  // Đăng ký API để xử lý yêu cầu

    std::cout << "Server is running at http://localhost:8080...\n";
    svr.listen("localhost", 8080);  // Khởi động server tại cổng 8080

    // Dọn dẹp thư viện Winsock sau khi sử dụng
    WSACleanup();

    return 0;
}
