//
//  LPTStoreItemComponent.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 05/10/18.
//

#ifndef LPTStoreItemComponent_hpp
#define LPTStoreItemComponent_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTStoreItemComponent : public BaseSprite {
public:
    CREATE_FUNC(LPTStoreItemComponent);
    static LPTStoreItemComponent * createStoreItemComponent(Vec2 position, int tag);
    virtual bool initWithSpriteName(Vec2 position, int tag);
    void setStoreItemsUI(StoreItem item, std::function<void(Ref*, Widget::TouchEventType)> callback, int buyButtonTag);
    virtual void onEnter();
    virtual void onExit();
    
private:
protected:
};

#endif /* LPTStoreItemComponent_hpp */
