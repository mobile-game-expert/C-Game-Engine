//
//  LPTHorizontalScrollView.cpp
//  LePasseTrappe-mobile
//
//  Created by Vinod on 27/09/18.
//

#include "LPTHorizontalScrollView.hpp"

#define     kMaxTableSelectionCell                      8
#define     kEmptyCellCount                             2
#define     kCellImageTag                               9000
#define     kCellSelectedIndicatorTag                   9001  //9004


LPTHorizontalScrollView* LPTHorizontalScrollView::createLayer() {
    auto layer = LPTHorizontalScrollView::create();
    return layer;
}

bool LPTHorizontalScrollView::init() {
    if (!BaseLayer::init()) {
        return false;
    }
    return true;
}

void LPTHorizontalScrollView::onEnter() {
    BaseLayer::onEnter();
}

/**
 setupUI : This method will check if player has won or not.
 - returns : PLAYER_STATE
 */
void LPTHorizontalScrollView::setupUI(vector<ValueMap> cellData, Vec2 position, Size contentSize) {
    gameSelectionImagesName = cellData;
    maxNumberOfCellIndicator = gameSelectionImagesName.size();
    this->setPosition(position);
    this->setContentSize(contentSize);
    
    LayerColor *layerColor = LayerColor::create(Color4B(255, 255, 255, 0), contentSize.width, contentSize.height);
    this->addChild(layerColor);
    this->addSelectedIndicator();
    this->addTableView();
}
#pragma mark AddTableView

void LPTHorizontalScrollView::addTableView() {
    gameSelectionTableView = TableView::create(this, Size(this->getContentSize().width, this->getContentSize().height * 0.9));
    gameSelectionTableView->setPosition(Vec2(0, this->getContentSize().height * 0.1));
    gameSelectionTableView->setDelegate(this);
    gameSelectionTableView->setDirection(ScrollView::Direction::HORIZONTAL);
    gameSelectionTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    gameSelectionTableView->setBounceable(false);
    this->addChild(gameSelectionTableView);
}
#pragma mark SetDelegate
void LPTHorizontalScrollView::setSelectionLayerDelegate(LPTHorizontalScrollViewDelegate *delegate) {
    this->selectionLayerDelegate = delegate;
}

#pragma mark CreateSelectedTableIndicator
void LPTHorizontalScrollView::addSelectedIndicator() {
    int startingIndexTag        =   kCellSelectedIndicatorTag;
    float startingPositionInX   =   maxNumberOfCellIndicator == kMaxTableSelectionCell ? this->getContentSize().width * 0.325 : this->getContentSize().width * 0.45;
    
    for(int cellIndicatorCounter = 1; cellIndicatorCounter <= maxNumberOfCellIndicator; cellIndicatorCounter++) {
        
        BaseLayer::createSprite(kInActiveSelection, startingIndexTag + cellIndicatorCounter, Vec2(startingPositionInX, this->getContentSize().height * 0.07), this);
        
        startingPositionInX     =   startingPositionInX + this->getContentSize().width * 0.05;
    }
}

void LPTHorizontalScrollView::changeSelectedStatusOfIndicator(int currentSelected) {
    for(int cellIndicatorCounter = 1; cellIndicatorCounter <= maxNumberOfCellIndicator; cellIndicatorCounter++) {
        BaseSprite *cellSelectedIndicator = (BaseSprite *) this->getChildByTag(kCellSelectedIndicatorTag + cellIndicatorCounter);
        if((kCellSelectedIndicatorTag + currentSelected) == (kCellSelectedIndicatorTag + cellIndicatorCounter)) {
            Sprite *changedTexture = Sprite::create(kActiveSelection);
            cellSelectedIndicator->setTexture(changedTexture->getTexture());
        }else {
            Sprite *changedTexture = Sprite::create(kInActiveSelection);
            cellSelectedIndicator->setTexture(changedTexture->getTexture());
        }
    }
}

#pragma mark TableViewDelegateMethod
void LPTHorizontalScrollView::scrollViewDidScroll(extension::ScrollView *view) {
    for(int counter = 1; counter < (gameSelectionImagesName.size() - 1 + kEmptyCellCount); counter++) {
        LPTGameSelectionCell *cell = (LPTGameSelectionCell *)gameSelectionTableView->cellAtIndex(counter);
        if (cell != NULL) {
            Size tableViewSize        =   this->cellSizeForTable(gameSelectionTableView);
            if (((cell->getPosition().x + view->getContentOffset().x) >  this->getContentSize().width * 0.2) && ((cell->getPosition().x + view->getContentOffset().x) <  this->getContentSize().width * 0.6)) {
                cell->ticketSprite->setScale(tableViewSize.width * 1.15 / cell->ticketSprite->getContentSize().width);
                currentSelectedTab = counter;
                this->changeSelectedStatusOfIndicator(currentSelectedTab);
            }else {
                cell->ticketSprite->setScale(tableViewSize.width * 0.9 / cell->ticketSprite->getContentSize().width);
            }
        }
    }
}


Size LPTHorizontalScrollView::tableCellSizeForIndex(TableView* table, ssize_t idx) {
    if (idx == 0 || idx == (gameSelectionImagesName.size() - 1 + kEmptyCellCount)) {
        return Size(this->getContentSize().width * 0.3, this->getContentSize().height);
    }
    return Size(this->getContentSize().width * 0.4, this->getContentSize().height);
}

Size LPTHorizontalScrollView::cellSizeForTable(TableView *table) {
    return Size(this->getContentSize().width * 0.4, this->getContentSize().height);
}

TableViewCell* LPTHorizontalScrollView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    LPTGameSelectionCell *tableViewCell = (LPTGameSelectionCell *)table->dequeueCell();
    if (tableViewCell == NULL) {
        tableViewCell = new LPTGameSelectionCell();
    }else {
        tableViewCell->removeAllChildrenWithCleanup(true);
    }
    
    if (idx != 0 && idx != (gameSelectionImagesName.size() - 1 + kEmptyCellCount)) {
        Size cellSize               =   this->cellSizeForTable(table);
        ValueMap cellData            =   gameSelectionImagesName[idx - 1];
        tableViewCell->initWithSpriteName(cellData, cellSize, kCellImageTag);
    }
    return tableViewCell;
}

ssize_t LPTHorizontalScrollView::numberOfCellsInTableView(TableView *table)
{
    return (gameSelectionImagesName.size() + kEmptyCellCount);
}

void LPTHorizontalScrollView::tableCellTouched(TableView* table, TableViewCell* cell)
{
    if(cell->getIdx() == currentSelectedTab && this->selectionLayerDelegate != NULL) {
        this->selectionLayerDelegate->tableCellSelectedWithIndex(currentSelectedTab);
    }
}

void LPTHorizontalScrollView::onExit() {
    BaseLayer::onExit();
}
