#ifndef __LOADINGSCENE_H__
#define __LOADINGSCENE_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "Helpers.h"

class LoadingScene : public cocos2d::Scene {
public:
	LoadingScene();
	~LoadingScene();
	
	CREATE_FUNC(LoadingScene);
	void onEnter() override;
protected:
	virtual bool init() override;
private:
	cocos2d::Size mScreenSize;
	cocos2d::Size mVisibleSize;
	cocos2d::Vec2 mOrigin;
	
	void load(float dt);
};

#endif // __LOADINGSCENE_H__
