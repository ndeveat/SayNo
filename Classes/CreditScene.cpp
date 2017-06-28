#include "stdafx.h"

Scene* CreditScene::createScene()
{
	auto scene = Scene::create();
	auto layer = CreditScene::create();
	scene->addChild(layer);
	return scene;
}

bool CreditScene::init()
{
	if (!Layer::init())
		return false;

	_winSize = Director::getInstance()->getWinSize();

	_bg = Sprite::create("BG/Credit-BG.png");
	_bg->setPosition(_winSize / 2);
	this->addChild(_bg, 1);

	_home = Sprite::create("Button/Home.png");
	_home->setPosition(_winSize.width / 2, _winSize.height * 0.2);
	this->addChild(_home, 3);

	return true;
}

void CreditScene::onEnter()
{
	Layer::onEnter();
	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = CC_CALLBACK_2(CreditScene::onTouchBegan, this);
	_listener->onTouchMoved = CC_CALLBACK_2(CreditScene::onTouchMoved, this);
	_listener->onTouchEnded = CC_CALLBACK_2(CreditScene::onTouchEnded, this);
	_listener->onTouchCancelled = CC_CALLBACK_2(CreditScene::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);

	auto Button = EventListenerKeyboard::create();
	Button->onKeyReleased = CC_CALLBACK_2(CreditScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Button, this);
}

void CreditScene::onExit()
{

	Layer::onExit();
}

bool CreditScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	_character = Sprite::create("character.png");
	auto action = ScaleTo::create(0.5, 1.2);
	auto action2 = ScaleTo::create(0.5, 1.0);
	auto action3 = ScaleTo::create(0.5, 1.2);

	if (_home->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_home->runAction(action3);
		_character->setPosition(_home->getPositionX() + 150, _home->getPositionY());
		_character->setTag(1);
		_character->runAction(RepeatForever::create(Sequence::create(action, action2, NULL)));
		this->addChild(_character, 4);
	}

	return true;
}

void CreditScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	auto action = ScaleTo::create(0.3, 1.0f);
	if (!_home->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_home->runAction(action);
		this->removeChild(getChildByTag(1));
	}
}

void CreditScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto action = ScaleTo::create(1.0f, 1.2f);
	auto action2 = RotateTo::create(0.3f, 15);
	auto action3 = RotateTo::create(0.3f, -15);
	TransitionCrossFade* scene;

	if (_home->getBoundingBox().containsPoint(touch->getLocation()))
	{
		if (!Singleton::getInstance()->_muteState)
		{
			SimpleAudioEngine::getInstance()->playEffect("Sound/Button.mp3");
		}
		_home->runAction(action);
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		scene = TransitionCrossFade::create(1.0f, MainScene::createScene());
		Director::getInstance()->replaceScene(scene);
	}
}

void CreditScene::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void CreditScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	//BACK
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		auto scene = TransitionCrossFade::create(1.0f, MainScene::createScene());
		Director::getInstance()->replaceScene(scene);
	}
}