#pragma once

#include "stdafx.h"

class Logo : public LayerColor
{
public:
	static Scene* createScene();
	virtual bool init();
	void teamLogo(Ref* sender);
	void changeScene(Ref* sender);
	CREATE_FUNC(Logo);
};
