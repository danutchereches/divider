#pragma once

#include "cocos2d.h"
#include "Objects.h"

class Ball : public cocos2d::Sprite
{
public:
	
	virtual bool init() override;
	
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
