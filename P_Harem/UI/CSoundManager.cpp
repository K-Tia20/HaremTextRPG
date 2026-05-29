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



// ⭐ 새로 추가되는 효과음 재생 구현부
void CSoundManager::PlayES(const wchar_t* fileName)
{
    // 유니코드 환경이므로 std::wstring과 mciSendStringW를 사용합니다.
    std::wstring fileStr(fileName);

    // 1. 혹시 이미 열려있을지 모르는 동일 에일리어스(파일명)를 닫습니다.
    std::wstring closeCommand = L"close \"" + fileStr + L"\"";
    mciSendStringW(closeCommand.c_str(), NULL, 0, NULL);

    // 2. WAV 파일을 열고 파일명 자체를 에일리어스(별명)로 지정합니다.
    std::wstring openCommand = L"open \"" + fileStr + L"\" type waveaudio alias \"" + fileStr + L"\"";
    mciSendStringW(openCommand.c_str(), NULL, 0, NULL);

    // 3. 처음(from 0)부터 재생합니다.
    std::wstring playCommand = L"play \"" + fileStr + L"\" from 0";
    mciSendStringW(playCommand.c_str(), NULL, 0, NULL);
}

// ⭐ 필요한 경우 효과음을 도중에 끄기 위한 함수
void CSoundManager::StopES(const wchar_t* fileName)
{
    std::wstring fileStr(fileName);
    std::wstring closeCommand = L"close \"" + fileStr + L"\"";
    mciSendStringW(closeCommand.c_str(), NULL, 0, NULL);
}