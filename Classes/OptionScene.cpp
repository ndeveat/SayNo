#include "stdafx.h"

Scene* OptionScene::createScene()
{
	auto scene = Scene::create();
	auto layer = OptionScene::create();
	scene->addChild(layer);
	return scene;
}

bool OptionScene::init()
{
	if (!Layer::init())
		return false;

	_winSize = Director::getInstance()->getWinSize();

	_bg = Sprite::create("BG/Option-BG.png");
	_bg->setPosition(_winSize / 2);
	this->addChild(_bg, 1);

	_credit = Sprite::create("Button/Credit.png");
	_credit->setPosition(_winSize.width / 2, _winSize.height * 0.8);
	this->addChild(_credit, 2);

	_home = Sprite::create("Button/Home.png");
	_home->setPosition(_winSize.width / 2, _winSize.height * 0.3);
	this->addChild(_home, 2);

	log("bool : %d", Singleton::getInstance()->_muteState);

	if (!Singleton::getInstance()->_muteState)
	{
		_mute = Sprite::create("Button/SoundOn.png");
	}
	else
	{
		_mute = Sprite::create("Button/SoundOff.png");
	}
	_mute->setPosition(_winSize.width * 0.48, _winSize.height * 0.56);
	this->addChild(_mute, 3);

	_count = 0;
	_muteState = false;
	_canTouch = true;

	return true;
}

void OptionScene::onEnter()
{
	Layer::onEnter();
	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = CC_CALLBACK_2(OptionScene::onTouchBegan, this);
	_listener->onTouchMoved = CC_CALLBACK_2(OptionScene::onTouchMoved, this);
	_listener->onTouchEnded = CC_CALLBACK_2(OptionScene::onTouchEnded, this);
	_listener->onTouchCancelled = CC_CALLBACK_2(OptionScene::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void OptionScene::onExit()
{

	Layer::onExit();
}

bool OptionScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	_character = Sprite::create("character.png");
	_character->setPosition(Vec2(600, 600));
	auto action = ScaleTo::create(0.5, 1.2);
	auto action2 = ScaleTo::create(0.5, 1.0);
	auto action3 = ScaleTo::create(0.5, 1.2);

	if (_credit->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_credit->runAction(action3);
		_character->setPosition(_credit->getPositionX() + 150, _credit->getPositionY());
		_character->setTag(1);
		_character->runAction(RepeatForever::create(Sequence::create(action, action2, NULL)));
		this->addChild(_character, 4);
	}

	if (_home->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_home->runAction(action3);
		_character->setPosition(_home->getPositionX() + 150, _home->getPositionY());
		_character->setTag(2);
		_character->runAction(RepeatForever::create(Sequence::create(action, action2, NULL)));
		this->addChild(_character, 4);
	}

	return true;
}

void OptionScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	auto action = ScaleTo::create(0.3, 1.0f);
	auto action2 = ScaleTo::create(0.3, 1.0f);
	if (!_credit->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_credit->runAction(action2);
		this->removeChild(getChildByTag(1));
	}

	if (!_home->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_home->runAction(action);
		this->removeChild(getChildByTag(2));
	}
}

void OptionScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto action = ScaleTo::create(1.0f, 1.2f);
	//auto action2 = RotateTo::create(0.1f, 15);
	//auto action3 = RotateTo::create(0.1f, -15);
	auto action4 = CallFunc::create(CC_CALLBACK_0(OptionScene::TextureChagne, this));
	TransitionCrossFade* scene;

	if (_credit->getBoundingBox().containsPoint(touch->getLocation()))
	{
		if (!Singleton::getInstance()->_muteState)
		{
			SimpleAudioEngine::getInstance()->playEffect("Sound/Button.mp3");
		}
		_credit->runAction(action);
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		scene = TransitionCrossFade::create(1.0f, CreditScene::createScene());
		Director::getInstance()->replaceScene(scene);
	}

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

	if (_mute->getBoundingBox().containsPoint(touch->getLocation()) && _canTouch)
	{
		//if (!_muteState) // 뮤트
		if (!Singleton::getInstance()->_muteState)
		{
			Singleton::getInstance()->_muteState = true;
			log("bool : %d", Singleton::getInstance()->_muteState);
			_muteState = true;
			_canTouch = false;
			_count = 0;
			_mute->runAction(RepeatForever::create(Sequence::create (action4, NULL)));
			// 알아서 대충봐가면서 음소거 넣으셈
		}
		else // 언 뮤트
		{
			Singleton::getInstance()->_muteState = false;
			SimpleAudioEngine::getInstance()->playEffect("Sound/Button.mp3");
			_muteState = false;
			_canTouch = false;
			_count = 6;
			_mute->runAction(RepeatForever::create(Sequence::create( action4, NULL)));
			// 알아서 대충봐가면서 음소거 넣으셈
		}
	}
}

void OptionScene::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void OptionScene::TextureChagne()
{
	auto action = RotateTo::create(0.3f, 0);
	if (_count == 0)
	{
		_mute->setTexture("Button/SoundOn2.png");
		_count++;
		return;
	}
	if (_count == 1)
	{
		_mute->setTexture("Button/SoundOn3.png");
		_count++;
		return;
	}
	if (_count == 2)
	{
		_mute->setTexture("Button/SoundOff.png");
		_mute->stopAllActions();
		_mute->runAction(action);
		_canTouch = true;
	}
	
	if (_count == 6)
	{
		_mute->setTexture("Button/SoundOn3.png");
		_count--;
		return;
	}

	if (_count == 5)
	{
		_mute->setTexture("Button/SoundOn2.png");
		_count--;
		return;
	}

	if (_count == 4)
	{
		_mute->setTexture("Button/SoundOn.png");
		_mute->stopAllActions();
		_mute->runAction(action);
		_canTouch = true;
	}

}