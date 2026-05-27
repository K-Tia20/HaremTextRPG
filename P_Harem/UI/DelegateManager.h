#pragma once

class UIManager;
class C_BattleSystem;

/**
 * @class DelegateManager
 * @brief 각 시스템 간의 이벤트 바인딩을 중재합니다.
 */
class DelegateManager {
public:
    void BindAll(UIManager* ui, C_BattleSystem* battleSys);
};
