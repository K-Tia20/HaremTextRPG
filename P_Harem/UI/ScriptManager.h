#pragma once

#include <string>
#include <unordered_map>

/**
 * @class C_ScriptManager
 * @brief 외부 텍스트 파일로부터 시나리오 대사를 로드하고 관리합니다.
 */
class C_ScriptManager {
public:
    static C_ScriptManager& GetInstance() {
        static C_ScriptManager instance;
        return instance;
    }

    void Init();
    const std::string& Get(const std::string& key);

private:
    C_ScriptManager() = default;
    std::string Trim(const std::string& s);

    std::unordered_map<std::string, std::string> m_scriptMap;
    std::string m_notFound = "[Script Error: Key Not Found]";
};
