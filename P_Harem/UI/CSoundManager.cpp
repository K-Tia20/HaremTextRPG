#include "CSoundManager.h"

CSoundManager& CSoundManager::GetInstance()
{
    static CSoundManager instance;
    return instance;
}

void CSoundManager::PlayBGM(const wchar_t* fileName)
{
    PlaySound(fileName, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void CSoundManager::StopBGM()
{
    PlaySound(NULL, 0, 0);
}