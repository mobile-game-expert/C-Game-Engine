//
//  LPTGameSelectionCell.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 28/09/18.
//

#include "LPTGameSelectionCell.hpp"

#define kCoinBaseImageTag           55555
#define kCoinLabelTag               55556
#define kTableNameLabelTag          55557
bool LPTGameSelectionCell::initWithSpriteName(ValueMap cellData, Size cellSize, int tag)
{
    if (!TableViewCell::init())
    {
        return false;
    }
    string imageName = cellData[kImageName].asString();
    ticketSprite = BaseSprite::createWithFileName(imageName);
    ticketSprite->setTag(tag);
    ticketSprite->setPosition(Vec2(cellSize.width * 0.5, cellSize.height * 0.5));
    this->addChild(ticketSprite);
    ticketSprite->setScale(cellSize.width * 0.9 / ticketSprite->getContentSize().width);
    
    if(APP_MANAGER->gameState == BET_SELECTION) {
        string betCoinImageBase = cellData[kPriceImageName].asString();
        double coinValue        = cellData[kPriceLabel].asDouble();
        string tableName        = cellData[kTableName].asString();
        this->addBetSelectionComponent(betCoinImageBase, coinValue, tableName);
    }
    return true;
}

void LPTGameSelectionCell::addBetSelectionComponent(string coinBaseImageName, double coinPrice, string tableName) {
    BaseSprite *coinBaseSprite = BaseSprite::createWithFileName(coinBaseImageName);
    coinBaseSprite->setTag(kCoinBaseImageTag);
    coinBaseSprite->setPosition(Vec2(ticketSprite->getContentSize().width * 0.5, ticketSprite->getContentSize().height * 0.685));
    coinBaseSprite->setScale(ticketSprite->getContentSize().width * 0.6 / coinBaseSprite->getContentSize().width);
    ticketSprite->addChild(coinBaseSprite);
    
    //LABEL
    string coinsValue = StringUtils::format("%.0f", coinPrice);
    Label *betAmountLabel  = Label::createWithTTF(coinsValue.c_str(), kOpenSans_Bold, 50);
    betAmountLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    betAmountLabel->setPosition(Vec2(coinBaseSprite->getContentSize().width * 0.75, coinBaseSprite->getContentSize().height * 0.5));
    betAmountLabel->setColor(Color3B::WHITE);
    betAmountLabel->setTag(kCoinLabelTag);
    coinBaseSprite->addChild(betAmountLabel);
    
    
    Label *tableNameLabel  = Label::createWithTTF(tableName.c_str(), kNOUGAT_EXTRABLACK_1, 120);
    tableNameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tableNameLabel->setPosition(Vec2(ticketSprite->getContentSize().width * 0.5, ticketSprite->getContentSize().height * 0.83));
    tableNameLabel->setColor(Color3B::WHITE);
    tableNameLabel->setTag(kTableNameLabelTag);
//    ticketSprite->addChild(tableNameLabel);
    
}

