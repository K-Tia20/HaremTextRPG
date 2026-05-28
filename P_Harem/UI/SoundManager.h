#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <iostream>

#pragma comment(lib, "winmm.lib")
/*
class CSoundManager {
public:
    static CSoundManager& GetInstance() {
        static CSoundManager instance;
        return instance;
    }

    // =========================
    // BGM (WAV / MCI)
    // =========================
    void PlayBGM(const std::string& fileName) {
        StopBGM();

        std::string path = "Sound/BGM/" + fileName;

        std::string openCmd =
            "open \"" + path + "\" type waveaudio alias bgm";

        MCIERROR err = mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
        if (err != 0) {
            PrintError(err);
            return;
        }

        err = mciSendStringA("play bgm repeat", NULL, 0, NULL);
        if (err != 0) {
            PrintError(err);
            return;
        }

        m_currentBGM = fileName;
    }

    void StopBGM() {
        mciSendStringA("stop bgm", NULL, 0, NULL);
        mciSendStringA("close bgm", NULL, 0, NULL);
        m_currentBGM.clear();
    }

    // =========================
    // SFX (WAV / MCI)
    // =========================
    void PlaySFX(const std::string& fileName) {

        std::string path = "Sound/SFX/" + fileName;

        std::string alias = "sfx_" + std::to_string(m_sfxCounter++);

        std::string openCmd =
            "open \"" + path + "\" type waveaudio alias " + alias;

        MCIERROR err = mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
        if (err != 0) {
            PrintError(err);
            return;
        }

        std::string playCmd = "play " + alias;

        err = mciSendStringA(playCmd.c_str(), NULL, 0, NULL);
        if (err != 0) {
            PrintError(err);
            return;
        }

        // SFX는 자동 close 안함 (짧게 재생 후 종료)
    }

    // =========================
    // Volume (BGM only)
    // =========================
    void SetVolume(int volume) {
        std::string cmd =
            "setaudio bgm volume to " + std::to_string(volume);

        mciSendStringA(cmd.c_str(), NULL, 0, NULL);
    }

private:
    CSoundManager() : m_sfxCounter(0) {}
    ~CSoundManager() { StopBGM(); }

    void PrintError(MCIERROR err) {
        char msg[256];
        mciGetErrorStringA(err, msg, 256);
        std::cout << "[MCI ERROR] " << msg << std::endl;
    }

private:
    std::string m_currentBGM;
    int m_sfxCounter = 0;
}; */