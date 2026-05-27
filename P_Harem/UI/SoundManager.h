#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <map>

#pragma comment(lib, "winmm.lib")

/**
 * @class CSoundManager
 * @brief mciSendString을 활용한 사운드 관리 클래스입니다.
 * 사운드 담당자님이 블로그에서 공부하신 방식을 정교화하여 구현했습니다.
 */
class CSoundManager {
public:
    static CSoundManager& GetInstance() {
        static CSoundManager instance;
        return instance;
    }

    /**
     * [PlayBGM] 배경음악을 재생합니다 (자동 루프).
     * @param fileName Sound/BGM/ 폴더 내 파일명
     */
    void PlayBGM(const std::string& fileName) {
        StopBGM();
        std::string path = "Sound/BGM/" + fileName;
        // mci 명령어: 열기 -> 재생 (repeat 옵션으로 무한 루프)
        std::string openCmd = "open \"" + path + "\" type mpegvideo alias bgm";
        mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
        mciSendStringA("play bgm repeat", NULL, 0, NULL);
        m_currentBGM = fileName;
    }

    /**
     * [StopBGM] 현재 재생 중인 배경음악을 중지하고 닫습니다.
     */
    void StopBGM() {
        mciSendStringA("stop bgm", NULL, 0, NULL);
        mciSendStringA("close bgm", NULL, 0, NULL);
        m_currentBGM = "";
    }

    /**
     * [PlaySFX] 효과음을 1회 재생합니다. (중첩 재생 가능)
     * @param fileName Sound/SFX/ 폴더 내 파일명
     */
    void PlaySFX(const std::string& fileName) {
        std::string path = "Sound/SFX/" + fileName;
        // 효과음은 각각 고유한 alias를 부여하여 겹치게 재생할 수 있도록 합니다.
        std::string alias = "sfx_" + std::to_string(m_sfxCounter++);
        std::string openCmd = "open \"" + path + "\" type wavalias " + alias; // wav 형식 대응
        
        // 만약 wav가 아니라면 mpegvideo로 범용 대응
        if (fileName.find(".mp3") != std::string::npos) {
            openCmd = "open \"" + path + "\" type mpegvideo alias " + alias;
        }

        mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
        std::string playCmd = "play " + alias + " wait"; // 효과음은 끝날 때까지 기다리거나 비동기로 실행
        // 'wait'를 빼면 비동기로 재생되어 게임 로직을 방해하지 않습니다.
        mciSendStringA(("play " + alias).c_str(), NULL, 0, NULL);
        
        // 사용 완료된 별칭은 나중에 정리하는 로직이 필요할 수 있으나 콘솔에서는 이 정도로 충분합니다.
    }

    /**
     * [SetVolume] 음량을 조절합니다 (0~1000).
     */
    void SetVolume(int volume) {
        std::string volCmd = "setaudio bgm volume to " + std::to_string(volume);
        mciSendStringA(volCmd.c_str(), NULL, 0, NULL);
    }

private:
    CSoundManager() : m_sfxCounter(0) {}
    ~CSoundManager() { StopBGM(); }

    std::string m_currentBGM;
    int m_sfxCounter;
};
