#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "audio/include/SimpleAudioEngine.h"
#include "ProtocolAnalytics.h"
#include "ProtocolGameServices.h"
#include "ProtocolIAP.h"
#include "PluginManager.h"
#include "LanguageManager.h"
#include "Helpers.h"
#include "LoadingScene.h"

/**
 @brief    The cocos2d Application.

 The reason for implement as private inheritance is to hide some interface call by Director.
 */
class AppDelegate: private cocos2d::Application
{
public:
	AppDelegate();
	virtual ~AppDelegate();
	
	virtual void initGLContextAttrs();
	
	/**
	 @brief    Implement Director and Scene init code here.
	 @return true    Initialize success, app continue.
	 @return false   Initialize failed, app terminate.
	 */
	virtual bool applicationDidFinishLaunching();
	
	/**
	 @brief  The function be called when the application enter background
	 @param  the pointer of the application
	 */
	virtual void applicationDidEnterBackground();
	
	/**
	 @brief  The function be called when the application enter foreground
	 @param  the pointer of the application
	 */
	virtual void applicationWillEnterForeground();
	
	static const int BASE_WIDTH;
	static const int BASE_HEIGHT;
	static const float SCALE_L_RES;
	static const float SCALE_M_RES;
	static const float SCALE_H_RES;
	static const float SCALE_XH_RES;
	static const float SCALE_XXH_RES;
	
	static void unloadPlugins();
	static void closeApp();
	
	float resolutionScale = 1;
	
	static bool showAds;
	
	static cocos2d::plugin::ProtocolAnalytics* pluginAnalytics;
	static cocos2d::plugin::ProtocolGameServices* pluginGameServices;
	static cocos2d::plugin::ProtocolIAP* pluginIAP;
};

#endif // _APP_DELEGATE_H_
