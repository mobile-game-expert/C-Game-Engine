//
//  LPTStoreLayer.hpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 08/10/18.
//

#ifndef LPTStoreLayer_hpp
#define LPTStoreLayer_hpp

#include <stdio.h>
#include "LPTHeaders.h"
#include "extensions/cocos-ext.h"
#include <ui/UIScrollView.h>
#include "LPTServerListener.hpp"

using namespace cocos2d::extension;
class LPTStoreLayer : public BaseLayer, public LPTPopUpDelegate, public sdkbox::IAPListener, public LPTServerDelegate {
public:
    CREATE_FUNC(LPTStoreLayer);
    static LPTStoreLayer* createLayer(int backButtonTag);
    virtual bool initWithTag(int backButtonTag);
    void onEnter();
    void onExit();
    //Popup callback
    virtual void okButtonAction(Popup_Type type);
    virtual void cancelButtonAction(Popup_Type type);
private:
    int backButtonTag = 0;
    vector<StoreItem> arrayStoreItems;
    ui::ScrollView *scrollViewStoreItem;
    BaseSprite *storeItemBaseSprite;
    void getStoreDataFromPlist();
    void setupUI();
    void setupScrollView();
    void createStoreItemsUI();
    void buttonCallback(Ref* sender, Widget::TouchEventType type);
    void updateUserDocumentForCoins();
    void updateUserDocumentForCoinsResponse(void *response);
    //IAP Methods
    virtual void onInitialized(bool ok) {};
    virtual void onSuccess(sdkbox::Product const& p) override;
    virtual void onFailure(sdkbox::Product const& p, const std::string &msg) override;
    virtual void onCanceled(sdkbox::Product const& p) override;
    virtual void onRestored(sdkbox::Product const& p) {};
    virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) {};
    virtual void onProductRequestFailure(const std::string &msg) {};
    virtual void onRestoreComplete(bool ok, const std::string &msg) {};
    
    void serverResponseComplete(int tag);
    void serverListenerResponseFaliure(int tag, string message);
    void serverResponseCompletedWithData(picojson::array data);
protected:
};
#endif /* LPTStoreLayer_hpp */
