//
//  LPTPlayerProfileLayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 01/10/18.
//

#ifndef LPTPlayerProfileLayer_hpp
#define LPTPlayerProfileLayer_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "LPTAvatarSelectionLayer.hpp"
#include "LPTPlayerProfileData.hpp"

class LPTPlayerProfileLayer : public BaseLayer, public LPTAvatarSelectionLayerDelegate {
public:
    //
    int backButtonTag = 0;
    LPTPlayerProfileData *player;
    //
    CREATE_FUNC(LPTPlayerProfileLayer);
    static LPTPlayerProfileLayer *createLayer(int tag, LPTPlayerProfileData *player);
    virtual bool initWithTag(int buttonTag, LPTPlayerProfileData *player);
    virtual void onEnter();
    virtual void onExit();
    void addTopBarInUserProfileScene(int tag);
    void addUserProfileBase();
    void addPlayerProfile(string imageName, Vec2 position, Vec2 anchorPoint, int tag, BaseSprite *parent);
    void addCoinsDescription(BaseSprite *parent, Vec2 position);
    void addPlayerProfileNameBar(BaseSprite *parent);
    void addPlayerStats(BaseSprite *parent);
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    virtual void profilePictureUpdated(string imageName);
    void disableEditButtton();
    void addPlayerXP(double playerCurrentXP, double playerTotalXP);
private:
};
#endif /* LPTPlayerProfileLayer_hpp */
