#include "stdafx.h"
#include "Logo.h"
Scene* Logo::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vect(0, -500));
	auto layer = Logo::create();

	scene->addChild(layer);

	return scene;
}

bool Logo::init()
{
	if (!LayerColor::initWithColor(Color4B::BLACK))
		return false;

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/Start.mp3");

	Sprite* mi = Sprite::create("LOGO/MI.png");
	mi->setOpacity(0);
	mi->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	mi->runAction(FadeIn::create(1.0f));
	this->addChild(mi);

	Sprite* mi2 = Sprite::create("LOGO/MI2.png");
	mi2->setOpacity(0);
	mi2->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2 - 130);
	mi2->runAction(FadeIn::create(0.6f));
	mi2->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, 80)), DelayTime::create(0.5f), FadeOut::create(0.6f), CallFuncN::create(CC_CALLBACK_1(Logo::teamLogo, this)), NULL));
	mi->runAction(Sequence::create(DelayTime::create(1.5f), FadeOut::create(0.6f), NULL));
	this->addChild(mi2, 5);

	return true;
}

void Logo::teamLogo(Ref* sender)
{
	LayerColor* layer = LayerColor::create(Color4B::WHITE);
	//layer->setOpacity(0);
	this->addChild(layer,10);

	Sprite* kgsh = Sprite::create("LOGO/KGSH.jpg");
	kgsh->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2);
	kgsh->setOpacity(0);
	layer->addChild(kgsh, 10);

	//layer->runAction(FadeIn::create(1.0f));
	kgsh->runAction(Sequence::create(FadeIn::create(1.0f), CallFuncN::create(CC_CALLBACK_1(Logo::changeScene, this)), NULL));

	/*LayerColor* layer = LayerColor::create(Color4B(241, 240, 236, 255));
	layer->setOpacity(0);
	this->addChild(layer);

	Sprite* mi = Sprite::create("LOGO/MI.png");
	mi->setOpacity(0);
	mi->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	layer->addChild(mi);
	

	layer->runAction(FadeIn::create(1.0f));
	mi->runAction(Sequence::create(FadeIn::create(2.0f), DelayTime::create(1.5f),CallFuncN::create(CC_CALLBACK_1(Logo::changeScene, this)), NULL));

	Sprite* logo = Sprite::create("LOGO/MI2.png");
	logo->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2 - 130);
	logo->runAction(FadeIn::create(0.6f));
	logo->runAction(MoveBy::create(1.0f, Vec2(0, 80)));
	this->addChild(logo,5);*/
}

void Logo::changeScene(Ref* sender)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0f, MainScene::createScene()));
}