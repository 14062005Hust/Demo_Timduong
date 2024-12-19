#include "cpp-httplib/httplib.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>

// Định nghĩa các tọa độ của các điểm
struct Location {
    std::string name;
    double lat;
    double lng;
};

std::vector<Location> locations = {
    {"Toà C1", 21.00698859515113, 105.84313676418928},
    {"Toà C2", 21.006430887292453, 105.84238676189455},
    {"Toà C3", 21.00664363939205, 105.843828639979},
    {"Toà C4", 21.006182680961018, 105.84409883484199}
};

// Hàm tìm tọa độ của một điểm theo tên
Location findLocation(const std::string& name) {
    for (const auto& loc : locations) {
        if (loc.name == name) {
            return loc;
        }
    }
    throw std::invalid_argument("Location not found");
}

int main() {
    httplib::Server svr;

    // Thêm CORS header để cho phép truy cập từ bất kỳ nguồn nào
    svr.set_base_dir("./");  // Đảm bảo thư mục cho tệp HTML

    // API để tính toán đường đi giữa 2 điểm
    svr.Get("/findRoute", [](const httplib::Request& req, httplib::Response& res) {
        // Thêm các header CORS để phép yêu cầu từ bất kỳ nguồn nào
        res.set_header("Access-Control-Allow-Origin", "*");  // Cho phép tất cả các nguồn
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        auto start = req.get_param_value("start");
        auto end = req.get_param_value("end");

        if (start.empty() || end.empty()) {
            res.set_content("{\"error\": \"Please provide both start and end locations.\"}", "application/json");
            return;
        }

        try {
            Location startLoc = findLocation(start);
            Location endLoc = findLocation(end);

            // Giả lập đường đi, trả về các điểm tọa độ trong dạng JSON
            std::ostringstream json_response;
            json_response << "{\"start\": \"" << startLoc.name
                          << "\", \"end\": \"" << endLoc.name
                          << "\", \"route\": [{\"lat\": " << startLoc.lat
                          << ", \"lng\": " << startLoc.lng << "}, "
                          << "{\"lat\": " << endLoc.lat
                          << ", \"lng\": " << endLoc.lng << "}]}\n";

            res.set_content(json_response.str(), "application/json");
        } catch (const std::invalid_argument&) {
            res.set_content("{\"error\": \"Location not found.\"}", "application/json");
        }
    });

    // Chạy máy chủ trên cổng 8080
    svr.listen("localhost", 8080);

    return 0;
}
