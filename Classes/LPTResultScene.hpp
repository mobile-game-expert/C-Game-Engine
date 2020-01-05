//
//  LPTResultScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Apple on 14/09/18.
//

#ifndef LPTResultScene_hpp
#define LPTResultScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTResultScene : public BaseLayer {
public:
    CREATE_FUNC(LPTResultScene);
    static BaseScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
private:
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
protected:
};

#endif /* LPTResultScene_hpp */
