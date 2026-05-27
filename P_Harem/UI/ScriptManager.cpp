#include "ScriptManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

std::string C_ScriptManager::Trim(const std::string& s) {
    if (s.empty()) return "";
    const std::string whitespace = " \t\n\r\f\v";
    size_t start = s.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(whitespace);
    std::string res = s.substr(start, end - start + 1);
    res.erase(std::remove(res.begin(), res.end(), '\r'), res.end());
    return res;
}

/**
 * [Init] - ANSI 변환 로직 강화 (\x1b 추가)
 */
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
            std::string val = Trim(line.substr(sep + 1));

            if (val.size() >= 2 && val.front() == '"' && val.back() == '"') {
                val = val.substr(1, val.size() - 2);
            }
            
            // [\n 변환]
            size_t nPos = 0;
            while ((nPos = val.find("\\n", nPos)) != std::string::npos) {
                val.replace(nPos, 2, "\n");
                nPos += 1;
            }

            // [\u001b 변환]
            size_t uPos = 0;
            while ((uPos = val.find("\\u001b", uPos)) != std::string::npos) {
                val.replace(uPos, 6, "\x1b");
                uPos += 1;
            }

            // [\x1b 변환] - 추가된 핵심 로직!
            size_t xPos = 0;
            while ((xPos = val.find("\\x1b", xPos)) != std::string::npos) {
                val.replace(xPos, 4, "\x1b");
                xPos += 1;
            }

            m_scriptMap[key] = val;
        }
    }
    file.close();
}

const std::string& C_ScriptManager::Get(const std::string& key) {
    auto it = m_scriptMap.find(key);
    if (it != m_scriptMap.end()) return it->second;
    return m_notFound;
}
