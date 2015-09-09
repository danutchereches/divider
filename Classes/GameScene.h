#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "Helpers.h"
#include "Loader.h"
#include "Ball.h"
#include "Objects.h"
#include "AppDelegate.h"

class GameScene : public cocos2d::Scene
{
public:
	virtual bool init() override;
	
	static int NUMBER_POOL_SIZE;
	static int NUMBER_POOL[];
	static int DIVISORS[];
	
	virtual GameScene* clone() const = 0;
protected:
	cocos2d::Size mScreenSize;
	cocos2d::Size mVisibleSize;
	cocos2d::Vec2 mOrigin;
	
	cocos2d::Layer* mGameLayer;
	cocos2d::Layer* mUILayer;
	
	cocos2d::Label* mScoreView;
	
	BallPool mBallPool;
	cocos2d::Vector<Ball*> mBalls;
	int mBallZOrder;
	int mCurrentDivisor;
	
	int mDivisorMin;
	int mDivisorMax;
	float mSpawnTimer;
	float mSpawnInterval;
	
	int mScore;
	
	bool mIsGameServicesAvailable;
	
	void checkNumbers();
	void initPools();
	
	virtual void update(float dt);
	virtual void updateSlow(float dt);
	
	virtual void updateDivisor(int d) = 0;
	virtual void updateScore();
	virtual void spawnBall() = 0;
	virtual void divideBall(Ball* ball) = 0;
	virtual void missBall(Ball* ball) = 0;
	
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onComeToForeground();
	void onComeToBackground();
};

class GameMode1Scene : public GameScene
{
public:
	bool init() override;
	
	GameScene* clone() const override;
	
	CREATE_FUNC(GameMode1Scene);
protected:
	void updateDivisor(int d) override;
	void spawnBall() override;
	void divideBall(Ball* ball) override;
	void missBall(Ball* ball) override;
	
	void ballPopCallback(Ball* ball);
};

class GameMode2Scene : public GameScene
{
public:
	bool init() override;
	
	GameScene* clone() const override;
	
	CREATE_FUNC(GameMode2Scene);
protected:
	
	float mBallSpeed; // pixels per second
	
	int mWaveNumber;
	float mWaveTimer;
	float mWaveLength;
	
	int mPreviousDivisor;
	int mNextDivisor;
	
	cocos2d::Label* mPreviousDivisorLabel;
	cocos2d::Label* mCurrentDivisorLabel;
	cocos2d::Label* mNextDivisorLabel;
	
	void update(float dt) override;
	void updateSlow(float dt) override;
	
	void updateDivisor(int d) override;
	void spawnBall() override;
	void divideBall(Ball* ball) override;
	void missBall(Ball* ball) override;
};

#endif // __GAMESCENE_H__
