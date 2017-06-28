#include "GameOverScene.h"
#include "Singleton.h"

Scene* GameOverScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
		return false;

	_winSize = Director::getInstance()->getWinSize();

	if (!Singleton::getInstance()->_muteState)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/GameOver.mp3", true);
	}

	_bg = Sprite::create("BG/GameOver-BG.jpg");
	_bg->setPosition(_winSize / 2);
	this->addChild(_bg, 1);

	_home = Sprite::create("Button/Home.png");
	_home->setPosition(_winSize.width / 2, _winSize.height * 0.3);
	this->addChild(_home, 2);

	srand(time(NULL));

	int num = rand() % 2 + 1;

	yourLung = Sprite::create("Anime/1_1.png");
	yourLung->setPosition(_winSize.width / 2, _winSize.height * 0.45f);
	this->addChild(yourLung,5);

	char temp[255];
	Label* curScore = Label::create("0", "Ariel", 30);
	curScore->setColor(Color3B::BLACK);
	curScore->setPosition(Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.89f));
	sprintf(temp, "%.f", UserDefault::getInstance()->getFloatForKey("HIGH_SCORE"));
	curScore->setString(temp);
	this->addChild(curScore,9);

	Label* highscore = Label::create("0", "Ariel", 30);
	highscore->setColor(Color3B::BLACK);
	highscore->setPosition(Vec2(this->getContentSize().width * 0.5, this->getContentSize().height * 0.75f));
	sprintf(temp, "%.f", Singleton::getInstance()->CigaretteShooter_curSocre);
	highscore->setString(temp);
	highscore->setString(temp);
	this->addChild(highscore,9);

	//-----------------------------------------------
	// 애니메이션 선택
	// 보기쉽게 나눔
	Animation* animation = Animation::create();
	animation->setDelayPerUnit(0.25f);
	switch (num)
	{
	case 1:
		for (int i = 1; i <= 5; i++)
		{
			char temp[255];
			sprintf(temp, "Anime/1_%d.png", i);
			animation->addSpriteFrameWithFileName(temp);
		}
		break;
	case 2:
		for (int i = 1; i <= 8; i++)
		{
			char temp[255];
			sprintf(temp, "Anime/2_%d.png", i);
			animation->addSpriteFrameWithFileName(temp);
		}
		break;
	}
	yourLung->runAction(RepeatForever::create(Animate::create(animation)));

	schedule(schedule_selector(GameOverScene::update));

	return true;
}

void GameOverScene::onEnter()
{
	Layer::onEnter();
	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = CC_CALLBACK_2(GameOverScene::onTouchBegan, this);
	_listener->onTouchMoved = CC_CALLBACK_2(GameOverScene::onTouchMoved, this);
	_listener->onTouchEnded = CC_CALLBACK_2(GameOverScene::onTouchEnded, this);
	_listener->onTouchCancelled = CC_CALLBACK_2(GameOverScene::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);

	auto Button = EventListenerKeyboard::create();
	Button->onKeyReleased = CC_CALLBACK_2(GameOverScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Button, this);
}

void GameOverScene::onExit()
{

	Layer::onExit();
}

bool GameOverScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	_character = Sprite::create("character.png");
	_character->setPosition(Vec2(600, 600));
	this->addChild(_character, 4);

	auto action = ScaleTo::create(0.5, 1.2);
	auto action2 = ScaleTo::create(0.5, 1.0);
	auto action3 = ScaleTo::create(0.5, 1.2);

	if (_home->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_home->runAction(action3);
		_character->setPosition(_home->getPositionX() + 150, _home->getPositionY());
		_character->setTag(1);
		_character->runAction(RepeatForever::create(Sequence::create(action, action2, NULL)));
		//this->addChild(_character, 4);
	}

	return true;
}

void GameOverScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	auto action = ScaleTo::create(0.3, 1.0f);
	if (!_home->getBoundingBox().containsPoint(touch->getLocation()))
	{
		this->removeChild(getChildByTag(1));
	}
}

void GameOverScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto action = ScaleTo::create(1.0f, 1.2f);
	TransitionCrossFade* scene;
	if (_home->getBoundingBox().containsPoint(touch->getLocation()))
	{
		if (!Singleton::getInstance()->_muteState)
		{
			SimpleAudioEngine::getInstance()->playEffect("Sound/Button.mp3");
		}
		if (!Singleton::getInstance()->_muteState)
		{
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		}
		_home->runAction(action);
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		scene = TransitionCrossFade::create(1.0f, MainScene::createScene());
		Director::getInstance()->replaceScene(scene);
	}
}

void GameOverScene::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void GameOverScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	//BACK
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		auto scene = TransitionCrossFade::create(1.0f, MainScene::createScene());
		Director::getInstance()->replaceScene(scene);
	}
}
void GameOverScene::update(float dt)
{

}