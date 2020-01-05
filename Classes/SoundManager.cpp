//
//  SoundManager.cpp
//  ScrambleWord
//
//  Created by ChicMic on 02/06/16.

#include "SoundManager.h"
#include "SimpleAudioEngine.h"
#include "GameUtils.hpp"


using namespace CocosDenshion;
SimpleAudioEngine* SoundManager::soundEngine = NULL;


#define     kBackgroundMusicKey         "background_Music_Status"
#define     kSoundKey                   "sound_Status"


void SoundManager::preloadEffects(){
    SimpleAudioEngine::getInstance()->preloadEffect(COINS_SOUND);
    SimpleAudioEngine::getInstance()->preloadEffect(BUTTON_TAP_SOUND);
    SimpleAudioEngine::getInstance()->preloadEffect(POPUP_SOUND);
    SimpleAudioEngine::getInstance()->preloadEffect(SCENE_CHANGE_SOUND);
    SimpleAudioEngine::getInstance()->preloadEffect(WIN_SOUND);
    SimpleAudioEngine::getInstance()->preloadEffect(LOOSE_SOUND);
    SimpleAudioEngine::getInstance()->preloadEffect(STRIKER_WOODEN_SOUND);
    SimpleAudioEngine::getInstance()->preloadEffect(STRIKER_STRIKER_SOUND);
}

void SoundManager::preloadBGMusic(){
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GAME_BACKGROUND_MUSIC);
}

#pragma Music Sound Methods
bool SoundManager::shouldPlayBackgroundMusic() {
    
}

#pragma mark - Check music playing
bool SoundManager::isBackgroundMusicPlaying() {
    return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}

//bool isBackgroundMusicPlaying() {
//
//}
#pragma mark MUSIC_STAT
void SoundManager::playMusic(string bgMusicFile ,  bool isLoop) {
    if (soundEngine == NULL) {
        soundEngine = SimpleAudioEngine::getInstance();
    }
    if (SoundManager::getMusicStatus())
        soundEngine->playBackgroundMusic(StringUtils::format("%s" , bgMusicFile.c_str()).c_str(), isLoop);
}

void SoundManager::pauseMusic() {
    if (soundEngine == NULL) {
        soundEngine = SimpleAudioEngine::getInstance();
    }
    soundEngine->pauseBackgroundMusic();
}

void SoundManager::resumeMusic() {
    if (soundEngine == NULL) {
        soundEngine = SimpleAudioEngine::getInstance();
    }
    
    if (SoundManager::getMusicStatus())
        soundEngine->resumeBackgroundMusic();
}

void SoundManager::stopMusic(){
    if (soundEngine == NULL) {
        soundEngine = SimpleAudioEngine::getInstance();
    }
    soundEngine->stopBackgroundMusic();
}

void SoundManager::setMusicStatus(bool isPlaying) {
    UserDefault::getInstance()->setBoolForKey(kBackgroundMusicKey, isPlaying);
}

bool SoundManager::getMusicStatus() {
    bool status = UserDefault::getInstance()->getBoolForKey(kBackgroundMusicKey);
    return status;
}

#pragma mark SOUND_STAT
int SoundManager::playSound(string sound ,float volume , bool loop){
    int soundId = -1;
    if (soundEngine == NULL) {
        soundEngine = SimpleAudioEngine::getInstance();
    }
    soundEngine->setEffectsVolume(volume);
    if (SoundManager::getSoundStatus()) {
        soundId = soundEngine->playEffect(StringUtils::format("%s" , sound.c_str()).c_str() , loop);
    }
    return soundId;
}

void SoundManager::stopSounds() {
    if (soundEngine == NULL) {
        soundEngine = SimpleAudioEngine::getInstance();
    }
    soundEngine->stopAllEffects();
}

void SoundManager::stopSoundById(int id) {
    if (soundEngine == NULL) {
        soundEngine = SimpleAudioEngine::getInstance();
    }
    soundEngine->stopEffect(id);
}

void SoundManager::setSoundStatus(bool isPlaying) {
    UserDefault::getInstance()->setBoolForKey(kSoundKey, isPlaying);
}

bool SoundManager::getSoundStatus() {
    bool status = UserDefault::getInstance()->getBoolForKey(kSoundKey, true);
    return status;
}
