#pragma once
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

class CSoundManager
{
public:
    static CSoundManager& GetInstance();

    void PlayBGM(const wchar_t* fileName);
    void StopBGM();

private:
    CSoundManager() = default;
};