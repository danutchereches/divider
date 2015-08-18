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

const float BallAction::SCALE_FROM = 0.35f;
const float BallAction::SCALE_TO = 1.0f;
const float BallAction::TOTAL_TIME = 30.0f;
const float BallAction::SHAKE_TIME = 10.0f;
//auto calculated
const float BallAction::SCALE_INDEX = TOTAL_TIME / (SCALE_TO - SCALE_FROM);
const float BallAction::SHAKE_INDEX = SCALE_TO - SHAKE_TIME * (SCALE_TO - SCALE_FROM) / TOTAL_TIME;

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
	_target->setScale(SCALE_FROM);
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
		_target->setScale(_target->getScale() + dt / SCALE_INDEX);
		
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
		
		if (_target->getScale() >= SCALE_TO)
		{
			mIsDone = true;
			mCallback();
		}
		else if (_target->getScale() > SHAKE_INDEX)
		{
			_target->setPosition(mMainPos + cocos2d::Vec2(rand() % 2 - 1, rand() % 2 - 1));
		}
	}
}

bool BallAction::isDone() const
{
	return mIsDone;
}
