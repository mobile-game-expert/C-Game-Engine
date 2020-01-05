//
//  SoundManager.h
//  ScrambleWord
//
//  Created by ChicMic on 02/06/16.
//

#ifndef SoundManager_hpp
#define SoundManager_hpp
#include <iostream>
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "GameConstants.h"
using namespace CocosDenshion;
using namespace cocos2d;
using namespace std;
class SoundManager{
public:
    static void preloadEffects();
    static SimpleAudioEngine *soundEngine;
    
    //SOUND
    static int playSound(string sound ,float volume = 1.0 , bool loop = false);
    static void stopSounds();
    static void stopSoundById(int id);
    static void setSoundStatus(bool isPlaying);
    static bool getSoundStatus();
    static bool isBackgroundMusicPlaying();
    
    //MUSIC
    static void preloadBGMusic();
    static bool shouldPlayBackgroundMusic();
    
    static void playMusic(string bgMusicFile , bool isLoop = true);
    static void pauseMusic();
    static void resumeMusic();
    static void stopMusic();
    static void setMusicStatus(bool isPlaying);
    static bool getMusicStatus();
    
//    virtual bool isBackgroundMusicPlaying();
};
#endif /* SoundManager_hpp */
