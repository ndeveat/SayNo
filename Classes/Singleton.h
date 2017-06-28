#pragma once
#include "stdafx.h"

class Singleton
{
private:
	static Singleton* instance;
public:
	float CigaretteShooter_curSocre;
	float CigaretteShooter_HighScore;

	bool _muteState;

	Singleton();
	~Singleton();

	static Singleton* getInstance();

	int init();

	void setcurScore(float score);
};