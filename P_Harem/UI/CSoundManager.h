#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <string> // std::wstring 사용을 위해 추가

#pragma comment(lib, "winmm.lib")

class CSoundManager
{
public:
    static CSoundManager& GetInstance();

    void PlayBGM(const wchar_t* fileName);
    void StopBGM();
    
    // ⭐ 기존 스타일을 맞춘 WAV 효과음 재생/정지 함수 추가
    void PlayES(const wchar_t* fileName);
    void StopES(const wchar_t* fileName);
    
private:
    CSoundManager() = default;
};