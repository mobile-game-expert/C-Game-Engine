//
//  LPTSettingsLayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 08/10/18.
//

#ifndef LPTSettingsLayer_hpp
#define LPTSettingsLayer_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "LPTPopUp.hpp"
#include "LPTServerListener.hpp"

class LPTSettingsLayer : public BaseLayer , public LPTPopUpDelegate, public LPTServerDelegate{
public:
    LOGOUT_TYPE gameLogoutType = kNOT_LOGGED_IN;
    //
    int backButtonTag = 0;
    bool needToShowTutorial = false;
    BaseSprite *settingBase;

    CREATE_FUNC(LPTSettingsLayer);
    static LPTSettingsLayer *createLayer(int tag, bool needToShowTutorial = true);
    virtual bool initWithTag(int buttonTag, bool showTutorial);
    virtual void onEnter();
    virtual void onExit();
    void addTopBarInUserProfileScene(int backButtonTag);
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void addSeparaterLayer();
    void addUI();
    void addSoundMenu();
    void addMusicMenu();
    void addTutorialScene();
    void addLogoutButton();
    void addSignButton();
    void changeOpacityOfAlernativeButton(int tag, int opacity);
    void addMusicAndEffectsButton(Vec2 position, int tag, bool rotationRequired, int opacity);
    void updateStatusOfMusicSoundMenu(bool status, int tag);
    void cancelButtonAction(Popup_Type type);
    void okButtonAction(Popup_Type type);
    
    //
    void serverResponseComplete(int tag);
    void serverListenerResponseFaliure(int tag, string message);
    void serverResponseCompletedWithData(picojson::array data);
    //
    void syncDataWithServer();
    void logoutFromDevice();
private:
    bool isMusicPlaying = false;
    bool isSoundPlaying = false;
};
#endif /* LPTSettingsLayer_hpp */
