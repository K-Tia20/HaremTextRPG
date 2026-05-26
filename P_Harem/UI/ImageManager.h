#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class C_ImageManager {
public:
    // 싱글톤 혹은 정적 접근을 위해
    static C_ImageManager& GetInstance() {
        static C_ImageManager instance;
        return instance;
    }

    // 초기화: UI/IMG 폴더 내의 지정된 히로인 이미지들을 미리 로드하고 변환함
    void Init();

    // 변환된 ANSI 문자열을 가져옴
    const std::string& GetCachedImage(const std::string& name);

private:
    C_ImageManager() = default;
    
    // PNG 파일을 읽어 ANSI RGB 문자열(하프 블록 + RLE 압축)로 변환
    std::string ConvertToAnsi(const std::string& filePath, int targetW, int targetH);

    std::unordered_map<std::string, std::string> m_imageCache;
    std::string m_emptyString = "";
};
