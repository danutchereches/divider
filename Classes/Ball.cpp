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
	_target->setScale(0.5f);
	if (_target->getParent())
		mParentSize = _target->getParent()->getContentSize();
	else
		mParentSize = cocos2d::Director::getInstance()->getWinSize();//TODO: maybe use visible size
	mMaxSize = mParentSize/2.5f;
	mMainPos = _target->getPosition();
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
		_target->setScale(_target->getScale() + dt / 40);
		
		cocos2d::Size size = _target->getBoundingBox().size;
		
		if (_target->getPosition().x < size.width/2)
		{
			mMainPos.x += size.width/2 - _target->getPosition().x;
			_target->setPositionX(mMainPos.x);
		}
		else if (_target->getPosition().x + size.width/2 > mParentSize.width)
		{
			mMainPos.x -= _target->getPosition().x + size.width/2 - mParentSize.width;
			_target->setPositionX(mMainPos.x);
		}
		
		if (_target->getPosition().y < size.height/2)
		{
			mMainPos.y += size.height/2 - _target->getPosition().y;
			_target->setPositionY(mMainPos.y);
		}
		else if (_target->getPosition().y + size.height/2 > mParentSize.height)
		{
			mMainPos.y -= _target->getPosition().y + size.height/2 - mParentSize.height;
			_target->setPositionY(mMainPos.y);
		}
		
		if (size.width > mMaxSize.width || size.height > mMaxSize.height)
		{
			mIsDone = true;
			mCallback();
		}
		else if (size.width > mMaxSize.width * 0.8f || size.height > mMaxSize.height * 0.8f)
		{
			_target->setPosition(mMainPos + cocos2d::Vec2(rand() % 2 - 1, rand() % 2 - 1));
		}
	}
}

bool BallAction::isDone() const
{
	return mIsDone;
}
