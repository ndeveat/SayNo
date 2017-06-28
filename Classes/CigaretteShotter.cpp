#include "CigaretteShotter.h"
#include "Singleton.h"

Scene* CigaretteShooter::createScene()
{
	auto scene = Scene::create();
	auto layer = CigaretteShooter::create();
	scene->addChild(layer);
	return scene;
}

bool CigaretteShooter::init()
{
	if (!Layer::init())
		return false;
	// 초기화
	playerVelocity = Vec2::ZERO;
	begineTouch = Vec2::ZERO;
	endTouch = Vec2::ZERO;
	state = false;
	ifOverhigh = false;

	// 초기화
	Sprite* GameStartBackground = Sprite::create("BG/Main-BG.png");
	GameStartBackground->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(GameStartBackground, 10);
	GameStartBackground->runAction(Sequence::create(FadeOut::create(4.0f), RemoveSelf::create(), NULL));

	srand(time(NULL));

	// 자이로 센서 활성화
	setAccelerometerEnabled(true);

	// 배경화면
	Sprite* background = Sprite::create("ingame_background.jpg");
	background->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(background);

	// 플레이어 초기화
	player = new Player();
	player->initPlayer(Vec2(D_DESIGN_WIDTH * 0.5, 300));
	player->getPlayer()->setScale(0.6f);
	this->addChild(player);

	// 플레이어 폐 상태
	player_state = Sprite::create("Lung.png");
	player_state->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height * 0.935));
	player_state->setScale(0.7f);
	this->addChild(player_state);

	// 스킬 게이지 배경
	Sprite* skillGage = Sprite::create("GageBig.png");
	skillGage->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(skillGage);

	// 스킬 게이지
	skillBar = Sprite::create("Gage.png");
	skillBar->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height * 0.048f));
	skillBar->setScaleX(0.0f);
	this->addChild(skillBar);

	// 하이스코어를 받아옴
	fhightScore = Singleton::getInstance()->CigaretteShooter_HighScore;
	// 기본 스코어
	fcurScore = 0.0f;

	// 스코어 Label
	score = Label::create("Score", "Ariel", 30);
	score->setPosition(Vec2(this->getContentSize().width * 0.8, this->getContentSize().height * 0.95f));
	this->addChild(score);

	curscore = Label::create("0", "Ariel", 30);
	curscore->setPosition(Vec2(this->getContentSize().width * 0.75, this->getContentSize().height * 0.90f));
	this->addChild(curscore);

	highscore = Label::create("HighScore", "Ariel", 30);
	highscore->setPosition(Vec2(this->getContentSize().width * 0.2, this->getContentSize().height * 0.95f));
	this->addChild(highscore);

	curhighscore = Label::create("0", "Ariel", 30);
	curhighscore->setPosition(Vec2(this->getContentSize().width * 0.15, this->getContentSize().height * 0.90f));
	char highTemp[200];
	sprintf(highTemp, "%.f", fhightScore);
	curhighscore->setString(highTemp);
	this->addChild(curhighscore);


	schedule(schedule_selector(CigaretteShooter::update));
	schedule(schedule_selector(CigaretteShooter::PlayerUpdate));
	schedule(schedule_selector(CigaretteShooter::scoreUpdate));
	schedule(schedule_selector(CigaretteShooter::EnemyUpdateFunc));
	return true;
}

void CigaretteShooter::onEnter()
{
	Layer::onEnter();

	auto Touch = EventListenerTouchOneByOne::create();
	Touch->onTouchBegan = CC_CALLBACK_2(CigaretteShooter::onTouchBegan, this);
	Touch->onTouchMoved = CC_CALLBACK_2(CigaretteShooter::onTouchMoved, this);
	Touch->onTouchEnded = CC_CALLBACK_2(CigaretteShooter::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Touch, this);
}

void CigaretteShooter::update(float dt)
{
	// 게임시작을 하지 않았을때
	static float GameStartTime = 0.0f;
	if (!GameStart)
	{
		GameStartTime += dt;
	}
	if (GameStartTime >= 3.5f)
	{
		GameStartTime = 0;
		GameStart = true;
	}

	if (GameStart)
	{
		static float checkup= 0.0f;
		checkup += dt;

		static float gagebar = 0.0f;
		gagebar += dt;

		//-----------------------------------
		this->skillBar->setScaleX(this->skillBar->getScaleX() + (0.08f * dt));
		/*if (gagebar >= 1.5f)
		{
			gagebar -= gagebar;
			this->skillBar->setScaleX(this->skillBar->getScaleX() + 0.2f);
		}*/

		if (this->skillBar->getScaleX() >= 1.0f)
		{
			this->skillBar->setScaleX(1.0f);
		}

		//-----------------------------------
		Collide();
		//-----------------------------------
		// 건강검진
		if (checkup >= 30.0f)
		{
			checkup -= checkup;
			CheckUP();
		}
	}
}
// 터치 리스너
bool CigaretteShooter::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (GameStart)
	{
		this->touch = true;
		this->longTouch = false;

		begineTouch = touch->getLocation();
	}
	return true;
}

void CigaretteShooter::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (GameStart)
	{
		endTouch = touch->getLocation();
	}
}

void CigaretteShooter::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (GameStart)
	{
		if (endTouch.y - begineTouch.y >= 50)
		{
			this->longTouch = true;
		}
		else
		{
			this->longTouch = false;
		}

		this->touch = false;
	}
}
// 자이로 센서
void CigaretteShooter::onAcceleration(Acceleration* acc, Event* unused_event)
{
	if (GameStart)
	{
		playerVelocity.x = playerVelocity.x * DEFAULT_DECELERATION + acc->x * (DEFAULT_SENSITIVITY);
		playerVelocity.y = playerVelocity.y * DEFAULT_DECELERATION + acc->y * (DEFAULT_SENSITIVITY);

		if (playerVelocity.x > MAX_VELOCITY){
			playerVelocity.x = MAX_VELOCITY;
		}
		else if (playerVelocity.x < -MAX_VELOCITY){
			playerVelocity.x = -MAX_VELOCITY;
		}
	}
}

void CigaretteShooter::EnemyCreate()
{
	if (GameStart)
	{
		int num = rand() % 3 + 1;

		Enemy* enemy = new Enemy();
		//------------------------------------
		// 적 생성
		switch (num)
		{

		case 1:
			enemy->initEnemy("Red.png", Vec2(D_DESIGN_WIDTH*0.2f, D_DESIGN_HEIGHT - 10));
			enemy->setHealth(150);
			break;

		case 2:
			enemy->initEnemy("Blue.png", Vec2(D_DESIGN_WIDTH*0.4f, D_DESIGN_HEIGHT - 10));
			enemy->setHealth(250);
			break;

		case 3:
			enemy->initEnemy("Green.png", Vec2(D_DESIGN_WIDTH*0.6f, D_DESIGN_HEIGHT - 10));
			enemy->setHealth(300);
			break;

		default:
			break;
		}
		//------------------------------------
		// 적의 포지션
		num = rand() % 3 + 1;
		switch (num)
		{

		case 1:
			enemy->getEnemy()->setPosition(Vec2(D_DESIGN_WIDTH*0.2f, D_DESIGN_HEIGHT - 10));
			break;

		case 2:
			enemy->getEnemy()->setPosition(Vec2(D_DESIGN_WIDTH*0.4f, D_DESIGN_HEIGHT - 10));
			break;

		case 3:
			enemy->getEnemy()->setPosition(Vec2(D_DESIGN_WIDTH*0.6f, D_DESIGN_HEIGHT - 10));
			break;

		default:
			break;
		}

		//---------------------------------
		// 적의 스케일
		enemy->getEnemy()->setScale(0.65f);

		this->addChild(enemy, 5);
		this->enemy.pushBack(enemy);
	}
}

void CigaretteShooter::EnemyUpdate()
{
	if (GameStart)
	{
		if (this->enemy.size())
		{
			for (auto it : this->enemy)
			{
				int num = rand() % 450 + 1;
				float ypos = it->getEnemy()->getPositionY();
				ypos -= 1;
				it->getEnemy()->setPosition(it->getEnemy()->getPositionX(), ypos);

				// 담배 총알 생성
				switch (num)
				{
				case 1:
				{
					Sprite* enemy_bullet = Sprite::create("Yellow.png");
					enemy_bullet->setRotation(180);
					enemy_bullet->setPosition(Vec2(it->getEnemy()->getPositionX(), it->getEnemy()->getPositionY()));
					enemy_bullet->setScale(0.35f);
					this->addChild(enemy_bullet, 1);
					this->enemy_bullet.pushBack(enemy_bullet);
				}
				break;
				case 2:{
					Sprite* enemy_bullet = Sprite::create("Yellow.png");
					enemy_bullet->setRotation(180);
					enemy_bullet->setPosition(Vec2(it->getEnemy()->getPositionX(), it->getEnemy()->getPositionY()));
					enemy_bullet->setScale(0.35f);
					this->addChild(enemy_bullet, 1);
					this->enemy_bullet.pushBack(enemy_bullet);
					break;
				}
				default:
					break;
				}
				//---------------------------------
				if (it->getEnemy()->getPositionY() <= 0)
				{
					_state += 1;

					if (_state >= 3)
					{
						_state = 3;
					}

					this->enemy.eraseObject(it);
					this->removeChild(it);

					this->CharacterChange(_state);
					break;
				}

			}
		}
	}
}

void CigaretteShooter::EnemyBulletUpdate()
{
	if (GameStart)
	{
		if (this->enemy_bullet.size())
		{
			for (auto it : this->enemy_bullet)
			{
				float ypos = it->getPositionY();
				ypos -= 8;
				it->setPosition(it->getPositionX(), ypos);
			}
		}
	}
}

void CigaretteShooter::Collide()
{
	if (GameStart)
	{
		bool attack = false;
		//---------------------------
		// 플레이어 총알과 적
		GO:
		for (auto bullet : this->player_bullet){
			Sprite* BULLET = bullet;
			for (auto enemys : this->enemy){
				Enemy* ENEMY = enemys;
				//-------------------------------------------------------------------------------
				if (ENEMY->getEnemy()->getBoundingBox().intersectsRect(BULLET->getBoundingBox()))
				{
					SimpleAudioEngine::getInstance()->playEffect("Sound/Hit.mp3");
					int enemysHealthData;
					if (BULLET->getTag() == 1)
					{
						enemysHealthData = enemys->getHealth() - player->getDamage();
					}
					else
					{
						enemysHealthData = enemys->getHealth() - (player->getDamage() * 2.5);
					}

					enemys->setHealth(enemysHealthData);

					Sprite* No = Sprite::create("Effect.png");
					No->setPosition(BULLET->getPosition());
					No->setScale(0.6f);
					this->addChild(No, 5);

					No->runAction(Sequence::create(FadeOut::create(1.0f), RemoveSelf::create(), NULL));

					// 적의 체력이 0일때
					if (enemys->getHealth() <= 0)
					{

						Sprite* boom = Sprite::create();
						boom->setScale(0.8f);
						boom->setPosition(ENEMY->getEnemy()->getPosition());

						int cnt = rand() % 4 + 1;
						switch (cnt)
						{
						case 1:
							boom->setTexture("Nar.png");
							break;
						case 2:
							boom->setTexture("Nar2.png");
							break;
						case 3:
							boom->setTexture("Tar.png");
							break;
						case 4:
							boom->setTexture("Tar2.png");
							break;
						default:
							break;
						}
						//-----------------------------
						// 적 소멸시 점수
						fcurScore += 1.0f;

						char curTemp[255];
						sprintf(curTemp, "%.f", fcurScore);
						curscore->setString(curTemp);
						if (fhightScore <= fcurScore)
						{
							ifOverhigh = true;
							if (ifOverhigh)
								curhighscore->setString(curTemp);
						}

						///////////////////////////////////////

						this->addChild(boom);
						boom->runAction(MoveBy::create(1.0f, Vec2(0, -10)));
						boom->runAction(FadeOut::create(1.0f));
						boom->runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), NULL));

						this->enemy.eraseObject(ENEMY);
						this->removeChild(ENEMY);
					}

					if (BULLET->getTag() == 1)
					{
						player_bullet.eraseObject(bullet);
						this->removeChild(bullet);
					}
					goto GO;

					break;
				}
			}
		}

		//------------
		//------------

		for (auto EnemyBullet : enemy_bullet)
		{
			Sprite* EBULLET = EnemyBullet;
			if (this->player->getPlayer()->getBoundingBox().intersectsRect(EBULLET->getBoundingBox()))
			{
				SimpleAudioEngine::getInstance()->playEffect("Sound/Hit.mp3");
				Sprite* getAttack = Sprite::create("getAttack.png");
				getAttack->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
				this->addChild(getAttack, 7);
				getAttack->runAction(Sequence::create(FadeOut::create(0.3f), RemoveSelf::create(), NULL));

				_state += 1;

				if (_state >= 3)
				{
					_state = 3;
				}

				// 캐릭터 체인지
				CharacterChange(_state);

				enemy_bullet.eraseObject(EBULLET);
				this->removeChild(EBULLET);
				break;
			}
		}
	}
}

void CigaretteShooter::CharacterChange(int num)
{
	if (GameStart)
	{
		switch (num)
		{
		case 0:
			this->player_state->setTexture("Lung_black.png");
			break;
		case 1:
			this->player_state->setTexture("Lung_black_black.png");
			break;
		case 2:
			this->player_state->setTexture("Lung_black_black_black.png");
			break;
		case 3:
		{
			GameStart = false;
			ChageScene();
			break;
		}
		default:
			break;
		}
	}
}

void CigaretteShooter::CheckUP()
{
	if (GameStart)
	{
		SimpleAudioEngine::getInstance()->playEffect("Sound/Heal.mp3");
		Sprite* chkUp = Sprite::create("checkUP.png");
		chkUp->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
		this->addChild(chkUp);
		chkUp->runAction(Sequence::create(FadeOut::create(0.3f), RemoveSelf::create(), NULL));

		Sprite* chkUp2 = Sprite::create("checkUP_label.png");
		chkUp2->setPosition(this->getContentSize().width / 2, this->getContentSize().height * 0.8);
		chkUp2->setScale(1.5f);
		this->addChild(chkUp2);
		chkUp2->runAction(MoveBy::create(1.0f, Vec2(0, 10)));
		chkUp2->runAction(Sequence::create(FadeOut::create(1.0f), RemoveSelf::create(), NULL));

		_state -= 1;

		// 캐릭터 체인지
		CharacterChange(_state);
	}
}

void CigaretteShooter::PlayerUpdate(float dt)
{
	// 게임을 시작했을떼
	if (GameStart)
	{
		static float bulletTime = 0.0f;
		bulletTime += dt;

		static float skill = 0.0f;
		skill += dt;

		//----------------------------------
		// 플레이어 이동
		Vec2 temp = player->getPlayer()->getPosition();

		temp.x += playerVelocity.x;
		temp.y += playerVelocity.y;

		if (temp.x < 50)
			temp.x = 50;
		if (temp.x > D_DESIGN_WIDTH - 80)
			temp.x = D_DESIGN_WIDTH - 80;
		if (temp.y < 100)
			temp.y = 100;
		if (temp.y > D_DESIGN_HEIGHT - 100)
			temp.y = D_DESIGN_HEIGHT - 100;

		player->getPlayer()->setPosition(temp);
		//-----------------------------------
		// 터치시 총알 생성
		if (touch)
		{
			if (bulletTime >= 0.15) 
			{
				SimpleAudioEngine::getInstance()->playEffect("Sound/Launch.mp3");
				bulletTime -= bulletTime;
				// 총알
				Sprite* bullet = Sprite::create("candy.png");
				bullet->setPosition(player->getPlayer()->getPosition());
				bullet->setScale(0.25f);
				bullet->setTag(1);
				this->addChild(bullet, 1);
				this->player_bullet.pushBack(bullet);
			}
		}

		if (longTouch)
		{
			bulletTime -= bulletTime;
			if (this->skillBar->getScaleX() >= 1.0f)
			{
				SimpleAudioEngine::getInstance()->playEffect("Sound/Goong.mp3");
				Sprite* space = Sprite::create("Space2.png");
				space->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
				this->addChild(space);
				space->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), NULL));

				// 총알
				Sprite* bullet = Sprite::create("skill.png");
				bullet->setPosition(player->getPlayer()->getPosition());
				bullet->setTag(2);
				bullet->setScale(0.6f);
				bullet->runAction(RepeatForever::create(RotateBy::create(1.0f, 40.0f)));

				this->addChild(bullet, 1);
				this->player_bullet.pushBack(bullet);
				this->skillBar->setScaleX(0.0f);

				longTouch = false;
			}
		}

		if (skill >= 5.0f)
		{
			skill -= skill;
		}

		//-----------------------------------
		// 총알이 앞으로 나가기 위한 루프
		if (this->player_bullet.size()){
			for (auto it : this->player_bullet){
				float ypos = it->getPositionY();
				if (it->getTag() == 1)
					ypos += 10;
				else
					ypos += 2;

				it->setPosition(it->getPositionX(), ypos);
			}
		}
	}

}

void CigaretteShooter::EnemyUpdateFunc(float dt)
{
	static float enemyTime = 0.0f;
	enemyTime += dt;
	//-----------------------------------
	// 적생성
	if (enemyTime >= 3.0)
	{
		enemyTime -= enemyTime;
		EnemyCreate();
	}
	EnemyUpdate();
	EnemyBulletUpdate();
}

void CigaretteShooter::ChageScene()
{
	if (ifOverhigh)
	{
		UserDefault::getInstance()->setFloatForKey("HIGH_SCORE", fcurScore);
		UserDefault::getInstance()->flush();
	}
	Singleton::getInstance()->setcurScore(fcurScore);
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	auto scene = TransitionCrossFade::create(1.0f, GameOverScene::createScene());
	Director::getInstance()->replaceScene(scene);
}

void CigaretteShooter::scoreUpdate(float dt)
{
	scoreTime += dt;
	//-----------------------------------
	// 스코어 업데이트
	if (scoreTime >= 4.0f)
	{
		scoreTime -= scoreTime;
		fcurScore += 1.0f;

		char curTemp[255];
		sprintf(curTemp, "%.f", fcurScore);
		curscore->setString(curTemp);
		if (fhightScore <= fcurScore)
		{
			ifOverhigh = true;
			curhighscore->setString(curTemp);
		}
	}
}