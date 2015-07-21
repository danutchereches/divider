#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "cocos2d.h"

class MyMenuItem : public cocos2d::MenuItemSprite
{
public:
	static MyMenuItem* create(cocos2d::Node* sprite, const cocos2d::ccMenuCallback& callback);
	static MyMenuItem* create(cocos2d::Node* enabledSprite, cocos2d::Node* disabledSprite, const cocos2d::ccMenuCallback& callback);
	
	void selected() override;
	void unselected() override;
	void activate() override;
	
	bool getEnabled();
	void setEnabled(bool state);
private:
	bool mEnabled;
};

template<class T> class GenericPool
{
public:
	GenericPool() : mAvailableItems(), mUnrecycledItemCount(0)
	{
		
	}
	virtual ~GenericPool()
	{
		
	}
	
	void initWithCapacity(int capacity)
	{
		batchAllocatePoolItems(capacity);
	}
	int getUnrecycledItemCount()
	{
		return mUnrecycledItemCount;
	}
	int getAvailableItemCount()
	{
		return mAvailableItems.size();
	}
	void batchAllocatePoolItems(int pCount)
	{
		cocos2d::log("Increasing pool capacity with %d (current size %d)", pCount, (int) mAvailableItems.size() + mUnrecycledItemCount);
		
		for (int i = 0; i < pCount; i++)
		{
			T item = onAllocatePoolItem();
			item->retain();
			mAvailableItems.push_back(item);
		}
	}
	T obtainPoolItem()
	{
		T item;
		if (mAvailableItems.size() == 0)
		{
			batchAllocatePoolItems(1);
		}
		
		item = mAvailableItems.back();
		mAvailableItems.pop_back();
		
		onObtainItem(item);
		
		mUnrecycledItemCount++;
		
		return item;
	}
	
	void recyclePoolItem(T pItem)
	{
		CCASSERT(pItem, "Cannot recycle null item!");
		
		onRecycleItem(pItem);
		mAvailableItems.push_back(pItem);
		mUnrecycledItemCount--;
		if (mUnrecycledItemCount < 0)
		{
			cocos2d::log("More items recycled than obtained!");
		}
	}
	
	void clearPool()
	{
		while (mAvailableItems.size() > 0)
		{
			mAvailableItems.pop_back();
		}
	}

protected:
	virtual T onAllocatePoolItem() = 0;
	virtual void onObtainItem(T pItem)
	{
		
	}
	virtual void onRecycleItem(T pItem)
	{
		
	}

private:
	std::vector<T> mAvailableItems;
	int mUnrecycledItemCount;
};

class SpritePool : public GenericPool<cocos2d::Sprite*>
{
public:
	void init(int capacity, cocos2d::SpriteFrame* spriteFrame, cocos2d::Node* parent);
protected:
	cocos2d::Node* mParent;
	cocos2d::SpriteFrame* mSpriteFrame;
	cocos2d::Sprite* onAllocatePoolItem() override;
	void onRecycleItem(cocos2d::Sprite* item) override;
};

#endif // __OBJECTS_H__
