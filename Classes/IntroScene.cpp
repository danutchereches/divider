#include "IntroScene.h"

IntroScene::IntroScene()
{
	cocos2d::log("intro scene constructed");
}

IntroScene::~IntroScene()
{
	cocos2d::log("intro scene destructed");
}

bool IntroScene::init()
{
	if (!cocos2d::Scene::init())
		return false;
	
	mScreenSize = cocos2d::Director::getInstance()->getWinSize();
	mVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	mOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	mIsGameServicesAvailable = false;
	
	cocos2d::Texture2D::TexParams texParams;
	texParams.magFilter = GL_LINEAR;
	texParams.minFilter = GL_LINEAR;
	texParams.wrapS = GL_REPEAT;
	texParams.wrapT = GL_REPEAT;
	
	cocos2d::Sprite* bg = cocos2d::Sprite::create("bg.png", cocos2d::Rect(mOrigin.x, mOrigin.y, mVisibleSize.width, mVisibleSize.height));
	bg->getTexture()->setTexParameters(texParams);
	bg->setPosition(cocos2d::Vec2(mOrigin.x + mVisibleSize.width/2, mOrigin.y + mVisibleSize.height/2));
	addChild(bg);
	
	auto logo = cocos2d::Sprite::create("logo.png");
	logo->setPosition(cocos2d::Vec2(mOrigin.x + mVisibleSize.width/2, mOrigin.y + mVisibleSize.height * 0.8f));
	this->addChild(logo);
	
	auto loading = cocos2d::Label::createWithTTF("Loading .. ", "fonts/semibold.otf", 6);
	loading->setColor(cocos2d::Color3B::WHITE);
	loading->setPosition(cocos2d::Vec2(mOrigin.x + mVisibleSize.width/2, mOrigin.y + mVisibleSize.height * 0.45f));
	loading->setTag(13);
	this->addChild(loading);
	
	schedule(schedule_selector(IntroScene::load), 0);
	
	return true;
}

void IntroScene::onEnter()
{
	cocos2d::Scene::onEnter();
	
}

void IntroScene::load(float dt)
{
	Loader::loadEverything();
	
	unschedule(schedule_selector(IntroScene::load));
	
	this->removeChildByTag(13);
	
	auto menu = cocos2d::Menu::create();
	menu->ignoreAnchorPointForPosition(false);
	menu->setPosition(cocos2d::Vec2(mOrigin.x, mOrigin.y));
	menu->setAnchorPoint(cocos2d::Vec2::ZERO);
	menu->setContentSize(cocos2d::Size(mVisibleSize.width, mVisibleSize.height));
	menu->setTag(200);
	this->addChild(menu, 2);
	
	auto mode1Btn = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("?", "fonts/default.otf", 6),
			[] (cocos2d::Ref* btn) {
		cocos2d::Director::getInstance()->pushScene(GameMode1Scene::create());
	});
	mode1Btn->setAnchorPoint(cocos2d::Vec2(0, 1));
	mode1Btn->setPosition(2, mVisibleSize.height - 2);
	menu->addChild(mode1Btn);
	
	auto mode2Btn = cocos2d::MenuItemLabel::create(cocos2d::Label::createWithTTF("?", "fonts/default.otf", 6),
			[] (cocos2d::Ref* btn) {
		cocos2d::Director::getInstance()->pushScene(GameMode2InfiniteScene::create());
	});
	mode2Btn->setAnchorPoint(cocos2d::Vec2(1, 1));
	mode2Btn->setPosition(mVisibleSize.width - 2, mVisibleSize.height - 2);
	menu->addChild(mode2Btn);
	
	cocos2d::Sprite* btnBg = cocos2d::Sprite::createWithSpriteFrameName("big_play_bg");
	btnBg->setPosition(mOrigin.x + mVisibleSize.width * 0.5f, mOrigin.y + mVisibleSize.height * 0.4f);
	this->addChild(btnBg, 1);
	
	auto mode3Btn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("big_play_btn"),
			nullptr, [] (cocos2d::Ref* btn) {
		cocos2d::Director::getInstance()->pushScene(LevelSelectScene::create());
	});
	mode3Btn->setPosition(mVisibleSize.width * 0.5f, mVisibleSize.height * 0.4f);
	menu->addChild(mode3Btn);
	
	auto gameServicesBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("google_play_btn"),
			nullptr, [] (cocos2d::Ref* btn) {
		if (AppDelegate::pluginGameServices)
		{
			if (AppDelegate::pluginGameServices->isSignedIn())
				AppDelegate::pluginGameServices->signOut();
			else
				AppDelegate::pluginGameServices->initiateSignIn();
		}
	});
	gameServicesBtn->setPosition(mVisibleSize.width * 0.5f, mVisibleSize.height * 0.10f);
	gameServicesBtn->setTag(300);
	gameServicesBtn->setVisible(!mIsGameServicesAvailable);
	menu->addChild(gameServicesBtn);
	
	auto achievementsBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("achievements_btn"),
			nullptr, [] (cocos2d::Ref* btn) {
		if (AppDelegate::pluginGameServices && AppDelegate::pluginGameServices->isSignedIn())
			AppDelegate::pluginGameServices->showAchievements();
	});
	achievementsBtn->setPosition(mVisibleSize.width * 0.3f, mVisibleSize.height * 0.12f);
	achievementsBtn->setTag(301);
	achievementsBtn->setVisible(mIsGameServicesAvailable);
	menu->addChild(achievementsBtn);
	
	auto leaderboardsBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("leaderboards_btn"),
			nullptr, [] (cocos2d::Ref* btn) {
		if (AppDelegate::pluginGameServices && AppDelegate::pluginGameServices->isSignedIn())
			AppDelegate::pluginGameServices->showLeaderboards();
	});
	leaderboardsBtn->setPosition(mVisibleSize.width * 0.7f, mVisibleSize.height * 0.12f);
	leaderboardsBtn->setTag(302);
	leaderboardsBtn->setVisible(mIsGameServicesAvailable);
	menu->addChild(leaderboardsBtn);
	/*
	auto exitBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("back_btn"),
			nullptr, [] (cocos2d::Ref* btn) {
		AppDelegate::closeApp();
	});
	exitBtn->setPosition(9, 9);
	menu->addChild(exitBtn);
	*/
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(IntroScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(IntroScene::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	schedule(schedule_selector(IntroScene::update), 0.5f);
	
	if (AppDelegate::pluginAnalytics != nullptr)
		AppDelegate::pluginAnalytics->logPageView("intro");
	
	if (AppDelegate::pluginGameServices)
		AppDelegate::pluginGameServices->startSession(2);
}

void IntroScene::update(float dt)
{
	if (AppDelegate::pluginGameServices->isSignedIn() != mIsGameServicesAvailable)
	{
		mIsGameServicesAvailable = AppDelegate::pluginGameServices->isSignedIn();
		
		auto menu = this->getChildByTag(200);
		
		auto gs = menu->getChildByTag(300);
		gs->setOpacity(!mIsGameServicesAvailable ? 0 : 255);
		gs->runAction(!mIsGameServicesAvailable
				? (cocos2d::ActionInterval*) cocos2d::Sequence::create(cocos2d::Show::create(), cocos2d::FadeIn::create(0.10f), nullptr)
				: (cocos2d::ActionInterval*) cocos2d::Sequence::create(cocos2d::FadeOut::create(0.10f), cocos2d::Hide::create(), nullptr));
		
		auto ach = menu->getChildByTag(301);
		ach->setOpacity(mIsGameServicesAvailable ? 0 : 255);
		ach->runAction(mIsGameServicesAvailable
				? (cocos2d::ActionInterval*) cocos2d::Sequence::create(cocos2d::Show::create(), cocos2d::FadeIn::create(0.25f), nullptr)
				: (cocos2d::ActionInterval*) cocos2d::Sequence::create(cocos2d::FadeOut::create(0.25f), cocos2d::Hide::create(), nullptr));
		
		auto lead = menu->getChildByTag(302);
		lead->setOpacity(mIsGameServicesAvailable ? 0 : 255);
		lead->runAction(mIsGameServicesAvailable
				? (cocos2d::ActionInterval*) cocos2d::Sequence::create(cocos2d::Show::create(), cocos2d::FadeIn::create(0.25f), nullptr)
				: (cocos2d::ActionInterval*) cocos2d::Sequence::create(cocos2d::FadeOut::create(0.25f), cocos2d::Hide::create(), nullptr));
		
		cocos2d::UserDefault* ud = cocos2d::UserDefault::getInstance();
		
		int stars = Level::getGlobalStars();
		if (stars > 0)
		{
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_FIRST_LEVEL);
			if (LevelSelectScene::LEVELS[LevelSelectScene::LEVEL_NR-1].getStars() > 0)
				AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_ALL_LEVELS);
			
			int fullStars = 0;
			for (int i = 0; i < LevelSelectScene::LEVEL_NR; i++)
				if (LevelSelectScene::LEVELS[i].getStars() >= 3)
					fullStars++;
			
			if (fullStars >= LevelSelectScene::LEVEL_NR)
				AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_ALL_3_STARS);
			else if (fullStars >= 1)
				AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_FIRST_3_STARS);
		}
		
		int highscore1 = ud->getIntegerForKey("highscore_mode_1", 0);
		if (highscore1 > 0)
			AppDelegate::pluginGameServices->publishScore(GameScene::LEADERBOARD_MODE_1_ID, highscore1);
		
		int highscore2 = ud->getIntegerForKey("highscore_mode_2", 0);
		if (highscore2 > 0)
		{
			AppDelegate::pluginGameServices->publishScore(GameScene::LEADERBOARD_MODE_2_ID, highscore2);
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_PLAY_GAME_2);
		}
		int maxWave = ud->getIntegerForKey("max_wave", 0);
		if (maxWave >= 1)
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_1_WAVE);
		if (maxWave >= 5)
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_5_WAVES);
		if (maxWave >= 10)
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_10_WAVES);
		if (maxWave >= 50)
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_50_WAVES);
		
		int globalScore = ud->getIntegerForKey("global_score", 0);
		if (globalScore >= 1)
		{
			AppDelegate::pluginGameServices->publishScore(GameScene::LEADERBOARD_GLOBAL_DIVIDED_ID, globalScore);
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_DIVIDE_1_NR);
		}
		if (globalScore >= 25)
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_DIVIDE_25_NR);
		if (globalScore >= 100)
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_DIVIDE_100_NR);
		if (globalScore >= 250)
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_DIVIDE_250_NR);
		if (globalScore >= 1000)
			AppDelegate::pluginGameServices->unlockAchievement(GameScene::ACHIEVEMENT_DIVIDE_1000_NR);
	}
}

void IntroScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button press %d", (int) keyCode);
	
}

void IntroScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button release %d", (int) keyCode);
	
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (AppDelegate::pluginAnalytics != nullptr)
			AppDelegate::pluginAnalytics->logEvent("click_back_btn");
		
		AppDelegate::closeApp();
	}
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_VOLUME_DOWN)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->decreaseVolume();
	}
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_VOLUME_UP)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->increaseVolume();
	}
}
