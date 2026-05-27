#include "ScriptManager.h"
#include <fstream>   // 파일을 읽고 쓰기 위한 도구예요.
#include <iostream>
#include <sstream>
#include <algorithm> // 글자들을 찾고 바꾸는 기능을 위해 필요해요.

/**
 * [Trim] 함수 - 10년 뒤의 너에게
 * 문장 앞뒤에 붙은 불필요한 공백이나, 윈도우에서 생기는 보이지 않는 글자(\r)를 잘라주는 함수야.
 * 이걸 안 하면 글자가 겹쳐 보이거나 커서가 제멋대로 움직일 수 있단다.
 */
std::string C_ScriptManager::Trim(const std::string& s) {
    if (s.empty()) return ""; // 문장이 비어있으면 바로 빈 상자를 돌려줘요.

    // 우리가 공백이라고 부르는 모든 특수 글자들의 목록이란다.
    const std::string whitespace = " \t\n\r\f\v";

    // 1. 앞에서부터 진짜 글자가 시작되는 위치를 찾아요.
    size_t start = s.find_first_not_of(whitespace);
    if (start == std::string::npos) return ""; // 만약 전부 공백뿐이라면 빈 상자를 줘요.

    // 2. 뒤에서부터 진짜 글자가 끝나는 위치를 찾아요.
    size_t end = s.find_last_not_of(whitespace);

    // 3. 시작부터 끝까지 알맹이만 쏙 빼냅니다.
    std::string res = s.substr(start, end - start + 1);
    
    // 4. [버그 수정] 혹시라도 문장 중간에 섞여 있을지 모르는 \r 문자를 한 번 더 완벽히 지워줍니다.
    res.erase(std::remove(res.begin(), res.end(), '\r'), res.end());
    
    return res;
}

/**
 * [Init] 함수
 * 작가님이 작성하신 Scenario.txt 파일을 열어 게임의 모든 대사를 장부에 기록해요.
 */
void C_ScriptManager::Init() {
    // 사진 매니저처럼 두 가지 경로를 모두 확인하는 지혜를 발휘했어.
    std::string paths[] = { "Data/Scenario.txt", "P_Harem/Data/Scenario.txt" };
    std::ifstream file;
    bool loaded = false;
    for (const auto& path : paths) {
        file.open(path);
        if (file.is_open()) { loaded = true; break; }
    }
    if (!loaded) return; // 파일을 못 찾으면 조용히 돌아갑니다.

    std::string line;
    // 파일을 한 줄 한 줄 정성스럽게 읽어나가요.
    while (std::getline(file, line)) {
        line = Trim(line); // 읽자마자 공백부터 깨끗이 치워줘요.
        if (line.empty() || line[0] == '#') continue; // 빈 줄이나 주석(#)은 무시한단다.

        // '키 : 내용' 형식에서 콜론(:)의 위치를 찾아요.
        size_t sep = line.find(':');
        if (sep != std::string::npos) {
            std::string key = Trim(line.substr(0, sep)); // 콜론 앞은 이름표(Key)
            std::string val = Trim(line.substr(sep + 1)); // 콜론 뒤는 실제 대사(Value)

            // 따옴표(" ")로 감싸져 있다면 겉껍질을 벗겨내요.
            if (val.size() >= 2 && val.front() == '"' && val.back() == '"') {
                val = val.substr(1, val.size() - 2);
            }
            
            // [줄바꿈 변환] 작가님이 쓴 \n 글자를 진짜 줄바꿈 신호로 바꿔줘요.
            size_t nPos = 0;
            while ((nPos = val.find("\\n", nPos)) != std::string::npos) {
                val.replace(nPos, 2, "\n");
                nPos += 1;
            }

            // [색상 코드 변환] \u001b 글자를 컴퓨터가 이해하는 색상 신호(\x1b)로 바꿔줘요.
            size_t pos = 0;
            while ((pos = val.find("\\u001b", pos)) != std::string::npos) {
                val.replace(pos, 6, "\x1b");
                pos += 1;
            }

            m_scriptMap[key] = val; // 완성된 대사를 장부(Map)에 기록합니다.
        }
    }
    file.close(); // 다 읽었으니 파일을 닫아주는 매너를 잊지 말렴.
}

/**
 * [Get] 함수
 * "이 대사 가져와!"라고 요청하면 장부에서 찾아 돌려주는 함수야.
 */
const std::string& C_ScriptManager::Get(const std::string& key) {
    auto it = m_scriptMap.find(key);
    if (it != m_scriptMap.end()) return it->second;
    // 만약 못 찾으면 10년 뒤의 너도 알 수 있게 빨간색 에러 메시지를 돌려줄게.
    return m_notFound; 
}
