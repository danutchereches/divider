#pragma once
#include "cocos2d.h"
#include "Helpers.h"

class Level
{
public:
	static int getProgress();
	
	Level(int id, int d, int s, int nrd, int nrid);
	
	inline int getId() { return mId; }
	inline int getDivisor() { return mDivisor; }
	inline int getNrDivisible() { return mNrDivisible; }
	inline int getNrIndivisible() { return mNrIndivisible; }
	inline int getSpeed() { return mSpeed; }
	const int getScore();
	void setScore(const int score, bool save = true);
private:
	int mId;
	int mDivisor;
	int mSpeed;
	int mNrDivisible;
	int mNrIndivisible;
	int mScore;
};
