#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // 외부 사진 파일(PNG)을 읽어오는 도구예요.
#include "ImageManager.h" 
#include "UILayoutDefs.h"
#include <iostream>
#include <sstream>
#include <algorithm> // 숫자의 크기를 비교할 때(std::min, max) 필요해요.

/**
 * [Init] 함수
 * 게임이 시작될 때 필요한 사진들을 미리 다 읽어서 창고에 정리해두는 함수란다.
 */
void C_ImageManager::Init() {
    // 사진을 찾을 때 프로그램의 실행 위치에 따라 다를 수 있어서 두 가지 경로를 다 준비했어.
    auto LoadWithFallback = [&](const std::string& name, const std::string& filename) {
        std::string paths[] = { "UI/IMG/" + filename, "P_Harem/UI/IMG/" + filename };
        bool success = false;
        for (const auto& path : paths) {
            LoadToRaw(name, path); // 사진 읽기 시도!
            if (m_rawAssets.count(name)) { success = true; break; }
        }
        // 만약 사진이 없으면 10년 뒤의 너도 알 수 있게 빨간색 에러를 띄워줄게.
        if (!success) std::cerr << "[Art Error] 사진을 찾을 수 없단다: " << filename << std::endl;
    };

    // 배경과 캐릭터 사진들을 창고에 차곡차곡 넣습니다.
    LoadWithFallback("BG_Test", "BG_Test.png");
    LoadWithFallback("CH_Red", "CH_Red.png");
    LoadWithFallback("CH_Green", "CH_Green.png");
    LoadWithFallback("CH_Blue", "CH_Blue.png");
    LoadWithFallback("CH_Normal", "CH_Normal.png");
    LoadWithFallback("SeA", "SeA.png");
}

/**
 * [LoadToRaw] 함수
 * 사진 파일에서 '투명도(Alpha)' 정보를 포함한 생동감 넘치는 색상 데이터를 추출해와요.
 */
void C_ImageManager::LoadToRaw(const std::string& name, const std::string& filePath) {
    int w, h, channels;
    // 4번 채널(RGBA)을 강제로 읽어서 투명한 배경을 살려냅니다.
    unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &channels, 4);
    
    if (data) {
        RawImage raw;
        raw.w = w; raw.h = h; raw.channels = 4;
        raw.data.assign(data, data + (w * h * 4)); // 메모리 바구니에 복사!
        m_rawAssets[name] = raw; // 이름표를 붙여서 보관.
        stbi_image_free(data); // 임시 데이터는 청소해요.
    }
}

/**
 * [GetLayeredImage] 함수 - 레이어 합성 엔진
 * 배경 위에 여러 캐릭터를 나란히 세워서 '완성된 한 장의 장면'을 만드는 함수란다.
 */
std::string C_ImageManager::GetLayeredImage(const std::string& bgName, const std::vector<LayerData>& characters) {
    int maxW = UI_LAYOUT::VIEWPORT_W - 2; // 뷰포트 테두리를 뺀 실제 너비
    int maxH = (UI_LAYOUT::VIEWPORT_H - 2) * 2; // 하프 블록을 쓰니까 세로 해상도는 2배!

    // 1. 도화지(Canvas)를 준비해요. 처음엔 투명(0)하게 시작합니다.
    std::vector<unsigned char> canvas(maxW * maxH * 4, 0);

    // [합성 기술: BlendLayer]
    // 기존 그림 위에 새로운 그림을 '투명도'를 따져가며 겹쳐 그리는 함수야.
    auto BlendLayer = [&](const std::string& assetName, int startX, int startY, int drawW, int drawH, bool isBG) {
        if (assetName.empty() || m_rawAssets.find(assetName) == m_rawAssets.end()) return;
        const auto& asset = m_rawAssets[assetName];

        for (int y = 0; y < drawH; ++y) {
            for (int x = 0; x < drawW; ++x) {
                int destX = startX + x;
                int destY = startY + y;
                // 도화지 밖으로 나가는 픽셀은 그리지 않아요 (안전장치).
                if (destX < 0 || destX >= maxW || destY < 0 || destY >= maxH) continue;

                // 사진의 어느 지점 색상을 가져올지 계산해요 (Nearest Neighbor).
                int srcX = (x * asset.w) / drawW;
                int srcY = (y * asset.h) / drawH;
                int srcIdx = (srcY * asset.w + srcX) * 4;
                int destIdx = (destY * maxW + destX) * 4;

                float alpha = asset.data[srcIdx + 3] / 255.0f; // 0.0(투명) ~ 1.0(불투명)
                
                if (isBG) {
                    // 1층: 배경은 무조건 덮어씌워요.
                    for (int c = 0; c < 3; ++c) canvas[destIdx + c] = asset.data[srcIdx + c];
                } else {
                    // 2층 이상: 캐릭터나 효과는 알파 블렌딩 공식을 써서 섞어준단다.
                    // 최종색 = (새그림색 * 알파) + (원래있던색 * (1-알파))
                    for (int c = 0; c < 3; ++c) {
                        canvas[destIdx + c] = (unsigned char)(asset.data[srcIdx + c] * alpha + canvas[destIdx + c] * (1.0f - alpha));
                    }
                }
            }
        }
    };

    // 1. 먼저 배경을 깔아요.
    BlendLayer(bgName, 0, 0, maxW, maxH, true);

    // 2. 그 위에 캐릭터들을 나란히 배치합니다 (비율 유지!).
    for (const auto& charData : characters) {
        if (m_rawAssets.find(charData.name) == m_rawAssets.end()) continue;
        const auto& asset = m_rawAssets[charData.name];

        // [버그 수정] 캐릭터가 찌그러지지 않게 원본 비율을 계산해요.
        float ratio = (float)asset.w / asset.h;
        int charH = maxH; // 높이는 화면에 꽉 맞추고
        int charW = (int)(charH * ratio); // 너비는 비율에 맞춰서 조절!

        // 너무 뚱뚱한 이미지는 뷰포트의 절반까지만 허용해요.
        if (charW > maxW / 2) charW = maxW / 2;

        // X 좌표는 사용자가 지정한 위치(0~100%)에 따라 배치하고, Y는 바닥에 딱 붙게!
        int startX = (charData.offsetX * (maxW - charW)) / 100;
        int startY = maxH - charH; 

        BlendLayer(charData.name, startX, startY, charW, charH, false);
    }

    // 3. 합성된 픽셀 도화지를 콘솔 전용 색상 코드로 바꿉니다.
    std::stringstream ss;
    int lastFR = -1, lastFG = -1, lastFB = -1;
    int lastBR = -1, lastBG = -1, lastBB = -1;

    for (int y = 0; y < maxH; y += 2) {
        ss << "\x1b[0m"; // 줄 시작할 때마다 색상 초기화! (색 번짐 방지)
        for (int x = 0; x < maxW; ++x) {
            int topIdx = (y * maxW + x) * 4; // 위쪽 픽셀
            int botIdx = ((y + 1) * maxW + x) * 4; // 아래쪽 픽셀

            int fr = canvas[topIdx], fg = canvas[topIdx + 1], fb = canvas[topIdx + 2];
            int br = canvas[botIdx], bg = canvas[botIdx + 1], bb = canvas[botIdx + 2];

            // 색상이 바뀔 때만 명령어를 추가해서 속도를 높였어 (최적화).
            if (fr != lastFR || fg != lastFG || fb != lastFB) {
                ss << "\x1b[38;2;" << fr << ";" << fg << ";" << fb << "m";
                lastFR = fr; lastFG = fg; lastFB = fb;
            }
            if (br != lastBR || bg != lastBG || bb != lastBB) {
                ss << "\x1b[48;2;" << br << ";" << bg << ";" << bb << "m";
                lastBR = br; lastBG = bg; lastBB = bb;
            }
            ss << "▀"; // 하프 블록 하나가 두 개의 픽셀색을 동시에 보여준단다.
        }
        ss << "\x1b[0m\n"; // 한 줄이 끝나면 엔터!
        lastFR = lastFG = lastFB = lastBR = lastBG = lastBB = -1;
    }

    return ss.str(); // 완성된 한 장의 그림(문자열)을 돌려줍니다.
}
