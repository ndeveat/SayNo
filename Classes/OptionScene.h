#pragma once

#include "stdafx.h"

class OptionScene : public Layer
{
private:
	Sprite* _bg;
	Sprite* _credit;
	Sprite* _home;
	Sprite* _mute;
	Sprite* _character;

	Size _winSize;

	EventListenerTouchOneByOne* _listener;

	int _count = 0;

	bool _muteState;
	bool _canTouch;

private:
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

	virtual void onEnter();
	virtual void onExit();

	void TextureChagne();

public:

public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(OptionScene);
};