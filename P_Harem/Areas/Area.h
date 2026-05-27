#pragma once

// 지역들의 부모 클래스
class C_World;

class C_Area
{
public:
	C_Area();

	virtual void SelectMenu() = 0;
	virtual void MoveArea() = 0;
	virtual void Update() = 0;
	virtual void Encounter() = 0;

	// 월드 가져오는 용도 (월드의 상태를 변경하기 위함)
	void SetWorld(C_World* world);
	C_World* GetWorld() { return World; }

protected:
	C_World* World = nullptr;
};

