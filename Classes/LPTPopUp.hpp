//
//  LPTPopUp.hpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 27/09/18.
//

#ifndef LPTPopUp_hpp
#define LPTPopUp_hpp

#include <stdio.h>
#include "LPTHeaders.h"

class LPTPopUpDelegate {
public:
    virtual void okButtonAction(Popup_Type type) = 0;
    virtual void cancelButtonAction(Popup_Type type) = 0;
};

class LPTPopUp : public BaseLayer{
public:
    CREATE_FUNC(LPTPopUp);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    static LPTPopUp* showPopUpWith(string title, string msg, bool areTwoButtonPopUp, Popup_Type type,string okTitle = "OK",string cancelTitle = "Cancel");
    void setPopUpDelegate(LPTPopUpDelegate *delegate);
private:
    LPTPopUpDelegate *delegate;
    void setUpUI(LPTPopUp *popUp,string title, string msg, bool areTwoButtonPopUp, string okTitle , string cancelTitle);
    void buttonCallback(Ref* sender, Widget::TouchEventType type);

protected:
};

#endif /* LPTPopUp_hpp */
