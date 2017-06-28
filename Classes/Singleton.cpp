#include "Singleton.h"

Singleton::Singleton()
{
	
}

Singleton::~Singleton()
{

}
int Singleton::init()
{
	_muteState = false;
	CigaretteShooter_curSocre = 0.0f;
	CigaretteShooter_HighScore = UserDefault::getInstance()->getFloatForKey("HIGH_SCORE");
	return 0;
}

void Singleton::setcurScore(float score)
{
	CigaretteShooter_curSocre = score;
}

Singleton* Singleton::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Singleton();
		instance->init();
	}
	return instance;
}


Singleton* Singleton::instance = nullptr;