#pragma once
#include <algorithm> // std::max
#include <cctype>    // std::isspace, std::isdigit, std::tolower
#include <fstream>   // std::ifstream để đọc file
#include <sstream>   // std::stringstream để cắt chuỗi
#include <string>
#include <vector>

namespace textutil{
std::string trim(const std::string& s){ // cắt bỏ các khoảng trắng thừa ở đầu b và cuối e chuỗi
    size_t b = 0, e = s.size(); // b là đầu chuỗi, e là cuối chuỗi
    while (b < e && std::isspace((unsigned char)s[b])) ++b; // dịch b sang phải khi gặp khoảng trắng
    while (e > b && std::isspace((unsigned char)s[e - 1])) --e; // dịch e sang trái khi gặp khoảng trống
    return s.substr(b, e - b); // Trả về đoạn chuỗi con từ b đến e
}
std::string lower(std::string s){
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
char detectDelim(const std::string& line){ // đoán ký tự phân cách của một dòng dữ liệu
    for (char c : {'|', ';', '\t', ','}) 
        if (line.find(c) != std::string::npos) return c; // tìm thấy, hàm sẽ trả về ký tự đó
    return '|'; // nếu không tìm thấy, mặc định trả về '|'
}
std::vector<std::string> split(const std::string& line, char d){
    std::vector<std::string> parts;
    std::string current = ""; // Biến tạm để gom các ký tự lại thành 1 từ/cột
    for (char c : line){
        if (c == d){  // Nếu gặp ký tự phân cách 
            parts.push_back(trim(current)); // Đưa phần vừa gom được vào danh sách
            current = ""; // Reset lại chuỗi tạm để chuẩn bị gom phần tiếp theo
        } else{ // Nếu chưa gặp ký tự phân cách, tiếp tục gom ký tự vào chuỗi tạm
            current += c; 
        }
    }
    parts.push_back(trim(current));
    return parts;
}
std::vector<std::string> readLines(const std::string& path){ // Đọc các dòng có dữ liệu 
    std::vector<std::string> lines;
    std::ifstream in(path);
    std::string l;
    bool first = true;
    while (std::getline(in, l)){
        if (first && l.size() >= 3 && (unsigned char)l[0] == 0xEF && (unsigned char)l[1] == 0xBB && (unsigned char)l[2] == 0xBF)
            l.erase(0, 3);
        first = false;
        if (!l.empty() && l.back() == '\r') l.pop_back();
        if (!trim(l).empty() && l[0] != '#') lines.push_back(l);
    }
    return lines;
}
// Sinh id kế tiếp theo đúng dạng id đang có (S15 -> S16, PL001 -> PL002)
std::string nextId(const std::vector<std::string>& ids, const std::string& defPrefix){
    std::string prefix = defPrefix;
    size_t width = 3;
    long long mx = 0;
    bool first = true;
    for (const auto& id : ids){
        size_t i = id.size();
        while (i > 0 && std::isdigit((unsigned char)id[i - 1])) --i;
        std::string digits = id.substr(i);
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
