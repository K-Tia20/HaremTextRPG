#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ImageManager.h"
#include "UILayoutDefs.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace UI_LAYOUT;

void C_ImageManager::Init() {
    auto LoadWithFallback = [&](const std::string& name, const std::string& filename) {
        std::string paths[] = { 
            "UI/IMG/" + filename, 
            "P_Harem/UI/IMG/" + filename,
            "../P_Harem/UI/IMG/" + filename,
            "../../P_Harem/UI/IMG/" + filename
        };
        bool success = false;
        for (const auto& path : paths) {
            LoadToRaw(name, path);
            if (m_rawAssets.count(name)) { success = true; break; }
        }
        // [수정] 리소스 누락 시 로그가 화면을 찢지 않도록 로직 상으로만 경고
    };

    // [배경 로드]
    LoadWithFallback("BG_Title", "BG_Title.png");
    LoadWithFallback("BG_TeamName", "BG_TeamName.png");
    LoadWithFallback("BG_City", "BG_City.png");
    LoadWithFallback("BG_City2", "BG_City2.png");
    LoadWithFallback("BG_Room", "BG_Room.png");
    LoadWithFallback("BG_YogerPresso_I", "BG_YogerPresso_I.png");
    LoadWithFallback("BG_YogerPresso_O", "BG_YogerPresso_O.png");
    LoadWithFallback("BG_Bemin", "BG_Bemin.png");
    LoadWithFallback("BG_Coupang", "BG_Coupang.png");
    LoadWithFallback("BG_Danggun", "BG_Danggun.png");
    LoadWithFallback("BG_HunPo", "BG_HunPo.png");
    LoadWithFallback("BG_Wedding", "BG_Wedding.png");
    //LoadWithFallback("BG_Boss", "BG_Boss.png");

    // [캐릭터 로드]
    LoadWithFallback("CH_Red", "CH_Red.png");
    LoadWithFallback("CH_Green", "CH_Green.png");
    LoadWithFallback("CH_Blue", "CH_Blue.png");
    LoadWithFallback("CH_And", "CH_And.png");
    LoadWithFallback("CH_Chashrin", "CH_Chashrin.png");
    LoadWithFallback("CH_Codelia", "CH_Codelia.png");
    LoadWithFallback("CH_Lina", "CH_Lina.png");
    LoadWithFallback("CH_Marry", "CH_Marry.png");
    LoadWithFallback("CH_Oplier", "CH_Oplier.png");
    LoadWithFallback("CH_Zhad", "CH_Zhad.png");
    LoadWithFallback("CH_Normal", "CH_Normal.png");
    LoadWithFallback("CH_Null", "CH_Null.png");
    LoadWithFallback("CH_Hanma", "CH_Hanma.png");
}

void C_ImageManager::LoadToRaw(const std::string& name, const std::string& filePath) {
    int w, h, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &channels, 4);
    
    if (data) {
        RawImage raw;
        raw.w = w; raw.h = h; raw.channels = 4;
        raw.data.assign(data, data + (w * h * 4));
        m_rawAssets[name] = raw;
        stbi_image_free(data);
    }
}

std::string C_ImageManager::GetLayeredImage(const std::string& bgName, const std::vector<LayerData>& characters) {
    int maxW = VIEWPORT_W - 2;
    int maxH = (VIEWPORT_H - 2) * 2;

    std::vector<unsigned char> canvas(maxW * maxH * 4, 0);

    auto BlendLayer = [&](const std::string& assetName, int startX, int startY, int drawW, int drawH, bool isBG, bool flipHorizontal) {
        if (assetName.empty() || m_rawAssets.find(assetName) == m_rawAssets.end()) return;
        const auto& asset = m_rawAssets[assetName];

        for (int y = 0; y < drawH; ++y) {
            for (int x = 0; x < drawW; ++x) {
                int destX = startX + x;
                int destY = startY + y;
                if (destX < 0 || destX >= maxW || destY < 0 || destY >= maxH) continue;

                int srcX = flipHorizontal ? (((drawW - 1 - x) * asset.w) / drawW) : ((x * asset.w) / drawW);
                int srcY = (y * asset.h) / drawH;
                int srcIdx = (srcY * asset.w + srcX) * 4;
                int destIdx = (destY * maxW + destX) * 4;

                float alpha = asset.data[srcIdx + 3] / 255.0f;
                
                if (isBG) {
                    for (int c = 0; c < 3; ++c) canvas[destIdx + c] = asset.data[srcIdx + c];
                } else {
                    for (int c = 0; c < 3; ++c) {
                        canvas[destIdx + c] = (unsigned char)(asset.data[srcIdx + c] * alpha + canvas[destIdx + c] * (1.0f - alpha));
                    }
                }
            }
        }
    };

    // 1. 배경 합성
    BlendLayer(bgName, 0, 0, maxW, maxH, true, false);

    // 2. 캐릭터들 합성
    for (const auto& charData : characters) {
        if (m_rawAssets.find(charData.name) == m_rawAssets.end()) continue;
        const auto& asset = m_rawAssets[charData.name];
        float ratio = (float)asset.w / asset.h;
        int charH = maxH;
        int charW = (int)(charH * ratio);
        if (charW > maxW / 2) charW = maxW / 2;
        int startX = (charData.offsetX * (maxW - charW)) / 100;
        int startY = maxH - charH; 
        BlendLayer(charData.name, startX, startY, charW, charH, false, charData.flipHorizontal);
    }

    // 3. ANSI 변환
    std::stringstream ss;
    int lastFR = -1, lastFG = -1, lastFB = -1;
    int lastBR = -1, lastBG = -1, lastBB = -1;

    for (int y = 0; y < maxH; y += 2) {
        ss << "\x1b[0m"; 
        for (int x = 0; x < maxW; ++x) {
            int topIdx = (y * maxW + x) * 4;
            int botIdx = ((y + 1) * maxW + x) * 4;
            int fr = canvas[topIdx], fg = canvas[topIdx + 1], fb = canvas[topIdx + 2];
            int br = canvas[botIdx], bg = canvas[botIdx + 1], bb = canvas[botIdx + 2];
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
    return ss.str();
}

/**
 * [GetFullScreenImage] - 화면 전체(160x45)를 덮는 이미지를 생성합니다.
 */
std::string C_ImageManager::GetFullScreenImage(const std::string& assetName) {
    if (m_rawAssets.find(assetName) == m_rawAssets.end()) return "";
    const auto& asset = m_rawAssets[assetName];

    int maxW = CONSOLE_WIDTH;
    int maxH = CONSOLE_HEIGHT * 2; // 각 글자가 2픽셀이므로 90픽셀

    std::stringstream ss;
    int lastFR = -1, lastFG = -1, lastFB = -1;
    int lastBR = -1, lastBG = -1, lastBB = -1;

    for (int y = 0; y < maxH; y += 2) {
        ss << "\x1b[0m";
        for (int x = 0; x < maxW; ++x) {
            // 원본 이미지에서 픽셀 샘플링 (Nearst Neighbor)
            int srcX1 = (x * asset.w) / maxW;
            int srcY1 = (y * asset.h) / maxH;
            int srcX2 = (x * asset.w) / maxW;
            int srcY2 = ((y + 1) * asset.h) / maxH;

            int idx1 = (srcY1 * asset.w + srcX1) * 4;
            int idx2 = (srcY2 * asset.w + srcX2) * 4;

            int fr = asset.data[idx1], fg = asset.data[idx1+1], fb = asset.data[idx1+2];
            int br = asset.data[idx2], bg = asset.data[idx2+1], bb = asset.data[idx2+2];

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
    return ss.str();
}
