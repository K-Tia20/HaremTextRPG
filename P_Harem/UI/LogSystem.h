#pragma once

#include <string>
#include <vector>
#include <memory>

/**
 * @class C_LogSystem
 * @brief 스마트폰의 핵심 기능(캘린더, 연락처, 가방)의 로직과 데이터를 관리합니다.
 */
class C_LogSystem {
public:
    static C_LogSystem& GetInstance() {
        static C_LogSystem instance;
        return instance;
    }

    // 캘린더 관련
    void AddCalendarLog(const std::string& logText);
    const std::vector<std::string>& GetCalendarLogs() const { return m_calendarLogs; }

    // 가방(인벤토리) 확인 UI 로직
    void ShowInventory();

    // 연락처 및 스마트폰 메인 메뉴 로직
    void ShowSmartphoneMenu();
    void ShowContactList();

private:
    C_LogSystem() = default;

    std::vector<std::string> m_calendarLogs; // 게임 내 발생 사건 기록
};
