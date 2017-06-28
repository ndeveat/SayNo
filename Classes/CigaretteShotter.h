#pragma once

#include "GameOverScene.h"
#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"

#define DEFAULT_DECELERATION    (0.4f)
#define DEFAULT_SENSITIVITY     (9.8f)
#define MAX_VELOCITY            (100.0f)

class CigaretteShooter : public Layer
{
private:
public:
    static Scene* createScene();
    bool init() override;
	void onEnter() override;
	void update(float dt);
	void PlayerUpdate(float dt);
	void EnemyUpdateFunc(float dt);
	CREATE_FUNC(CigaretteShooter);
	

	void ChageScene();

private:
	// 터치 리스너
	bool onTouchBegan(Touch *touch, Event *unused_event) override;
	void onTouchMoved(Touch *touch, Event *unused_event) override;
	void onTouchEnded(Touch *touch, Event *unused_event) override;
	// 자이로 센서
	void onAcceleration(Acceleration* acc, Event* unused_event) override;

private:
	// 캐릭터
	// 총알을 담을 백터
	// 터치 유무
	bool touch = false;
	bool longTouch = false;

	// 게임이 시작할때(로고가 끝나고)
	bool GameStart = false;

	Vec2 begineTouch;
	Vec2 endTouch;
	// 사용자의 기울기
	Vec2 playerVelocity;

	void Collide();
	void CharacterChange(int num);
	int _state = -1;
	//-----------------

	void EnemyCreate();
	void EnemyUpdate();
	void EnemyBulletUpdate();

	// 플레이어
	Player* player;
	// 플레이어 상태
	Sprite* player_state;
	bool state;
	// 플레이어 총알
	Vector<Sprite*> player_bullet;
	// 적
	Vector<Enemy*> enemy;
	// 적 총알
	Vector<Sprite*> enemy_bullet;

	//----------------------------
	// 건강검진
	void CheckUP();
	// 필살기 게이지
	Sprite* skillBar;

	// 스코어 시간
	float scoreTime = 0.0f;
	bool ifOverhigh;
	// 큰 스코어
	float fhightScore;
	// 현재 스코어
	float fcurScore;
	Label* score;
	Label* highscore;
	
	Label* curscore;
	Label* curhighscore;

	void scoreUpdate(float dt);
};