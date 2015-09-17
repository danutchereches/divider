#pragma once
#include "cocos2d.h"
#include "Helpers.h"

class Level
{
public:
	static int getProgress();
	
	Level(int d, int s, int nrd, int nrid);
	
	inline int getDivisor() { return mDivisor; }
	inline int getNrDivisible() { return mNrDivisible; }
	inline int getNrIndivisible() { return mNrIndivisible; }
	inline int getSpeed() { return mSpeed; }
private:
	int mNumber;
	int mDivisor;
	int mSpeed;
	int mNrDivisible;
	int mNrIndivisible;
};
