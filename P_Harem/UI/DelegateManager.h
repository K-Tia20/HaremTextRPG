#pragma once 
class UIManager;
class C_World; // 모든 로직의 집합체

class DelegateManager
{
public:
    // "조립 한 줄"을 담당할 함수
    static void Connect(C_World& world, UIManager& ui);
};