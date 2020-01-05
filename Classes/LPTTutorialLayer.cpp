//
//  LPTTutorialLayer.cpp
//  LePasseTrappe-mobile
//
//  Created by Pragati Thakur on 11/10/18.
//

#include "LPTTutorialLayer.hpp"
#include "LPTTopBar.hpp"
#include "LPTSettingsScene.hpp"

#define TOP_BAR_TAG             8001
#define LEFT_BUTTON_TAG         8003
#define RIGHT_BUTTON_TAG        8004
#define INDICATOR_BASE_TAG      8100
#define TUTORIAL_BASE_TAG       8200

#define kTutorialStartIndex     0
#define kTutorialEndIndex       2

#define kLeftButtonRotation     180
#define kButtonActiveOpacity    255
#define kButtonInactiveOpacity  150

#define kInstruction1           "Use the Bungee to launch the disks on your side through the gate"
#define kInstruction2           "You may move the disks to side if they are in the way, but\ncannot move them forward"
#define kInstruction3           "First one to clear their side from all disks wins"

//
LPTTutorialLayer* LPTTutorialLayer::createLayer(int backButtonTag) {
    auto layer = LPTTutorialLayer::create();
    layer->initWithTag(backButtonTag);
    return layer;
}

bool LPTTutorialLayer::initWithTag(int tag) {
    if (!BaseLayer::init()) {
        return false;
    }
    APP_MANAGER->gameState = TUTORIAL_SCENE;
    backButtonTag = tag;
    setBackgroundSprite(kGamePlayBG);
    LayerColor *layerColor = LayerColor::create(Color4B(16, 6, 8, 190), this->getContentSize().width, this->getContentSize().height);
    this->addChild(layerColor);
    return true;
}

void LPTTutorialLayer::onEnter() {
    BaseLayer::onEnter();
    setupTutorialData();
    setupUI();
}

void LPTTutorialLayer::onExit() {
    BaseLayer::onExit();
}
#pragma mark - Setup data
void LPTTutorialLayer::setupTutorialData() {
    arrayInstruction.push_back(kInstruction1);
    arrayInstruction.push_back(kInstruction2);
    arrayInstruction.push_back(kInstruction3);
}

#pragma mark - SetupUI
void LPTTutorialLayer::setupUI() {
    //TopBar
    LPTTopBar *layer = GameUtils::createTopBar(this, kUserProfileTopBarImageName);
    layer->setTag(TOP_BAR_TAG);
    layer->addUserProfileTopBar(backButtonTag, kTutorialSceneTitle, CC_CALLBACK_2(LPTTutorialLayer::buttonCallback, this));
    
    //Left Side Button
    auto leftButton = createButton(kMusicOnOffButtonImageName, LEFT_BUTTON_TAG, Vec2(this->getContentSize().width*0.05, (this->getContentSize().height- layer->getContentSize().height) * 0.5), this, CC_CALLBACK_2(LPTTutorialLayer::buttonCallback, this), 1);
    leftButton->setOpacity(kButtonInactiveOpacity);
    leftButton->setRotation(180);
    //Right Side Button
    createButton(kMusicOnOffButtonImageName, RIGHT_BUTTON_TAG, Vec2(this->getContentSize().width*0.95, (this->getContentSize().height- layer->getContentSize().height) * 0.5), this, CC_CALLBACK_2(LPTTutorialLayer::buttonCallback, this), 1);
    
    //Tutorial data
    addSelectedIndicator();
    addPageViewForTutorial(Vec2(this->getContentSize().width*0.1, (this->getContentSize().height - layer->getContentSize().height) * 0.1), Size(this->getContentSize().width *0.8, (this->getContentSize().height - layer->getContentSize().height) * 0.8));
}

#pragma mark - Setup Button Callback
void LPTTutorialLayer::buttonCallback(Ref* sender, Widget::TouchEventType type) {
    Button *button = (Button*)sender;
    switch (type) {
        case Widget::TouchEventType::BEGAN:
            break;
        case Widget::TouchEventType::CANCELED:
            break;
        case Widget::TouchEventType::ENDED:
        {
            SoundManager::playSound(BUTTON_TAP_SOUND);
            switch (button->getTag()) {
                case kBackToSettingSceneBackButtonTag:
                    Director::getInstance()->popScene();
                    break;
                case kBackToGameSceneFromTutorialButtonTag :
                    this->removeFromParentAndCleanup(true);
                    break;
                case RIGHT_BUTTON_TAG:{
                    int currentPage = pageView->getCurrentPageIndex();
                    if(currentPage + 1 > kTutorialEndIndex) {
                        button->setOpacity(kButtonInactiveOpacity);
                    } else {
                        button->setOpacity(kButtonActiveOpacity);
                        currentPage = currentPage + 1;
                        pageView->scrollToItem(currentPage);
                        if(currentPage + 1 > kTutorialEndIndex) {
                            button->setOpacity(kButtonInactiveOpacity);
                        }
                    }
                    Button *btnLeft = (Button*)this->getChildByTag(LEFT_BUTTON_TAG);
                    btnLeft->setOpacity(kButtonActiveOpacity);
                    changeSelectedStatusOfIndicator(currentPage);
                }
                    break;
                case LEFT_BUTTON_TAG:{
                    int currentPage = pageView->getCurrentPageIndex();
                    if(currentPage - 1 < kTutorialStartIndex) {
                        button->setOpacity(kButtonInactiveOpacity);
                    } else {
                        button->setOpacity(kButtonActiveOpacity);
                        currentPage = currentPage - 1;
                        pageView->scrollToItem(currentPage);
                        if(currentPage - 1 < kTutorialStartIndex) {
                            button->setOpacity(kButtonInactiveOpacity);
                        }
                    }
                    Button *btnRight = (Button*)this->getChildByTag(RIGHT_BUTTON_TAG);
                    btnRight->setOpacity(kButtonActiveOpacity);
                    changeSelectedStatusOfIndicator(currentPage);
                }
                    break;
                default:
                    break;
            }
        }
            break;
        case Widget::TouchEventType::MOVED:
            break;
    }
}


#pragma mark CreateSelectedTableIndicator
void LPTTutorialLayer::addSelectedIndicator() {
    int startingIndexTag        =   INDICATOR_BASE_TAG;
    float startingPositionInX   =   this->getContentSize().width * 0.45;
    for(int cellIndicatorCounter = 0; cellIndicatorCounter <= kTutorialEndIndex; cellIndicatorCounter++) {
        BaseSprite *ticketSprite = BaseLayer::createSprite(kInActiveSelection, startingIndexTag + cellIndicatorCounter, Vec2(startingPositionInX, this->getContentSize().height * 0.05), this);
        startingPositionInX     =   startingPositionInX + this->getContentSize().width * 0.05;
    }
}

void LPTTutorialLayer::changeSelectedStatusOfIndicator(int currentSelected) {
    int startingIndexTag        =   INDICATOR_BASE_TAG;
    for(int cellIndicatorCounter = 0; cellIndicatorCounter <= kTutorialEndIndex; cellIndicatorCounter++) {
        BaseSprite *cellSelectedIndicator = (BaseSprite *) this->getChildByTag(startingIndexTag + cellIndicatorCounter);
        if((startingIndexTag + cellIndicatorCounter) == (INDICATOR_BASE_TAG + currentSelected)) {
            Sprite *changedTexture = Sprite::create(kActiveSelection);
            cellSelectedIndicator->setTexture(changedTexture->getTexture());
        }else {
            Sprite *changedTexture = Sprite::create(kInActiveSelection);
            cellSelectedIndicator->setTexture(changedTexture->getTexture());
        }
    }
}

#pragma mark - Pageview setup
void LPTTutorialLayer::addPageViewForTutorial(Vec2 position, Size pageViewSize) {
    // Create the page view
    
    pageView = PageView::create();
    pageView->setPosition(position);
    pageView->setDirection(PageView::Direction::HORIZONTAL);
    pageView->setContentSize(pageViewSize);
    pageView->setBounceEnabled(false); //Disable circular pageview
    pageView->setBackGroundColor(Color3B::WHITE);
    
    pageView->setItemsMargin(0);
    pageView->removeAllItems();
    pageView->setIndicatorEnabled(false);
    this->addChild(pageView);
    
    pageView->addEventListener([=](Ref* sender,PageView::EventType type) {
        this->changeSelectedStatusOfIndicator((int)pageView->getCurrentPageIndex());
        Button *btnLeft = (Button*)this->getChildByTag(LEFT_BUTTON_TAG);
        Button *btnRight = (Button*)this->getChildByTag(RIGHT_BUTTON_TAG);
        if(pageView->getCurrentPageIndex() == kTutorialStartIndex) {
            btnLeft->setOpacity(kButtonInactiveOpacity);
            btnRight->setOpacity(kButtonActiveOpacity);
        } else  if(pageView->getCurrentPageIndex() == kTutorialEndIndex) {
            btnLeft->setOpacity(kButtonActiveOpacity);
            btnRight->setOpacity(kButtonInactiveOpacity);
        } else {
            btnLeft->setOpacity(kButtonActiveOpacity);
            btnRight->setOpacity(kButtonActiveOpacity);
        }
    });
    
    for (int pageIndex = kTutorialStartIndex; pageIndex <= kTutorialEndIndex; pageIndex++)
    {
        Layout* layout = Layout::create();
        layout->setContentSize(pageViewSize);
//        LayerColor *layerColor = LayerColor::create(Color4B::BLUE, this->getContentSize().width, this->getContentSize().height);
//        layout->addChild(layerColor);
        auto lblInstruction = createTTFLabel(arrayInstruction.at(pageIndex), Vec2(layout->getContentSize().width * 0.5,layout->getContentSize().height*0.075), Vec2::ANCHOR_MIDDLE, layout, Color3B::WHITE, kOpenSans_Bold, 24);
        lblInstruction->setAlignment(TextHAlignment::CENTER);
        
        auto tutorialSprite = createSprite(StringUtils::format("%s%d.png",kTutorialImageName, pageIndex+1), TUTORIAL_BASE_TAG + pageIndex, Vec2(layout->getContentSize().width*0.5, layout->getContentSize().height - ((layout->getContentSize().height - lblInstruction->getContentSize().height)*0.5)), layout);
        tutorialSprite->setScale((layout->getContentSize().height - lblInstruction->getContentSize().height) * 0.95/ tutorialSprite->getContentSize().height);
        
        pageView->addPage(layout);
    }
    pageView->scrollToItem(0);
}
