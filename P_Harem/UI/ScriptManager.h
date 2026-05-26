#pragma once
#include <string>
#include <unordered_map>

class C_ScriptManager {
public:
    // 싱글톤 패턴
    static C_ScriptManager& GetInstance() {
        static C_ScriptManager instance;
        return instance;
    }

    // 파일에서 데이터를 로드함 (Data/Scenario.txt)
    void Init();

    // 키를 통해 대사를 가져옴
    const std::string& Get(const std::string& key);

private:
    C_ScriptManager() = default;
    
    // 문자열 양끝 공백 제거 유틸리티
    std::string Trim(const std::string& s);

    std::unordered_map<std::string, std::string> m_scriptMap;
    std::string m_notFound = "[Script Error: Key Not Found]";
};
