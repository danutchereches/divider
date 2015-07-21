#include "AppDelegate.h"

const int AppDelegate::BASE_WIDTH = 160;
const int AppDelegate::BASE_HEIGHT = 120;
const float AppDelegate::SCALE_L_RES = 3.0;   //scale 0.250000
const float AppDelegate::SCALE_M_RES = 4.0;   //scale 0.333333
const float AppDelegate::SCALE_H_RES = 6.0;   //scale 0.500000
const float AppDelegate::SCALE_XH_RES = 8.0;  //scale 0.666666
const float AppDelegate::SCALE_XXH_RES = 12.0;//scale 1.000000

bool AppDelegate::showAds = false;

cocos2d::plugin::ProtocolAnalytics* AppDelegate::pluginAnalytics = nullptr;
cocos2d::plugin::ProtocolGameServices* AppDelegate::pluginGameServices = nullptr;
cocos2d::plugin::ProtocolIAP* AppDelegate::pluginIAP = nullptr;

AppDelegate::AppDelegate()
{
	
}

AppDelegate::~AppDelegate()
{
	
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
	
	cocos2d::GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// initialize director
	auto director = cocos2d::Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview)
	{
		glview = cocos2d::GLViewImpl::createWithRect(std::string("divider"), cocos2d::Rect(0, 0, 480, 640), 1.0f);
		director->setOpenGLView(glview);
	}
	
	glview->setDesignResolutionSize(AppDelegate::BASE_WIDTH, AppDelegate::BASE_HEIGHT, ResolutionPolicy::NO_BORDER, ResolutionGravity::CENTER);
	cocos2d::Size frameSize = glview->getFrameSize();
	
	cocos2d::log("frame size %f, %f", frameSize.width, frameSize.height);
	
	std::vector<std::string> searchPath;
	
	if (frameSize.width >= 1920 * 0.9f) {
		this->resolutionScale = AppDelegate::SCALE_XXH_RES;
		searchPath.push_back("gfx/xxh_res");
	} else if (frameSize.width >= 1280 * 0.9f) {
		this->resolutionScale = AppDelegate::SCALE_XH_RES;
		searchPath.push_back("gfx/xh_res");
	} else if (frameSize.width >= 960 * 0.9f) {
		this->resolutionScale = AppDelegate::SCALE_H_RES;
		searchPath.push_back("gfx/h_res");
	} else if (frameSize.width >= 640 * 0.9f) {
		this->resolutionScale = AppDelegate::SCALE_M_RES;
		searchPath.push_back("gfx/m_res");
	} else {
		this->resolutionScale = AppDelegate::SCALE_L_RES;
		searchPath.push_back("gfx/l_res");
	}
	searchPath.push_back("gfx/all_res");
	searchPath.push_back("sfx");
	cocos2d::FileUtils::getInstance()->setSearchPaths(searchPath);
	
	cocos2d::log("resolution scale %f, %f", resolutionScale, (AppDelegate::BASE_WIDTH * this->resolutionScale) / AppDelegate::BASE_WIDTH);
	
	director->setContentScaleFactor(this->resolutionScale);
	
	// turn on display FPS
	director->setDisplayStats(true);
	
	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);
	
	pluginAnalytics = dynamic_cast<cocos2d::plugin::ProtocolAnalytics*>
			(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("GoogleAnalytics"));
	pluginAnalytics->setDebugMode(true);
	pluginAnalytics->startSession("UA-999-999");
	pluginAnalytics->setCaptureUncaughtException(true);
	pluginAnalytics->setSessionContinueMillis(300);
	
	/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	const char *gsPluginName = "AppleGameCenter";
#else
	const char *gsPluginName = "GpsGames";
#endif
	pluginGameServices = dynamic_cast<cocos2d::plugin::ProtocolGameServices*>
			(cocos2d::plugin::PluginManager::getInstance()->loadPlugin(gsPluginName));
	pluginGameServices->setDebugMode(true);
	pluginGameServices->startSession();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	const char *iapPluginName = "IAPAppleStore";
#else
	const char *iapPluginName = "IAPGooglePlay";
#endif
	pluginIAP = dynamic_cast<cocos2d::plugin::ProtocolIAP*>
			(cocos2d::plugin::PluginManager::getInstance()->loadPlugin(iapPluginName));
	cocos2d::plugin::TIAPDeveloperInfo devInfo;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	devInfo.insert(std::pair<std::string, std::string>("GooglePlayAppKey", ""));
#endif
//	pluginIAP->setInventoryCallback(CC_CALLBACK_1(AppDelegate::IAPInventoryCallback, this));
	pluginIAP->configDeveloperInfo(devInfo);
	pluginIAP->setDebugMode(true);
	*/
	
	cocos2d::UserDefault* ud = cocos2d::UserDefault::getInstance();
	showAds = !ud->getBoolForKey("remove_ads", false);
	
	// create a scene. it's an autorelease object
	auto scene = LoadingScene::create();
	
	// run
	director->runWithScene(scene);
	
	return true;
}

void AppDelegate::unloadPlugins()
{
	if (pluginAnalytics != nullptr)
	{
		pluginAnalytics->stopSession();
		cocos2d::plugin::PluginManager::getInstance()->unloadPlugin(pluginAnalytics->getPluginName());
		pluginAnalytics = nullptr;
	}
	
	if (pluginGameServices != nullptr)
	{
		pluginGameServices->stopSession();
		cocos2d::plugin::PluginManager::getInstance()->unloadPlugin(pluginGameServices->getPluginName());
		pluginGameServices = nullptr;
	}
	
	if (pluginIAP != nullptr)
	{
		cocos2d::plugin::PluginManager::getInstance()->unloadPlugin(pluginIAP->getPluginName());
		pluginIAP = nullptr;
	}
}

void AppDelegate::closeApp()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	
	AppDelegate::unloadPlugins();
	cocos2d::Director::getInstance()->end();
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	cocos2d::Director::getInstance()->stopAnimation();
	
	// if you use SimpleAudioEngine, it must be pause
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	cocos2d::Director::getInstance()->startAnimation();
	
	// if you use SimpleAudioEngine, it must resume here
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
