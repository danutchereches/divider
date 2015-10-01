#include "GameOverlay.h"
#include "Helpers.h"
#include "LevelSelectScene.h"

bool GameOverlay::init()
{
	if (!cocos2d::LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 240)))
		return false;
	
	ignoreAnchorPointForPosition(false);
	setAnchorPoint(cocos2d::Vec2::ZERO);
	setPosition(cocos2d::Director::getInstance()->getVisibleOrigin());
	setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
	
	/*
	cocos2d::Texture2D::TexParams texParams;
	texParams.magFilter = GL_LINEAR;
	texParams.minFilter = GL_LINEAR;
	texParams.wrapS = GL_REPEAT;
	texParams.wrapT = GL_REPEAT;
	
	cocos2d::Sprite* bg = cocos2d::Sprite::create("blux.png", cocos2d::Rect(0, 0, getContentSize().width, getContentSize().height));
	bg->getTexture()->setTexParameters(texParams);
	bg->setPosition(cocos2d::Vec2::ZERO);
	bg->setAnchorPoint(cocos2d::Vec2::ZERO);
	this->addChild(bg);
	
	auto fu = cocos2d::FileUtils::getInstance();
	auto fs = fu->getStringFromFile(fu->fullPathForFilename("frostedglass"));
	cocos2d::GLProgram* GLProgram = cocos2d::GLProgram::createWithByteArrays(cocos2d::ccPositionTextureColor_noMVP_vert, fs.c_str());
	
	auto glProgramState = cocos2d::GLProgramState::getOrCreateWithGLProgram(GLProgram);
	bg->setGLProgramState(glProgramState);
	
	auto size = bg->getTexture()->getContentSizeInPixels();
//	bg->getGLProgramState()->setUniformVec2("resolution", size);
//	bg->getGLProgramState()->setUniformFloat("blurRadius", 20);
//	bg->getGLProgramState()->setUniformFloat("sampleNum", 7.0f);
	*/
	
	mMenu = cocos2d::Menu::create();
	mMenu->ignoreAnchorPointForPosition(false);
	mMenu->setPosition(cocos2d::Vec2::ZERO);
	mMenu->setAnchorPoint(cocos2d::Vec2::ZERO);
	mMenu->setContentSize(getContentSize());
	this->addChild(mMenu);
	
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameOverlay::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameOverlay::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameOverlay::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	auto listener2 = cocos2d::EventListenerKeyboard::create();
	listener2->onKeyPressed = CC_CALLBACK_2(GameOverlay::onKeyPressed, this);
	listener2->onKeyReleased = CC_CALLBACK_2(GameOverlay::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
	
	return true;
}

bool GameOverlay::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You touched id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	event->stopPropagation();
	
	return true;
}

void GameOverlay::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You moved id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	event->stopPropagation();
}

void GameOverlay::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You ended move id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	event->stopPropagation();
}

void GameOverlay::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button press %d", (int) keyCode);
	
	event->stopPropagation();
}

void GameOverlay::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button release %d", (int) keyCode);
	
	event->stopPropagation();
	
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		
	}
}

bool PauseOverlay::init()
{
	if (!GameOverlay::init())
		return false;
	
	cocos2d::Sprite* title = cocos2d::Sprite::createWithSpriteFrameName("pause_title");
	title->setPosition(getContentSize().width/2, getContentSize().height * 0.8f);
	addChild(title);
	
	cocos2d::MenuItemSprite* resume = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("play_btn_small"),
			nullptr, [this] (cocos2d::Ref* node) { if (resumeCallback) resumeCallback(); removeFromParentAndCleanup(true); });
	resume->setPosition(getContentSize().width * 0.5f, getContentSize().height * 0.5f);
	mMenu->addChild(resume);
	
	cocos2d::MenuItemSprite* restart = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("retry_btn"),
			nullptr, [this] (cocos2d::Ref* node) { if (restartCallback) restartCallback(); removeFromParentAndCleanup(true); });
	restart->setPosition(getContentSize().width * 0.5f, getContentSize().height * 0.25f);
	mMenu->addChild(restart);
	
	auto exitBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("menu_btn"),
			nullptr, [this] (cocos2d::Ref* btn) { if (exitCallback) exitCallback(); removeFromParentAndCleanup(true); });
	exitBtn->setPosition(9, 9);
	mMenu->addChild(exitBtn);
	
	resumeCallback = nullptr;
	restartCallback = nullptr;
	exitCallback = nullptr;
	
	return true;
}

void PauseOverlay::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		event->stopPropagation();
		
		if (resumeCallback != nullptr)
			resumeCallback();
		
		removeFromParentAndCleanup(true);
	}
	else
	{
		GameOverlay::onKeyReleased(keyCode, event);
	}
}
DieOverlay* DieOverlay::create(int nr, int score/* = 0*/)
{
	DieOverlay* overlay = new (std::nothrow) DieOverlay();
	
	if (overlay && overlay->init(nr, score))
	{
		overlay->autorelease();
		return overlay;
	}
	else
	{
		delete overlay;
		overlay = nullptr;
		return nullptr;
	}
}

bool DieOverlay::init(int nr, int score)
{
	if (!GameOverlay::init())
		return false;
	
	cocos2d::Label* title = cocos2d::Label::createWithTTF("GAME OVER", "fonts/semibold.otf", 10);
	title->setPosition(getContentSize().width/2, getContentSize().height * 0.80f);
	addChild(title);
	
	std::string message;
	
	if (nr > 0)
	{
		int length = 0, divisors[100], nrDiv = 0;
		char* buf = (char*) malloc(100);
		if (buf != nullptr)
		{
			length += sprintf(buf, "%d is divisible by", nr);
			
			for (int i = 2; i < nr && i < 100; i++)
				if (nr % i == 0)
					divisors[nrDiv++] = i;
			
			if (nrDiv == 0)
				sprintf(buf, "Oops! Prime number.\nSomebody fucked this up.");
			else if (nrDiv == 1)
				sprintf(buf + length, " %d.", divisors[0]);
			else
				for (int i = 0; i < nrDiv; i++)
					length += sprintf(buf + length, i == nrDiv - 1 ? " and %d." : (i == 0 ? " %d" : ", %d"), divisors[i]);
			
			message = buf;
			free(buf);
		}
	}
	else
	{
		message = "Try harder.\nDivide more numbers to pass the level.";
	}
	
	cocos2d::Label* text = cocos2d::Label::createWithTTF(message, "fonts/semibold.otf", 7, cocos2d::Size(80, 0), cocos2d::TextHAlignment::CENTER);
	text->setLineSpacing(2.0f);
	text->setPosition(getContentSize().width/2, getContentSize().height * 0.50f);
	addChild(text);
	
	if (score >= 0)
	{
		cocos2d::Label* scoreLevel = cocos2d::Label::createWithTTF(helpers::String::format("SCORE: %d", score), "fonts/semibold.otf", 7);
		scoreLevel->setPosition(getContentSize().width/2, getContentSize().height * 0.40f);
		addChild(scoreLevel);
		
		text->setPositionY(getContentSize().height * 0.6f);
	}
	
	cocos2d::MenuItemSprite* restart = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("retry_btn"),
			nullptr, [this] (cocos2d::Ref* node) { if (restartCallback) restartCallback(); removeFromParentAndCleanup(true); });
	restart->setPosition(getContentSize().width * 0.5f, getContentSize().height * 0.25f);
	mMenu->addChild(restart);
	
	auto exitBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("menu_btn"),
			nullptr, [this] (cocos2d::Ref* btn) { if (exitCallback) exitCallback(); removeFromParentAndCleanup(true); });
	exitBtn->setPosition(9, 9);
	mMenu->addChild(exitBtn);
	
	restartCallback = nullptr;
	exitCallback = nullptr;
	
	return true;
}

void DieOverlay::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		event->stopPropagation();
		
		if (exitCallback != nullptr)
			exitCallback();
	}
	else
	{
		GameOverlay::onKeyReleased(keyCode, event);
	}
}

FinishOverlay* FinishOverlay::create(int score, int level/* = -1*/, int stars/* = -1*/)
{
	FinishOverlay* overlay = new (std::nothrow) FinishOverlay();
	
	if (overlay && overlay->init(score, level, stars))
	{
		overlay->autorelease();
		return overlay;
	}
	else
	{
		delete overlay;
		overlay = nullptr;
		return nullptr;
	}
}

bool FinishOverlay::init(int score, int level, int stars)
{
	if (!GameOverlay::init())
		return false;
	
	cocos2d::Label* title = cocos2d::Label::createWithTTF(stars > 0 ? "CONGRATULATIONS!" : "ALMOST THERE", "fonts/semibold.otf", 9);
	title->setPosition(getContentSize().width/2, getContentSize().height * 0.85f);
	addChild(title);
	
	cocos2d::Sprite* levelBg = cocos2d::Sprite::createWithSpriteFrameName("level_win_bg");
	levelBg->setPosition(getContentSize().width * 0.5f, getContentSize().height * 0.7f);
	addChild(levelBg);
	
	cocos2d::Label* levelLabel = cocos2d::Label::createWithTTF(helpers::String::format("%d", level), "fonts/semibold.otf", 14);
	levelLabel->setPosition(getContentSize().width * 0.5f, getContentSize().height * 0.7f);
	addChild(levelLabel);
	
	for (int i = 0; i < 3; i++)
	{
		cocos2d::Sprite* star = cocos2d::Sprite::createWithSpriteFrameName(stars > i ? "big_star_w" : "big_star_b");
		star->setPosition(levelBg->getPositionX() - 13 + 13 * i, levelBg->getPositionY() - (i == 1 ? 22 : 19));
		addChild(star);
	}
	
	cocos2d::Label* scoreLevel = cocos2d::Label::createWithTTF(helpers::String::format("SCORE: %d", score), "fonts/semibold.otf", 7);
	scoreLevel->setPosition(getContentSize().width/2, getContentSize().height * 0.40f);
	addChild(scoreLevel);
	
	cocos2d::MenuItemSprite* restart = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("retry_btn"),
			nullptr, [this] (cocos2d::Ref* node) { if (restartCallback) restartCallback(); removeFromParentAndCleanup(true); });
	restart->setPosition(getContentSize().width * 0.5f, getContentSize().height * 0.25f);
	mMenu->addChild(restart);
	
	if (stars > 0 && level < LevelSelectScene::LEVEL_NR)
	{
		restart->setPositionX(getContentSize().width * 0.33f);
		
		cocos2d::MenuItemSprite* next = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("play_btn_small"),
				nullptr, [this] (cocos2d::Ref* node) { if (nextLevelCallback) nextLevelCallback(); removeFromParentAndCleanup(true); });
		next->setPosition(getContentSize().width * 0.66f, getContentSize().height * 0.25f);
		next->setScale(0.67f);
		mMenu->addChild(next);
	}
	
	auto exitBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("menu_btn"),
			nullptr, [this] (cocos2d::Ref* btn) { if (exitCallback) exitCallback(); removeFromParentAndCleanup(true); });
	exitBtn->setPosition(9, 9);
	mMenu->addChild(exitBtn);
	
	restartCallback = nullptr;
	nextLevelCallback = nullptr;
	exitCallback = nullptr;
	
	return true;
}

void FinishOverlay::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		event->stopPropagation();
		
		if (exitCallback != nullptr)
			exitCallback();
	}
	else
	{
		GameOverlay::onKeyReleased(keyCode, event);
	}
}
