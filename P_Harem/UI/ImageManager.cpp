#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ImageManager.h"
#include "UILayoutDefs.h"
#include <iostream>
#include <sstream>
#include <algorithm>

void C_ImageManager::Init() {
    // 뷰포트 최대 가용 크기
    int maxW = UI_LAYOUT::VIEWPORT_W - 2;
    int maxH = (UI_LAYOUT::VIEWPORT_H - 2) * 2; 

    auto LoadImage = [&](const std::string& name, const std::string& filename) {
        std::string path1 = "UI/IMG/" + filename;
        std::string path2 = "P_Harem/UI/IMG/" + filename;
        
        std::string result = ConvertToAnsi(path1, maxW, maxH);
        if (result.find("Failed") != std::string::npos) {
            result = ConvertToAnsi(path2, maxW, maxH);
        }
        m_imageCache[name] = result;
    };

    LoadImage("SeA", "SeA.jpg");
    LoadImage("DoDoHae", "DoDoHae.png");
    LoadImage("YuRi", "YuRi.png");
    LoadImage("Teto", "Teto.png");
}

const std::string& C_ImageManager::GetCachedImage(const std::string& name) {
    if (m_imageCache.find(name) != m_imageCache.end()) {
        return m_imageCache[name];
    }
    return m_emptyString;
}

std::string C_ImageManager::ConvertToAnsi(const std::string& filePath, int maxW, int maxH) {
    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 3);
    
    if (!data) return "Image Load Failed: " + filePath;

    // --- 비율 유지 리사이징 계산 ---
    float srcRatio = (float)width / height;
    float boxRatio = (float)maxW / maxH;

    int targetW, targetH;
    if (srcRatio > boxRatio) {
        // 가로가 상대적으로 더 긴 경우 (가로에 맞춤)
        targetW = maxW;
        targetH = (int)(maxW / srcRatio);
    } else {
        // 세로가 상대적으로 더 긴 경우 (세로에 맞춤)
        targetH = maxH;
        targetW = (int)(maxH * srcRatio);
    }
    // 하프 블록을 위해 세로 픽셀 수는 무조건 짝수여야 함
    if (targetH % 2 != 0) targetH--;
    if (targetH < 2) targetH = 2;

    std::stringstream ss;
    
    // 중앙 정렬을 위한 오프셋 정보 삽입 (DrawImage에서 사용하도록 문자열 맨 앞에 특수 태그 삽입 가능하나, 
    // 여기서는 단순하게 각 라인 앞에 공백을 넣는 방식으로 최적화)
    int xPadding = (maxW - targetW) / 2;

    int lastFR = -1, lastFG = -1, lastFB = -1;
    int lastBR = -1, lastBG = -1, lastBB = -1;

    for (int y = 0; y < targetH; y += 2) {
        // 가로 중앙 정렬을 위한 공백 추가
        if (xPadding > 0) ss << std::string(xPadding, ' ');

        for (int x = 0; x < targetW; x++) {
            int srcX = (x * width) / targetW;
            int srcY_top = (y * height) / targetH;
            int srcY_bot = ((y + 1) * height) / targetH;

            int topIdx = (srcY_top * width + srcX) * 3;
            int botIdx = (srcY_bot * width + srcX) * 3;

            int fr = data[topIdx], fg = data[topIdx + 1], fb = data[topIdx + 2];
            int br = data[botIdx], bg = data[botIdx + 1], bb = data[botIdx + 2];

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
            ss << "▀"; 
        }
        ss << "\x1b[0m\n"; 
        lastFR = lastFG = lastFB = lastBR = lastBG = lastBB = -1;
    }

    stbi_image_free(data);
    return ss.str();
}
