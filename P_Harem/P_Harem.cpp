
#include "Framework/Framework.h"
#include "GameManager/World.h"

using namespace std;

int main()
{
	shared_ptr<World> _World = make_shared<World>();

	while (true)
	{
		// TODO : 
		// Update호출하면 시작할 때 MainMenu
		// MainMenu에서 시작하면 이름 정하는 함수 호출
		// 이름 정하면 캐릭터 정하는 함수 호출
		// world->Update();
	}
	return 0;
}