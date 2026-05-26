#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ImageManager.h"
#include "UILayoutDefs.h"
#include <iostream>
#include <sstream>
#include <algorithm>

void C_ImageManager::Init() {
    int maxW = UI_LAYOUT::VIEWPORT_W - 2;
    int maxH = (UI_LAYOUT::VIEWPORT_H - 2) * 2;

    auto LoadImage = [&](const std::string& name, const std::string& filename) {
        std::string paths[] = { "UI/IMG/" + filename, "P_Harem/UI/IMG/" + filename };
        std::string result;
        for (const auto& path : paths) {
            result = ConvertToAnsi(path, maxW, maxH);
            if (result.find("Failed") == std::string::npos) break;
        }
        m_imageCache[name] = result;
    };

    LoadImage("SeA", "SeA.jpg");
    LoadImage("DoDoHae", "DoDoHae.png");
    LoadImage("YuRi", "YuRi.png");
    LoadImage("Teto", "Teto.png");
}

const std::string& C_ImageManager::GetCachedImage(const std::string& name) {
    auto it = m_imageCache.find(name);
    return (it != m_imageCache.end()) ? it->second : m_emptyString;
}

std::string C_ImageManager::ConvertToAnsi(const std::string& filePath, int maxW, int maxH) {
    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 3);
    if (!data) return "Image Load Failed: " + filePath;

    float srcRatio = (float)width / height;
    float boxRatio = (float)maxW / maxH;

    int targetW, targetH;
    if (srcRatio > boxRatio) {
        targetW = maxW;
        targetH = (int)(maxW / srcRatio);
    } else {
        targetH = maxH;
        targetW = (int)(maxH * srcRatio);
    }
    if (targetH % 2 != 0) targetH--;
    if (targetH < 2) targetH = 2;

    std::stringstream ss;
    int xPadding = (maxW - targetW) / 2;
    int lastFR = -1, lastFG = -1, lastFB = -1;
    int lastBR = -1, lastBG = -1, lastBB = -1;

    for (int y = 0; y < targetH; y += 2) {
        ss << "\x1b[0m"; // Reset state per line to avoid bleeding
        if (xPadding > 0) ss << std::string(xPadding, ' ');

        for (int x = 0; x < targetW; x++) {
            int srcX = (x * width) / targetW;
            int topIdx = ((y * height / targetH) * width + srcX) * 3;
            int botIdx = (((y + 1) * height / targetH) * width + srcX) * 3;

            int fr = data[topIdx], fg = data[topIdx + 1], fb = data[topIdx + 2];
            int br = data[botIdx], bg = data[botIdx + 1], bb = data[botIdx + 2];

            if (fr != lastFR || fg != lastFG || fb != lastFB) {
                ss << "\x1b[38;2;" << fr << ";" << fg << ";" << fb << "m";
                lastFR = fr; lastFG = fg; lastFB = fb;
            }
            if (br != lastBR || bg != lastBG || bb != lastBB) {
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
