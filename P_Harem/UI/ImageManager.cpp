#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ImageManager.h"
#include "UILayoutDefs.h"
#include <iostream>
#include <sstream>
#include <algorithm>

void C_ImageManager::Init() {
    // 예시: 히로인 이미지 로드 (실제 파일이 UI/IMG 폴더에 있어야 함)
    // 뷰포트 크기에 맞춰 타겟 해상도 설정 (하프 블록 사용 시 세로 픽셀은 2배)
    int targetW = UI_LAYOUT::VIEWPORT_W;
    int targetH = UI_LAYOUT::VIEWPORT_H * 2; 

    // 실제 경로 예시 (사용자가 PNG를 저장할 위치)
    m_imageCache["SeA"] = ConvertToAnsi("UI/IMG/SeA.png", targetW, targetH);
    m_imageCache["DoDoHae"] = ConvertToAnsi("UI/IMG/DoDoHae.png", targetW, targetH);
    m_imageCache["YuRi"] = ConvertToAnsi("UI/IMG/YuRi.png", targetW, targetH);
    m_imageCache["Teto"] = ConvertToAnsi("UI/IMG/Teto.png", targetW, targetH);
}

const std::string& C_ImageManager::GetCachedImage(const std::string& name) {
    if (m_imageCache.find(name) != m_imageCache.end()) {
        return m_imageCache[name];
    }
    return m_emptyString;
}

std::string C_ImageManager::ConvertToAnsi(const std::string& filePath, int targetW, int targetH) {
    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 3);
    
    if (!data) return "Image Load Failed: " + filePath;

    std::stringstream ss;
    
    // 이전 색상을 기억하여 RLE 압축 (초기값은 불가능한 값으로 설정)
    int lastFR = -1, lastFG = -1, lastFB = -1;
    int lastBR = -1, lastBG = -1, lastBB = -1;

    for (int y = 0; y < targetH; y += 2) {
        for (int x = 0; x < targetW; x++) {
            // Nearest Neighbor Resampling
            int srcX = (x * width) / targetW;
            int srcY_top = (y * height) / targetH;
            int srcY_bot = ((y + 1) * height) / targetH;

            int topIdx = (srcY_top * width + srcX) * 3;
            int botIdx = (srcY_bot * width + srcX) * 3;

            int fr = data[topIdx], fg = data[topIdx + 1], fb = data[topIdx + 2];
            int br = data[botIdx], bg = data[botIdx + 1], bb = data[botIdx + 2];

            // 색상이 바뀔 때만 ANSI 코드 추가 (최적화)
            bool fChanged = (fr != lastFR || fg != lastFG || fb != lastFB);
            bool bChanged = (br != lastBR || bg != lastBG || bb != lastBB);

            if (fChanged) {
                ss << "\x1b[38;2;" << fr << ";" << fg << ";" << fb << "m";
                lastFR = fr; lastFG = fg; lastFB = fb;
            }
            if (bChanged) {
                ss << "\x1b[48;2;" << br << ";" << bg << ";" << bb << "m";
                lastBR = br; lastBG = bg; lastBB = bb;
            }

            ss << "▀"; // 하프 블록
        }
        ss << "\x1b[0m\n"; // 줄바꿈 시 색상 초기화 (터미널 버그 방지)
        lastFR = lastFG = lastFB = lastBR = lastBG = lastBB = -1; // 라인마다 초기화
    }

    stbi_image_free(data);
    return ss.str();
}
