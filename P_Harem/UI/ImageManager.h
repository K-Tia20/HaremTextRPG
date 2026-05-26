#pragma once

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class C_ImageManager
 * @brief PNG 이미지를 로드하여 ANSI RGB 문자열로 변환 및 캐싱합니다.
 */
class C_ImageManager {
public:
    static C_ImageManager& GetInstance() {
        static C_ImageManager instance;
        return instance;
    }

    void Init();
    const std::string& GetCachedImage(const std::string& name);

private:
    C_ImageManager() = default;
    std::string ConvertToAnsi(const std::string& filePath, int maxW, int maxH);

    std::unordered_map<std::string, std::string> m_imageCache;
    std::string m_emptyString = "";
};
