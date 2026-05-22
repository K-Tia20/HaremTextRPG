
#include "Framework/Framework.h"
#include "GameManager/World.h"

using namespace std;

int main()
{
	shared_ptr<C_World> World = make_shared<C_World>();

	while (true)
	{
		// Update호출하면 시작할 때 MainMenu
		World->Update();
	}
	return 0;
}