#include "stdafx.h"
#include "CigaretteShotter.h"

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainScene::init()
{
	if (!Layer::init())
		return false;

	_winSize = Director::getInstance()->getWinSize();

	_bg = Sprite::create("BG/Main-BG.png");
	_bg->setPosition(_winSize / 2);
	this->addChild(_bg, 1);

	_start = Sprite::create("Button/Start.png");
	_start->setPosition(_winSize.width / 2, _winSize.height * 0.9);
	this->addChild(_start, 2);

	_option = Sprite::create("Button/Option.png");
	_option->setPosition(_winSize.width / 2, _winSize.height * 0.1);
	this->addChild(_option, 3);

	return true;
}

void MainScene::onEnter()
{
	Layer::onEnter();
	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	_listener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
	_listener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
	_listener->onTouchCancelled = CC_CALLBACK_2(MainScene::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);

	auto Button = EventListenerKeyboard::create();
	Button->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Button, this);
}

void MainScene::onExit()
{
	
	Layer::onExit();
}

bool MainScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	_character = Sprite::create("character.png");
	_character->setPosition(Vec2(600, 600));
	this->addChild(_character, 4);

	auto action = ScaleTo::create(0.5, 1.2);
	auto action2 = ScaleTo::create(0.5, 1.0);
	auto action3 = ScaleTo::create(0.5, 1.2);

	if (_start->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_start->runAction(action3);
		_character->setPosition(_start->getPositionX() + 150, _start->getPositionY());
		_character->setTag(1);
		_character->runAction(RepeatForever::create(Sequence::create(action, action2, NULL)));
	}

	if (_option->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_option->runAction(action3);
		_character->setPosition(_option->getPositionX() + 150, _option->getPositionY());
		_character->setTag(2);
		_character->runAction(RepeatForever::create(Sequence::create(action, action2, NULL)));
	}

	return true;
}

void MainScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	auto action = ScaleTo::create(0.3, 1.0f);
	auto action2 = ScaleTo::create(0.3, 1.0f);
	if (!_start->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_start->runAction(action2);
		this->removeChild(getChildByTag(1));
	}

	if (!_option->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_option->runAction(action);
		this->removeChild(getChildByTag(2));
	}
}

void MainScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	TransitionCrossFade* scene;
	if (_start->getBoundingBox().containsPoint(touch->getLocation()))
	{
		if (!Singleton::getInstance()->_muteState)
		{
			SimpleAudioEngine::getInstance()->playEffect("Sound/Button.mp3");
		}
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		scene = TransitionCrossFade::create(1.0f, CigaretteShooter::createScene());
		Director::getInstance()->replaceScene(scene);
	}

	if (_option->getBoundingBox().containsPoint(touch->getLocation()))
	{
		if (!Singleton::getInstance()->_muteState)
		{
			SimpleAudioEngine::getInstance()->playEffect("Sound/Button.mp3");
		}
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		scene = TransitionCrossFade::create(1.0f, OptionScene::createScene());
		Director::getInstance()->replaceScene(scene);
	}
}

void MainScene::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	//BACK
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		Director::getInstance()->end();
	}
}
