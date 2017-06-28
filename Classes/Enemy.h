#pragma once

#include "stdafx.h"

class Enemy : public Node
{
private:
	Sprite* _enemy;
	int		_health;
	int		_damage;
protected:
public:
	void initEnemy(const char* path,Vec2 position)
	{
		_enemy = Sprite::create(path);
		_enemy->setPosition(position);
		this->addChild(_enemy);

		_health = 100;
		_damage = 100;
	}

	Sprite* getEnemy()
	{
		return _enemy;
	}

	int getHealth()
	{
		return _health;
	}

	void setHealth(int health)
	{
		_health = health;
	}
	int getDamage()
	{
		return _damage;
	}
};