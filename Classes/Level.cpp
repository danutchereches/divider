#include "Level.h"
#include "LevelSelectScene.h"

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

int Level::getGlobalStars()
{
	int n = 0;
	
	for (int i = 0; i < LevelSelectScene::LEVEL_NR; i++)
		n += LevelSelectScene::LEVELS[i].getStars();
	
	return n;
}

Level::Level(int id, int d, int s, int nrd, int nrid)
{
	mId = id;
	mDivisor = d;
	mSpeed = s;
	mNrDivisible = nrd;
	mNrIndivisible = nrid;
	mScore = -1;
}

const int Level::getScore()
{
	if (mScore == -1)
		mScore = cocos2d::UserDefault::getInstance()->getIntegerForKey(helpers::String::format("score_%d", mId).c_str(), 0);
	
	return mScore;
}

void Level::setScore(const int score, bool save/* = true */)
{
	if (score <= getScore())
		return;
	
	if (save)
		cocos2d::UserDefault::getInstance()->setIntegerForKey(helpers::String::format("score_%d", mId).c_str(), score);
	
	mScore = score;
}

int Level::getStars()
{
	if (getScore() >= getNrDivisible())
		return 3;
	
	if (getScore() >= getNrDivisible() * 0.75f)
		return 2;
	
	if (getScore() >= getNrDivisible() * 0.5f)
		return 1;
	
	return 0;
}
