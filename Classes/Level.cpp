#include "Level.h"

int Level::getProgress()
{
	cocos2d::UserDefault* ud = cocos2d::UserDefault::getInstance();
	
	int l;
	for (l = 1; l < 99; l++)
	{
		if (ud->getIntegerForKey(helpers::String::format("score_%d", l).c_str(), 0) <= 0)
			return l;
	}
	
	return l;
}

Level::Level(int d, int s, int nrd, int nrid)
{
	mNumber = 0;
	mDivisor = d;
	mSpeed = s;
	mNrDivisible = nrd;
	mNrIndivisible = nrid;
}
