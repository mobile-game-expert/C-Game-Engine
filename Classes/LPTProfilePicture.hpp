//
//  LPTProfilePicture.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 18/09/18.
//

#ifndef LPTProfilePicture_hpp
#define LPTProfilePicture_hpp

#include <stdio.h>
#include "LPTHeaders.h"

using namespace cocos2d;


class LPTProfilePicture : public BaseLayer {
public:
    CREATE_FUNC(LPTProfilePicture);
    static LPTProfilePicture *createLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    ClippingNode* createProfilePictureView(string userImage, string stencilImage, Vec2 position,int tag, bool intractionEnable,  std::function<void(Ref*, Widget::TouchEventType)> callback = NULL);
    void updatePicture(string image, int tag);
    void updatePictureWithSpriteFrame(SpriteFrame *image, int tag);
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
};
#endif /* LPTProfilePicture_hpp */
