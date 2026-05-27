#include "Framework/Framework.h"
#include "GameManager/World.h"

using namespace std;

/**
 * [Main Entry Point] - 팀원들의 의도를 100% 반영한 버전
 * 팀원들은 shared_ptr을 통한 명시적 생명주기 관리를 선호하십니다.
 * 그 뜻을 존중하여 구조를 다시 조립했습니다.
 */
int main()
{
	// 1. 초기 환경 설정 (UTF-8 한글 출력 및 랜덤 시드)
	system("chcp 65001");
	srand(static_cast<unsigned int>(time(nullptr)));

	// 2. [팀의 의도 수용] 싱글톤이 아닌 shared_ptr로 월드를 생성합니다.
	shared_ptr<C_World> World = make_shared<C_World>();

	while (true)
	{
		// 3. 월드의 업데이트 루프 가동
		// 이 안에서 팀원들이 짠 로직이 우리 UI 프레임 안에서 돌아가게 됩니다.
        // 첫 프레임에서 Init()과 오프닝이 실행되도록 설계되었습니다.
		World->Update();
	}

	return 0;
}
