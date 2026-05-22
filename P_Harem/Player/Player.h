
#pragma once

#include <iostream>
#include <string.h>

using namespace std;

class Charater 
{
public:

	Charater(string name, int hp, int mp, int attack, int defense, int like) : name(name), hp(hp), mp(mp), attack(attack), defense(defense), like(like) {}; //  초기화 
	virtual ~Charater() {} // 부모 클래스의 소멸자는 반드시 virtual

protected:

	string name; //플레이어 이름
	int hp = 200; //체력
	int mp = 100; //마나
	int attack = 30; //공격력
	int defense; //방어력
	int like; //호감도

};

class Player : public Charater
{
public:

	Player(string name, int hp, int mp, int attack, int defense, int like) : Charater(name, hp, mp, attack, defense,like){} //플레이어의 정보 설정 (체력, 마나, 공격력, 방어력, 호감도)
	
	//레벨이 올라갈 때 처리하는 함수 묶음 

	void hpUP() { hp *= playerexp; }
	void mpUP() { mp *= playerexp; }
	void attackUP() { attack *= playerexp; }
	void defenseUP() { defense *= playerexp; }
	void likeUP() { like *= playerexp; }

	~Player(){}; //플레이어의 정보 초기화

protected:

	int playerexp = 0; //경험치
	int like; //호감도
	int level = 1; //레벨

};


class Enemy : public Charater // 적 캐릭터의 정보 설정 (체력, 마나, 공격력, 방어력, 호감도)
{
public:

	Enemy(string name, int hp, int mp, int attack, int defense, int like); //적 캐릭터의 정보 설정 (체력, 마나, 공격력, 방어력, 호감도)

	void hpUP() {} // Level이 올라가면 체력이 증가하는 함수
	void mpUP() {} // Level이 올라가면 마력이 증가하는 함수
	void attackUP() {} // Level이 올라가면 공격력이 증가하는 함수
	void defenseUP() {} // Level이 올라가면 방어력 증가하는 함수

	~Enemy(){}; //적 캐릭터의 정보 초기화

protected:

	string name; //적 캐릭터 이름
	string dropbitemname; //적 캐릭터가 드랍하는 아이템
	int enemylevel; //적 캐릭터의 레벨
	int enemyhp; //적 캐릭터의 체력
	int enemymp; //적 캐릭터의 마나
	int enemylike; //적 캐릭터의 호감도
	int enemyattack; //적 캐릭터의 공격력
	int enemydefense; //적 캐릭터의 방어력
	int dropcoin; //적 캐릭터가 드랍하는 코인
	int dropexp; //적 캐릭터가 드랍하는 경험치

};

class FireEnemy : public Enemy, Player // 불 속성의 적 캐릭터 (핫녀)
{
	FireEnemy(string name, int hp, int mp, int attack, int defense, int like); //불 속성의 적 캐릭터의 정보 설정 (체력, 마나, 공격력, 방어력, 호감도)

	void ElementEffect() // 상성관계 로직 함수
	{

	}

private:

	int enemytype = 0; //적 캐릭터의 타입 (불, 물, 풀)

};

class WaterEnemy : public Enemy, Player // 물 속성의 적 캐릭터 (냉미녀)
{
	WaterEnemy(string name, int hp, int mp, int attack, int defense, int like); //물 속성의 적 캐릭터의 정보 설정 (체력, 마나, 공격력, 방어력, 호감도)

	void ElementEffect() // 상성관계 로직 함수
	{

	}

private :

	int enemytype = 1; //적 캐릭터의 타입 (불, 물, 풀)

};

class FlowerEnemy : public Enemy, Player // 풀 속성의 적 캐릭터 (초식녀)
{
	FlowerEnemy(string name, int hp, int mp, int attack, int defense, int like); //풀 속성의 적 캐릭터의 정보 설정 (체력, 마나, 공격력, 방어력, 호감도)
	
	void ElementEffect() // 상성관계 로직 함수
	{

	}
	
private:

	int enemytype = 2; //적 캐릭터의 타입 (불, 물, 풀)

};

class NormalEnemy : public Enemy, Player //일반 속성의 적 캐릭터 (일반녀)
{
	NormalEnemy(string name, int hp, int mp, int attack, int defense, int like);
	
	void ElementEffect() // 상성관계 로직 함수
	{
		
	}

private:

	int enemytype = 3; //적 캐릭터의 타입 (불, 물, 풀)

};