//
//  LPTAvatarSelectionLayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 16/10/18.
//

#ifndef LPTAvatarSelectionLayer_hpp
#define LPTAvatarSelectionLayer_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "extensions/cocos-ext.h"
#include <ui/UIScrollView.h>
#include "LPTPopUp.hpp"
#include "LPTServerListener.hpp"

class LPTAvatarSelectionLayerDelegate {
public:
    virtual void profilePictureUpdated(string imageName) = 0;
};

using namespace cocos2d::extension;
class LPTAvatarSelectionLayer : public BaseLayer , public LPTPopUpDelegate, public LPTServerDelegate {
public:
    CREATE_FUNC(LPTAvatarSelectionLayer);
    static LPTAvatarSelectionLayer* createLayer();
    virtual bool initWithTag();
    virtual void onEnter();
    virtual void onExit();
    void setAvatarDelegate(LPTAvatarSelectionLayerDelegate* delegate);
    LPTAvatarSelectionLayerDelegate *avatarDelegate;
    
    void serverResponseComplete(int tag);
    void serverListenerResponseFaliure(int tag, string message);
    void serverResponseCompletedWithData(picojson::array data);
private:
   
    BaseSprite *avatarBaseSprite;
    ui::ScrollView *avatarScrollView;
    Size avatarCellSize;
    string selectedAvatar, previousAvatar;
    Button *doneButton , *cancelButton;
    void setupUI();
    void setupScrollView();
    void createAvatarsUI();
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void okButtonAction(Popup_Type type);
    void cancelButtonAction(Popup_Type type);
    void updateUserDocumentForProfilePic();
    void updateUserDocumentForProfilePicResponse(void *response);
    void addSelectionGlowOnAvatar(int selectedIndex);
};
#endif /* LPTAvatarSelectionLayer_hpp */
