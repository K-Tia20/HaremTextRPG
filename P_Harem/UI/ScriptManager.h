#pragma once

#include <string>
#include <unordered_map>
#include <vector>

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
    
    /**
     * @brief 키를 통해 대사를 가져옵니다.
     */
    const std::string& Get(const std::string& key);

    /**
     * @brief 플레이스홀더({0}, {1}...)를 포함한 대사를 가져와 실제 데이터로 치환합니다.
     * @param key 시나리오 키
     * @param args 치환할 데이터들 (가변 인자)
     */
    template<typename... Args>
    std::string GetFormat(const std::string& key, Args... args) {
        std::string raw = Get(key);
        std::vector<std::string> vArgs = { std::to_string(args)... };
        for (size_t i = 0; i < vArgs.size(); ++i) {
            std::string placeholder = "{" + std::to_string(i) + "}";
            size_t pos = raw.find(placeholder);
            while (pos != std::string::npos) {
                raw.replace(pos, placeholder.length(), vArgs[i]);
                pos = raw.find(placeholder, pos + vArgs[i].length());
            }
        }
        return raw;
    }

    /**
     * @brief 문자열 전용 포맷팅 함수입니다. (std::to_string이 지원되지 않는 문자열용)
     */
    std::string GetFormatStr(const std::string& key, const std::vector<std::string>& args) {
        std::string raw = Get(key);
        for (size_t i = 0; i < args.size(); ++i) {
            std::string placeholder = "{" + std::to_string(i) + "}";
            size_t pos = raw.find(placeholder);
            while (pos != std::string::npos) {
                raw.replace(pos, placeholder.length(), args[i]);
                pos = raw.find(placeholder, pos + args[i].length());
            }
        }
        return raw;
    }

private:
    C_ScriptManager() = default;
    std::string Trim(const std::string& s);

    std::unordered_map<std::string, std::string> m_scriptMap;
    std::string m_notFound = "[Script Error: Key Not Found]";
};
