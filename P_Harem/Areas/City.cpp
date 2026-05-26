
#include "../Framework/Framework.h"
#include "City.h"


// 생성자에서 World를 받음
C_City::C_City(C_World* world)
{
	C_Area::SetWorld(world);
}
