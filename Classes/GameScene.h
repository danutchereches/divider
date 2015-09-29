#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "Helpers.h"
#include "Loader.h"
#include "Ball.h"
#include "Objects.h"
#include "Level.h"
#include "GameOverlay.h"
#include "AppDelegate.h"

enum class GAME_SCENE_STATES {
	NONE,
	START,
	PLAY,
	PAUSED,
	OVER
};

class GameScene : public cocos2d::Scene
{
public:
	virtual bool init() override;
	
	static int NUMBER_POOL_SIZE;
	static int NUMBER_POOL[];
	static int DIVISORS[];
	static const std::string ANALYTICS_GAME_MODE_INDEX;
	static const std::string ANALYTICS_LEVEL_INDEX;
	static const std::string ANALYTICS_WAVE_INDEX;
	static const std::string ANALYTICS_SCORE_INDEX;
	
	virtual GameScene* clone() const = 0;
	
	virtual void restartGame();
	virtual void startGame();
	virtual void resumeGame();
	virtual void pauseGame();
	virtual void endGame(int nr);
	virtual void exitGame();
protected:
	cocos2d::Size mScreenSize;
	cocos2d::Size mVisibleSize;
	cocos2d::Vec2 mOrigin;
	
	cocos2d::Node* mGameLayer;
	cocos2d::Layer* mUILayer;
	
	cocos2d::LayerColor* mTopBar;
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
	
	GAME_SCENE_STATES mSceneState;
	
	bool mIsGameServicesAvailable;
	
	void checkNumbers();
	virtual int getNumber();
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
	
	virtual void endGame(int nr) override;
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
	
protected:
	
	float mBallSpeed; // pixels per second
	
	cocos2d::Label* mCurrentDivisorLabel;
	
	virtual void update(float dt) override;
	virtual void updateSlow(float dt) override;
	
	virtual void updateDivisor(int d) override;
	virtual void spawnBall() override;
	virtual void divideBall(Ball* ball) override;
	virtual void missBall(Ball* ball) override;
	virtual void missBall(Ball* ball, bool manual) = 0;
};

class GameMode2InfiniteScene : public GameMode2Scene
{
public:
	bool init() override;
	
	GameScene* clone() const override;
	
	CREATE_FUNC(GameMode2InfiniteScene);
	
	virtual void endGame(int nr) override;
protected:
	
	int mWaveNumber;
	float mWaveTimer;
	float mWaveLength;
	
	int mPreviousDivisor;
	int mNextDivisor;
	
	cocos2d::Label* mPreviousDivisorLabel;
	cocos2d::Label* mNextDivisorLabel;
	
	void update(float dt) override;
	void updateSlow(float dt) override;
	
	void startWave();
	void endWave();
	void setDivisorRange();
	
	void updateDivisor(int d) override;
	virtual void divideBall(Ball* ball) override;
	virtual void missBall(Ball* ball, bool manual) override;
};

class GameMode2LevelScene : public GameMode2Scene
{
public:
	virtual bool initWithLevelNumber(Level* level);
	
	GameScene* clone() const override;
	
	static GameMode2LevelScene* create(Level* level);
	
	virtual void endGame(int nr) override;
protected:
	int* mNumbers;
	int mNumbersSize;
	int mNumbersIndex;
	
	Level* mLevel;
	float mLevelTimer;
	
	cocos2d::Label* mTimerLabel;
	
	virtual int getNumber() override;
	
	virtual void update(float dt) override;
	virtual void updateSlow(float dt) override;
	
	virtual void divideBall(Ball* ball) override;
	virtual void missBall(Ball* ball, bool manual) override;
};

#endif // __GAMESCENE_H__
