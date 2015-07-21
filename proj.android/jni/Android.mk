LOCAL_PATH := $(call my-dir)
COCOS_PATH := /usr/local/cocos2d-x

include $(CLEAR_VARS)

$(call import-add-path,$(COCOS_PATH))
$(call import-add-path,$(COCOS_PATH)/external)
$(call import-add-path,$(COCOS_PATH)/cocos)
$(call import-add-path,$(COCOS_PATH)/plugin/publish)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := main.cpp \
					../../Classes/Helpers.cpp \
					../../Classes/LanguageManager.cpp \
					../../Classes/AppDelegate.cpp \
					../../Classes/Loader.cpp \
					../../Classes/LoadingScene.cpp \
					../../Classes/Objects.cpp \
					../../Classes/GameScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += PluginProtocolStatic

# LOCAL_STATIC_LIBRARIES += box2d_static
# LOCAL_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_STATIC_LIBRARIES += spine_static
# LOCAL_STATIC_LIBRARIES += cocostudio_static
# LOCAL_STATIC_LIBRARIES += cocos_network_static
# LOCAL_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,protocols/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
# $(call import-module,extensions)
