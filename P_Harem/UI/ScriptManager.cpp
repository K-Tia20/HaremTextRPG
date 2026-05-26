#include "ScriptManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

void C_ScriptManager::Init() {
    std::string paths[] = { "Data/Scenario.txt", "P_Harem/Data/Scenario.txt" };
    std::ifstream file;
    
    bool loaded = false;
    for (const auto& path : paths) {
        file.open(path);
        if (file.is_open()) { loaded = true; break; }
    }

    if (!loaded) return;

    std::string line;
    while (std::getline(file, line)) {
        line = Trim(line);
        if (line.empty() || line[0] == '#') continue;

        size_t sep = line.find(':');
        if (sep != std::string::npos) {
            std::string key = Trim(line.substr(0, sep));
            std::string value = Trim(line.substr(sep + 1));
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }
            m_scriptMap[key] = value;
        }
    }
    file.close();
}

const std::string& C_ScriptManager::Get(const std::string& key) {
    auto it = m_scriptMap.find(key);
    return (it != m_scriptMap.end()) ? it->second : m_notFound;
}

std::string C_ScriptManager::Trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) start++;
    auto end = s.end();
    do { end--; } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}
