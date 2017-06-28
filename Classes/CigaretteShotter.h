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
	// ��ġ ������
	bool onTouchBegan(Touch *touch, Event *unused_event) override;
	void onTouchMoved(Touch *touch, Event *unused_event) override;
	void onTouchEnded(Touch *touch, Event *unused_event) override;
	// ���̷� ����
	void onAcceleration(Acceleration* acc, Event* unused_event) override;

private:
	// ĳ����
	// �Ѿ��� ���� ����
	// ��ġ ����
	bool touch = false;
	bool longTouch = false;

	// ������ �����Ҷ�(�ΰ� ������)
	bool GameStart = false;

	Vec2 begineTouch;
	Vec2 endTouch;
	// ������� ����
	Vec2 playerVelocity;

	void Collide();
	void CharacterChange(int num);
	int _state = -1;
	//-----------------

	void EnemyCreate();
	void EnemyUpdate();
	void EnemyBulletUpdate();

	// �÷��̾�
	Player* player;
	// �÷��̾� ����
	Sprite* player_state;
	bool state;
	// �÷��̾� �Ѿ�
	Vector<Sprite*> player_bullet;
	// ��
	Vector<Enemy*> enemy;
	// �� �Ѿ�
	Vector<Sprite*> enemy_bullet;

	//----------------------------
	// �ǰ�����
	void CheckUP();
	// �ʻ�� ������
	Sprite* skillBar;

	// ���ھ� �ð�
	float scoreTime = 0.0f;
	bool ifOverhigh;
	// ū ���ھ�
	float fhightScore;
	// ���� ���ھ�
	float fcurScore;
	Label* score;
	Label* highscore;
	
	Label* curscore;
	Label* curhighscore;

	void scoreUpdate(float dt);
};