#pragma once

#include "cocos2d.h"
#include "Objects.h"

class Ball : public cocos2d::Sprite
{
public:
	
	virtual bool init() override;
	
	inline int getNumber() { return mNumber; };
	void setNumber(int nr);
	
	CREATE_FUNC(Ball);
	
protected:
	int mNumber;
	cocos2d::Label* mLabel;
	
private:
	
};

class BallPool : public GenericPool<Ball*>
{
public:
	void init(int capacity, cocos2d::Node* parent);
protected:
	cocos2d::Node* mParent;
	
	Ball* onAllocatePoolItem() override;
	void onRecycleItem(Ball* item) override;
};

class BallAction : public cocos2d::Action
{
public:
	static BallAction* create(const std::function<void()> &func);
	
	virtual BallAction* clone() const override;
	virtual BallAction* reverse(void) const  override;
	virtual void startWithTarget(cocos2d::Node *target) override;
	virtual void step(float dt) override;
	virtual bool isDone() const override;
	
CC_CONSTRUCTOR_ACCESS:
	BallAction() { mIsDone = false; }
	virtual ~BallAction() {}
	
	bool initWithDuration(float duration);
	
	static const float SCALE_FROM;
	static const float SCALE_TO;
	static const float TOTAL_TIME;
	static const float SHAKE_TIME;
	static const float SCALE_INDEX;
	static const float SHAKE_INDEX;

protected:
	cocos2d::Size mParentSize;
	cocos2d::Size mMaxSize;
	cocos2d::Vec2 mMainPos;
	bool mIsDone;
	std::function<void()> mCallback;
private:
	CC_DISALLOW_COPY_AND_ASSIGN(BallAction);
};
