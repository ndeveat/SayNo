#pragma once

#include "stdafx.h"

class Player : public Node
{
private:
	Sprite* _player;
	int		_damage;
protected:
public:
	Player()
	{

	};
	~Player()
	{

	};
	Player(Vec2 position)
	{
		initPlayer(position);
	}
	void initPlayer(Vec2 position)
	{
		_player = Sprite::create("character.png");
		_player->setPosition(position);
		this->addChild(_player);

		_damage = 25;
	}

	Sprite* getPlayer()
	{
		return _player;
	}

	int getDamage()
	{
		return _damage;
	}
};