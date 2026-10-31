#pragma once
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace textutil{

std::string trim(const std::string& s){
    size_t b = 0, e = s.size();
    while (b < e && std::isspace((unsigned char)s[b])) ++b;
    while (e > b && std::isspace((unsigned char)s[e - 1])) --e;
    return s.substr(b, e - b);
}
std::string lower(std::string s){
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    return s;
}
int toInt(const std::string& s, int def = 0){
    try{
        return std::stoi(s);
    }
    catch (...){
        return def;
    }
}
long long toLL(const std::string& s, long long def = 0){
    try{
        return std::stoll(s);
    }
    catch (...){
        return def;
    }
}
// Đoán ký tự phân cách của một dòng dữ liệu
char detectDelim(const std::string& line){
    for (char c : {'|', ';', '\t', ','}) if (line.find(c) != std::string::npos) return c;
    return '|';
}
std::vector<std::string> split(const std::string& line, char d){
    std::vector<std::string> parts;
    std::string item;
    std::stringstream ss(line);
    while (std::getline(ss, item, d)) parts.push_back(trim(item));
    return parts;
}
// Đọc các dòng có dữ liệu (bỏ BOM, bỏ \r, bỏ dòng trống và dòng chú thích bắt đầu bằng #)
std::vector<std::string> readLines(const std::string& path){
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
