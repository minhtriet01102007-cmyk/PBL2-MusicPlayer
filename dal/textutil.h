#pragma once
#include <algorithm> // std::max
#include <cctype>    // std::isspace, std::isdigit, std::tolower
#include <fstream>   // std::ifstream để đọc file
#include <sstream>   // std::stringstream để cắt chuỗi
#include <string>
#include <vector>

// chứa các hàm tiện ích xử lý chuỗi 
namespace textutil{
std::string trim(const std::string& s){ // Xóa khoảng trắng ở đầu và cuối chuỗi
    size_t b = 0, e = s.size(); // b là đầu chuỗi, e là cuối chuỗi, size_t: kiểu số nguyên không dấu
    while (b < e && std::isspace((unsigned char)s[b])) ++b; // Dịch b sang phải cho đến khi gặp ký tự không phải khoảng trắng
    while (e > b && std::isspace((unsigned char)s[e - 1])) --e; // Dịch e sang trái cho đến khi gặp ký tự không phải khoảng trắng
    return s.substr(b, e - b); // Lấy chuỗi con bắt đầu tại b, có độ dài e - b
}
std::string lower(std::string s){ // Chuyển toàn bộ các ký tự hiện tại trong chuỗi s thành chữ thường
    for (char& c : s){ // Duyệt qua từng ký tự 
        c = std::tolower((unsigned char)c); // Chuyển ký tự đó thành chữ thường
    }
    return s;
}
int toInt(const std::string& s, int def = 0){ // chuyển chuỗi sang số nguyên
    try{
        return std::stoi(s);
    }
    catch (...){
        return def;
    }
}
long long toLL(const std::string& s, long long def = 0){ // chuyển chuỗi sang số nguyên lớn
    try{
        return std::stoll(s);
    }
    catch (...){
        return def;
    }
}
char detectDelim(const std::string& line){ // Tự động nhận diện ký tự phân cách giữa các cột dữ liệu
    for (char c : {'|', ';', '\t', ','}) 
        if (line.find(c) != std::string::npos) return c; // tìm thấy, hàm sẽ trả về ký tự đó, std::string::npos: Giá trị biểu thị không tìm thấy
    return '|'; // nếu không tìm thấy, mặc định trả về '|'
}
std::vector<std::string> split(const std::string& line, char d){ // Tách một chuỗi thành nhiều chuỗi con dựa vào ký tự phân cách d, sau đó lưu vào vector<string>
    std::vector<std::string> parts; // Tạo vector để lưu các phần tử sau khi tách
    std::string current = ""; // Tạo chuỗi tạm để gom các ký tự trước khi gặp dấu phân cách
    for (char c : line){
        if (c == d){  // Nếu gặp ký tự phân cách 
            parts.push_back(trim(current)); // Đưa phần vừa gom được vào danh sách
            current = ""; // Reset lại chuỗi tạm để chuẩn bị gom phần tiếp theo
        } else{ 
            current += c; // Nếu chưa gặp ký tự phân cách thì nối ký tự vào chuỗi tạm
        }
    }
    parts.push_back(trim(current));
    return parts;
}
std::vector<std::string> readLines(const std::string& path){ // Đọc các dòng có dữ liệu 
    std::vector<std::string> lines; // Tạo vector để lưu các dòng đọc được
    std::ifstream in(path);
    std::string l;
    bool first = true;
    while (std::getline(in, l)){
        if (first && l.size() >= 3 && (unsigned char)l[0] == 0xEF && (unsigned char)l[1] == 0xBB && (unsigned char)l[2] == 0xBF) // Xóa UTF-8 BOM
            l.erase(0, 3);
        first = false;
        if (!l.empty() && l.back() == '\r') l.pop_back(); // Xử lý xuống dòng kiểu Windows: Xóa ký tự \r thừa ở cuối dòng nếu file được tạo trên Windows nhưng chạy trên Linux/macOS
        if (!trim(l).empty() && l[0] != '#') lines.push_back(l); // Bỏ qua dòng trống và dòng ghi chú
    }
    return lines;
}
// Nối các chuỗi lại thành một dòng, ngăn cách bằng sep
std::string join(const std::vector<std::string>& parts, const std::string& sep){
    std::string result = "";
    for (size_t i = 0; i < parts.size(); ++i){
        if (i > 0) result += sep;
        result += parts[i];
    }
    return result;
}
// Dấu phân cách giữa các cột khi GHI file (dấu '|' ghi thành " | " cho dễ đọc)
std::string columnSep(char d){
    return (d == '|') ? " | " : std::string(1, d);
}
// Dấu phân cách các phần tử trong MỘT cột (ví dụ danh sách id bài hát), phải khác dấu phân cách cột
char listDelim(char d){
    return (d == ';') ? ',' : ';';
}
// Nhận diện dấu phân cách của cả file từ dòng dữ liệu đầu tiên (file trống/không có -> '|')
char detectFileDelim(const std::string& path){
    auto lines = readLines(path);
    return lines.empty() ? '|' : detectDelim(lines[0]);
}
// Sinh id kế tiếp theo đúng dạng id đang có (S15 -> S16, PL001 -> PL002)
std::string nextId(const std::vector<std::string>& ids, const std::string& defPrefix){
    std::string prefix = defPrefix; // Phần tiền tố của ID
    size_t width = 3;               // Số lượng chữ số của phần số
    long long mx = 0;               // Số lớn nhất tìm được
    bool first = true;              // Đánh dấu ID hợp lệ đầu tiên
    for (const auto& id : ids){ // Duyệt từng id
        size_t i = id.size();
        while (i > 0 && std::isdigit((unsigned char)id[i - 1])) --i; // duyệt từ cuối chuỗi về đầu để xác định phần chữ số liên tiếp ở cuối ID
        std::string digits = id.substr(i); // Tách phần chữ và phần số
        if (digits.empty()) continue;
        if (first){
            prefix = id.substr(0, i);
            width = digits.size();
            first = false;
        }
        mx = std::max(mx, toLL(digits));
    }
    std::string num = std::to_string(mx + 1);
    if (num.size() < width) num = std::string(width - num.size(), '0') + num;
    return prefix + num;
}
}