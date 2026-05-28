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
    
    struct LayerData {
        std::string name;
        int offsetX = 0; // 0 ~ 100 (너비 기준 백분율)
        int offsetY = 0; // 0 ~ 100 (높이 기준 백분율)
        bool flipHorizontal = false; // 좌우 반전 여부
    };

    /**
     * @brief 여러 레이어를 합성합니다.
     * @param bgName 배경 키
     * @param characters 캐릭터 목록 (위치 정보 포함)
     */
    std::string GetLayeredImage(const std::string& bgName, const std::vector<LayerData>& characters);
    std::string GetFullScreenImage(const std::string& assetName);

private:
    C_ImageManager() = default;

    // 이미지의 생 픽셀 데이터를 보관하는 구조체
    struct RawImage {
        std::vector<unsigned char> data;
        int w, h, channels;
    };

    void LoadToRaw(const std::string& name, const std::string& filename);
    std::string ConvertRawToAnsi(const RawImage& img, int maxW, int maxH);

    std::unordered_map<std::string, RawImage> m_rawAssets;
    std::string m_emptyString = "";
};
