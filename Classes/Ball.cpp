#include "Ball.h"

bool Ball::init()
{
	if (!cocos2d::Sprite::initWithSpriteFrameName("ball"))
		return false;
	
	mNumber = 0;
	mLabel = cocos2d::Label::createWithTTF("0", "fonts/default.ttf", 8);
	mLabel->setPosition(getContentSize().width/2, getContentSize().height/2);
	this->addChild(mLabel);
	
	return true;
}

void Ball::setNumber(int nr)
{
	mNumber = nr;
	
	char buf[50];
	sprintf(buf, "%d", mNumber);
	mLabel->setString(std::string(buf));
}

void BallPool::init(int capacity, cocos2d::Node* parent)
{
	if (getAvailableItemCount() > 0)
		clearPool();
	
	mParent = parent;
	
	initWithCapacity(capacity);
}

Ball* BallPool::onAllocatePoolItem()
{
	Ball* item = Ball::create();
	item->setVisible(false);
	item->pause();
	mParent->addChild(item);
	return item;
}

void BallPool::onRecycleItem(Ball* item)
{
	item->setScale(1);
	item->stopAllActions();
}
