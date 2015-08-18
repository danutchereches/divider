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
	item->setVisible(false);
	item->pause();
	item->setPosition(-50, -50);
}

BallAction* BallAction::create(const std::function<void()> &func)
{
	BallAction *ret = new (std::nothrow) BallAction();
	
	if (ret)
	{
		ret->mCallback = func;
		ret->autorelease();
	}
	
	return ret;
}

BallAction* BallAction::clone() const
{
	return BallAction::create(mCallback);
}

void BallAction::startWithTarget(cocos2d::Node *target)
{
	cocos2d::Action::startWithTarget(target);
	
	cocos2d::Size size = target->getBoundingBox().size;
	
	if (_target->getAnchorPoint() != cocos2d::Vec2::ANCHOR_MIDDLE)
	{
		_target->setPosition(_target->getPosition()
				- cocos2d::Vec2(size.width * _target->getAnchorPoint().x, size.height * _target->getAnchorPoint().y)
				+ cocos2d::Vec2(size.width/2, size.height/2));
		_target->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	}
	
	if (_target->getParent())
		mMaxSize = _target->getParent()->getContentSize();
	else
		mMaxSize = cocos2d::Director::getInstance()->getWinSize();//TODO: maybe use visible size
	mPOffset = cocos2d::Vec2(_target->getPositionX() / (mMaxSize.width - size.width),
			_target->getPositionY() / (mMaxSize.height - size.height));
	mIsDone = false;
}

BallAction* BallAction::reverse() const
{
	return BallAction::create(mCallback);
}

void BallAction::step(float dt)
{
	if (_target)
	{
		_target->setScale(_target->getScale() + dt / 20);
		
		cocos2d::Size size = _target->getBoundingBox().size;
		
		if (_target->getPosition().x < size.width/2)
			_target->setPositionX(size.width/2);
		else if (_target->getPosition().x + size.width/2 > mMaxSize.width)
			_target->setPositionX(mMaxSize.width - size.width/2);
		
		if (_target->getPosition().y < size.height/2)
			_target->setPositionY(size.height/2);
		else if (_target->getPosition().y + size.height/2 > mMaxSize.height)
			_target->setPositionY(mMaxSize.height - size.height/2);
		
		if (size.width > mMaxSize.width || size.height > mMaxSize.height)
		{
			mIsDone = true;
			mCallback();
		}
	}
}

bool BallAction::isDone() const
{
	return mIsDone;
}
