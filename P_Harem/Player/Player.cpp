#include "../Framework/Framework.h"
#include "../GameManager/World.h"
#include "../UI/UIManager.h"
#include "../Creature/Creature.h"
#include "../Inventory/Inventory.h"
#include "Player.h"

using namespace std;

C_Player::C_Player() : Money(0)
{
    Inventory = make_shared<C_Inventory>();
}

shared_ptr<C_Creature> C_Player::SetFightGirl()
{
    auto ui = C_World::GetInstance().GetUI();
    if (GirlFrends.empty()) {
        ui->PrintLog("시스템: 함께 싸울 여자친구가 없습니다.");
        return nullptr;
    }

    ui->PrintLog("--- 출격 히로인 선택 ---");
    for (size_t i = 0; i < GirlFrends.size(); ++i) {
        ui->PrintLog(to_string(i + 1) + ". " + GirlFrends[i]->GetName() + " (체력: " + to_string(GirlFrends[i]->GetCurrentHp()) + ")");
    }
    ui->PrintLog("0. 뒤로 가기 | 누구를 보낼까요? : [INPUT]");

    int choice = InputInt();
    if (choice <= 0 || choice > (int)GirlFrends.size()) return nullptr;

    return GirlFrends[choice - 1];
}

void C_Player::SetName(string name) { Name = name; }
void C_Player::AddGirlFrends(shared_ptr<C_Creature> girlfrend) { GirlFrends.push_back(girlfrend); }
void C_Player::AddMoney(int addMoney) { Money += addMoney; }
void C_Player::SubMoney(int subMoney) { Money -= subMoney; }
void C_Player::AddItem(shared_ptr<C_Items> item) { if (Inventory) Inventory->AddItem(item); }

shared_ptr<C_Items> C_Player::UsingItem() {
    if (!Inventory || Inventory->GetSize() <= 0) return nullptr;
    int randomIndex = rand() % Inventory->GetSize();
    auto item = Inventory->GetItem(randomIndex);
    Inventory->SubItem(randomIndex);
    return item;
}

int C_Player::GetHighlevel() {
    if (GirlFrends.empty()) return 1;
    int high = 1;
    for (auto& g : GirlFrends) { if (g->GetLevel() > high) high = g->GetLevel(); }
    return high;
}

void C_Player::RemoveGirlFriend(std::shared_ptr<C_Creature> girl) {
	GirlFrends.erase(std::remove(GirlFrends.begin(), GirlFrends.end(), girl), GirlFrends.end());
}

string C_Player::InputString() {
    auto ui = C_World::GetInstance().GetUI();
    if (ui) ui->SetCursorToInputArea();
    string value;
    getline(cin >> ws, value);
    if (ui) ui->DrawInputBox(""); 
    return value;
}

int C_Player::InputInt() {
    auto ui = C_World::GetInstance().GetUI();
    int value;
    while (true) {
        if (ui) ui->SetCursorToInputArea();
        if (cin >> value) {
            string dummy; getline(cin, dummy);
            if (ui) ui->DrawInputBox(""); 
            return value;
        }
        cin.clear();
        string dummy; getline(cin, dummy);
        if (ui) ui->PrintLog("\x1b[31mError: 숫자만 입력 가능합니다.\x1b[0m");
    }
}
