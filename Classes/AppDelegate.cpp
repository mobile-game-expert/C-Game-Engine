/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "LPTLoginScene.hpp"
#include "LPTGameSelectionScene.hpp"
#include "LPTHeaders.h"
#include "LPTGameScene.hpp"
#include "LPTGameSceneRW.hpp"
#include "LPTServerListener.hpp"
#include "LPTEvents.hpp"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
    APP_MANAGER->checkInternetConnection();
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("LePasseTrappe", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("LePasseTrappe");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    sdkbox::PluginFacebook::init();
    sdkbox::IAP::init();
    setUpSearchPathAndResources(director);
    register_all_packages();
    
    APP_MANAGER->getDebugValues();
    App42API::Initialize(APPWARP_API_KEY, APPWARP_SECRET_KEY);
    AppWarp::Client::initialize(APPWARP_API_KEY,APPWARP_SECRET_KEY);
    CustomCodeService::Initialize(APPWARP_API_KEY, APPWARP_SECRET_KEY);
    
    //preload sound and music
    SoundManager::preloadBGMusic();
    SoundManager::preloadEffects();
    
    // create a scene. it's an autorelease object
    BaseScene *scene;
    if(PLAYER->getLoginState() == NONE) {
        scene = LPTLoginScene::createScene(kNOT_LOGGED_IN);
    } else {
        scene = LPTGameSelectionScene::createScene();
    }
    // run
    director->runWithScene(scene);
    UserDefault::getInstance()->setBoolForKey(kShowLoaderForFirstTime, true);
    APP_MANAGER->appStartTime = GameUtils::getCurrentTime();
    APP_MANAGER->checkInternetConnection();
    APP_MANAGER->totalSpendsByUser = APP_MANAGER->getTotalSpendsByUser();
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    
    Director::getInstance()->stopAnimation();
    LPTServerListener::getInstance()->updateUserGameData(0, 0, 0, 0, 0, PLAYER->getPlayerAuthToken(), "false", kUpdateUserOnlineStatus);
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    LPTServerListener::getInstance()->updateUserGameData(0, 0, 0, 0, 0, PLAYER->getPlayerAuthToken(), "true", kUpdateUserOnlineStatus);
    APP_MANAGER->checkInternetConnection();
#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}

void AppDelegate::applicationWillTerminate() {
    APP_MANAGER->appEndTime = GameUtils::getCurrentTime();
    LPTEvents::logUserSessionLength();
    AppWarp::Client *warpClientRef;
    warpClientRef = AppWarp::Client::getInstance();
    if (PLAYER->isAdmin) {
        warpClientRef->deleteRoom(PLAYER->getRoomId());
    }else {
        warpClientRef->leaveRoom(PLAYER->getRoomId());
    }
    warpClientRef->disconnect();
}

void AppDelegate::setUpSearchPathAndResources(cocos2d::Director *director)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
      GameUtils::isAndroid = true;
#endif
    
    auto glview     = director->getOpenGLView();
    auto frameSize  = glview->getFrameSize();
    std::vector<std::string> searchPath;
    float ratio = frameSize.width / frameSize.height;
    CCLOG("%f --- %f", frameSize.width, frameSize.height);
    if(ratio >= 2.0) {
        GameUtils::isIpad = false;
        GameUtils::isIphone = false;
        GameUtils::isIphoneX = true;
        searchPath.push_back("iPhoneX"); //
        glview->setDesignResolutionSize(kWidthiPhoneX, kHeightiPhoneX, ResolutionPolicy::EXACT_FIT);
    }else if (ratio >= 1.33 && ratio < 1.5) {
        GameUtils::isIpad = true;
        GameUtils::isIphone = false;
        GameUtils::isIphoneX = false;
        searchPath.push_back("iPad"); //bgs 1024x768
        glview->setDesignResolutionSize(kWidthiPad, kHeightiPad, ResolutionPolicy::EXACT_FIT);
    }
    else if (ratio >= 1.5 && ratio < 1.7)
    {
        GameUtils::isIphone = true;
        GameUtils::isIpad = false;
        GameUtils::isIphoneX = false;
        searchPath.push_back("iPhoneHD");//bgs 960x640
        glview->setDesignResolutionSize(kWidthiPhoneHD, kHeightiPhoneHD, ResolutionPolicy::EXACT_FIT);
    }
    else
    {
        GameUtils::isIpad = false;
        GameUtils::isIphone = true;
        GameUtils::isIphoneX = false;
        searchPath.push_back("iPhone5");  //bgs 1136x640
        glview->setDesignResolutionSize(kWidthiPhone5, kHeightiPhone5, ResolutionPolicy::EXACT_FIT);
    }
    director->setContentScaleFactor(1);

    searchPath.push_back("Common");
    searchPath.push_back("Common/GamePlay");
    searchPath.push_back("Common/MainMenuLoginScene");
    searchPath.push_back("Common/GameSelectionScene");
    searchPath.push_back("Common/PlayerProfile");
    searchPath.push_back("Common/GameResult");
    searchPath.push_back("Common/MatchMaking");
    searchPath.push_back("Common/Avatars");
    searchPath.push_back("Common/Store");
    searchPath.push_back("Common/Setting");
    searchPath.push_back("Common/FriendSearch");
    searchPath.push_back("Common/Tutorial");
    searchPath.push_back("Common/XP_BAR");
    searchPath.push_back("Data");
    searchPath.push_back("fonts");
    searchPath.push_back("sounds");
    FileUtils::getInstance()->setSearchResolutionsOrder(searchPath);
}
