//
//  LPTSettingsScene.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 17/09/18.
//

#ifndef LPTSettingsScene_hpp
#define LPTSettingsScene_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTSettingsScene : public BaseScene {
public:
    CREATE_FUNC(LPTSettingsScene);
    static LPTSettingsScene *createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
private:

protected:
};
#endif /* LPTSettingsScene_hpp */
