//
//  LPTServerDelegate.hpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 06/12/18.
//

#ifndef LPTServerDelegate_hpp
#define LPTServerDelegate_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "picojson.h"
using namespace cocos2d;
using namespace std;


class LPTServerDelegate {
public:
    virtual void serverResponseComplete(int tag) = 0;
    virtual void serverListenerResponseFaliure(int tag, string message) = 0;
    virtual void serverResponseCompletedWithData(picojson::array data) = 0;
};

#endif /* LPTServerDelegate_hpp */
