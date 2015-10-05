#include "LevelSelectScene.h"

Level LevelSelectScene::LEVELS[] =
{
		Level(101, 2, 10.0f, 20, 30), // level 1
		Level(102, 5, 10.0f, 20, 30), // level 2
		Level(103, 3, 10.0f, 20, 40), // level 3
		Level(104, 4, 11.0f, 20, 30), // level 4
		Level(105, 6, 12.0f, 20, 40), // level 5
		Level(106, 3, 13.0f, 20, 50), // level 6
		Level(107, 5, 20.0f, 20, 60), // level 7
		Level(108, 8, 15.0f, 20, 30), // level 8
		Level(109, 9, 14.0f, 20, 30), // level 9
		Level(110, 7, 14.0f, 20, 30), // level 10
		Level(111, 6, 16.0f, 20, 60), // level 11
		Level(112, 4, 17.0f, 20, 50), // level 12
		Level(113, 7, 15.0f, 20, 50), // level 13
		Level(114, 9, 14.5f, 20, 50), // level 14
		Level(115, 7, 17.0f, 20, 80)  // level 15
};
const int LevelSelectScene::LEVEL_NR = 15;

LevelSelectScene::LevelSelectScene() : mMenu(nullptr)
{
	cocos2d::log("level select scene constructed");
}

LevelSelectScene::~LevelSelectScene()
{
	cocos2d::log("level select scene destructed");
}

bool LevelSelectScene::init()
{
	if (!cocos2d::Scene::init())
		return false;
	
	mScreenSize = cocos2d::Director::getInstance()->getWinSize();
	mVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	mOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	cocos2d::Texture2D::TexParams texParams;
	texParams.magFilter = GL_LINEAR;
	texParams.minFilter = GL_LINEAR;
	texParams.wrapS = GL_REPEAT;
	texParams.wrapT = GL_REPEAT;
	
	cocos2d::Sprite* bg = cocos2d::Sprite::create("bg.png", cocos2d::Rect(mOrigin.x, mOrigin.y, mVisibleSize.width, mVisibleSize.height));
	bg->getTexture()->setTexParameters(texParams);
	bg->setPosition(cocos2d::Vec2(mOrigin.x + mVisibleSize.width/2, mOrigin.y + mVisibleSize.height/2));
	addChild(bg);
	
	auto logo = cocos2d::Sprite::createWithSpriteFrameName("levels_title");
	logo->setPosition(cocos2d::Vec2(mOrigin.x + mVisibleSize.width/2, mOrigin.y + mVisibleSize.height * 0.92f));
	this->addChild(logo);
	
	mMenu = cocos2d::Menu::create();
	mMenu->ignoreAnchorPointForPosition(false);
	mMenu->setPosition(cocos2d::Vec2(mOrigin.x, mOrigin.y));
	mMenu->setAnchorPoint(cocos2d::Vec2::ZERO);
	mMenu->setContentSize(cocos2d::Size(mVisibleSize.width, mVisibleSize.height));
	this->addChild(mMenu);
	
	cocos2d::MenuItem* exitBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("back_btn"),
			nullptr, [] (cocos2d::Ref* btn) {
		cocos2d::Director::getInstance()->popScene();
	});
	exitBtn->setPosition(9, 9);
	mMenu->addChild(exitBtn);
	
	cocos2d::MenuItem* menuItem;
	float width = (mVisibleSize.width - 20) / 3;
	float height = (mVisibleSize.height - 40) / 5;
	int n = LEVEL_NR;
	//cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("bitmap_font");
	//cocos2d::log("rect %f %f", frame->getOriginalSize().width, frame->getOriginalSizeInPixels().height);
	
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			int levelNr = y * 3 + x;
			
			if (levelNr >= n)
				break;
			
			menuItem = cocos2d::MenuItem::create();
			menuItem->setPosition(cocos2d::Vec2(10 + width * x, mVisibleSize.height - 25 - y * height));
			menuItem->setContentSize(cocos2d::Size(width, height));
			menuItem->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
			menuItem->setTag(LEVELS[levelNr].getId());
			mMenu->addChild(menuItem);
			
			cocos2d::Sprite* btnBg = cocos2d::Sprite::createWithSpriteFrameName("level_btn");
			btnBg->setPosition(width/2, height * 0.60f);
			btnBg->setTag(99999);
			menuItem->addChild(btnBg);
			
			cocos2d::Label* label = cocos2d::Label::createWithTTF(helpers::String::format("%d", levelNr+1), "fonts/semibold.otf", 6);
			label->enableShadow(cocos2d::Color4B::BLACK, cocos2d::Size(0.25f, -0.25f));
			label->setPosition(btnBg->getContentSize().width/2, btnBg->getContentSize().height*0.5f);
			/*
			cocos2d::Label* label = cocos2d::Label::createWithCharMap(frame->getTexture(),
					frame->getOriginalSizeInPixels().width/10, frame->getOriginalSizeInPixels().height, '0');
			label->setString(helpers::String::format("%d", levelNr+1));
			label->setPosition(btnBg->getContentSize().width/2, btnBg->getContentSize().height*0.5f);
			*//*
			cocos2d::LayerColor* lc = cocos2d::LayerColor::create(cocos2d::Color4B::RED);
			lc->ignoreAnchorPointForPosition(false);
			lc->setPosition(btnBg->getContentSize().width/2, btnBg->getContentSize().height/2);
			lc->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
			lc->setContentSize(label->getContentSize());
			btnBg->addChild(lc);
			*/
			btnBg->addChild(label);
			
			for (int i = 0; i < 3; i++)
			{
				cocos2d::Sprite* star = cocos2d::Sprite::createWithSpriteFrameName("star_b");
				star->setPosition(width / 4 * (i+1), i == 1 ? 5 : 6);
				star->setTag(i+1);
				menuItem->addChild(star);
			}
		}
	}
	
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(LevelSelectScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(LevelSelectScene::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	if (AppDelegate::pluginAnalytics != nullptr)
	{
		cocos2d::plugin::LogEventParamMap params;
		params.insert(cocos2d::plugin::LogEventParamPair(GameScene::ANALYTICS_GAME_MODE_INDEX, "mode_3"));
		AppDelegate::pluginAnalytics->logPageView("level_select", &params);
	}
	
	return true;
}

void LevelSelectScene::onEnter()
{
	cocos2d::Scene::onEnter();
	
	cocos2d::MenuItem* menuItem;
	cocos2d::Node* label;
	cocos2d::Sprite* star;
	cocos2d::SpriteFrame* starW = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName("star_w");
	int n = sizeof(LEVELS)/sizeof(Level);
	bool unlock = true;
	
	for (int i = 0; i < n; i++)
	{
		menuItem = dynamic_cast<cocos2d::MenuItem*>(mMenu->getChildByTag(LEVELS[i].getId()));
		
		if (!menuItem)
			continue;
		
		label = menuItem->getChildByTag(99999);
		if (unlock)
		{
			label->setColor(cocos2d::Color3B::WHITE);
			menuItem->setCallback([i] (cocos2d::Ref* btn) {
				cocos2d::Director::getInstance()->pushScene(GameMode2LevelScene::create(&LEVELS[i]));
			});
		}
		else
		{
			label->setColor(cocos2d::Color3B::GRAY);
		}
		
		if (LEVELS[i].getStars() >= 1)
		{
			star = dynamic_cast<cocos2d::Sprite*>(menuItem->getChildByTag(1));
			star->setSpriteFrame(starW);
			unlock = true;
		}
		else
			unlock = false;
		
		if (LEVELS[i].getStars() >= 2)
		{
			star = dynamic_cast<cocos2d::Sprite*>(menuItem->getChildByTag(2));
			star->setSpriteFrame(starW);
		}
		
		if (LEVELS[i].getStars() >= 3)
		{
			star = dynamic_cast<cocos2d::Sprite*>(menuItem->getChildByTag(3));
			star->setSpriteFrame(starW);
		}
	}
}

void LevelSelectScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button press %d", (int) keyCode);
	
}

void LevelSelectScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button release %d", (int) keyCode);
	
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (AppDelegate::pluginAnalytics != nullptr)
			AppDelegate::pluginAnalytics->logEvent("click_back_btn");
		
		cocos2d::Director::getInstance()->popScene();
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
