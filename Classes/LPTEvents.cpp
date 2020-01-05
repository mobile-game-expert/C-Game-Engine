//
//  LPTEvents.cpp
//  LePasseTrappe-mobile
//
//  Created by Davinder Singh on 28/02/19.
//

#include "LPTEvents.hpp"

// User Events
void LPTEvents::logAppOpenTime(){
    struct tm *appOpenTime = APP_MANAGER->appStartTime;
    __String strTime = StringUtils::format("%d:%d:%d %s",appOpenTime->tm_hour,appOpenTime->tm_min,appOpenTime->tm_sec,appOpenTime->tm_zone);
    log("=== %s",strTime.getCString());
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserEnteryTime;
    valueMap[keyFIREventData] = strTime.getCString();
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logUserLocation(){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserLocation;
    valueMap[keyFIREventData] = UserDefault::getInstance()->getStringForKey(keyLPTPlayerCountryArea,"");
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logUserAge(string age){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserAge;
    valueMap[keyFIREventData] = StringUtils::format("Age : %s",age.c_str());
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

// user Session Events
void LPTEvents::logUserSessionLength() {
    struct tm *sessionTime = GameUtils::getDiffrenceBetweenTimes(APP_MANAGER->appStartTime, APP_MANAGER->appEndTime);
    __String strTime = StringUtils::format("%d:%d:%d",sessionTime->tm_hour,sessionTime->tm_min,sessionTime->tm_sec);
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserSessionLength;
    valueMap[keyFIREventData] = strTime.getCString();
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logUserGameLength(bool isWon, string timeTaken){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserGameLength;
    valueMap[keyFIREventData] = StringUtils::format("%s Time = %s",(isWon?"Win":"Loose"),timeTaken.c_str());
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logNumberOfSPG(int noOfgames){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserTotal_SPG_In_Session;
    valueMap[keyFIREventData] = StringUtils::format("%d",noOfgames);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logNumberOfMPG(int noOfgames){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserTotal_MPG_In_Session;
    valueMap[keyFIREventData] = StringUtils::format("%d",noOfgames);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logUserLevel(int level){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserLevel;
    valueMap[keyFIREventData] = StringUtils::format("%d",level);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logUserXP(int userXP){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserXP;
    valueMap[keyFIREventData] = StringUtils::format("%d",userXP);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logUserTotalSpentsPerSession(float totalSpentsInSession){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserSpentsPerSession;
    valueMap[keyFIREventData] = StringUtils::format("%f",totalSpentsInSession);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logUserTotalSpents(float totalSpents){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserTotalSpentsIAP;
    valueMap[keyFIREventData] = StringUtils::format("%f",totalSpents);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logIAPPackPurchased(float packPrice){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserPurchaseIAP;
    valueMap[keyFIREventData] = StringUtils::format("%f",packPrice);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

// User Game events
void LPTEvents::logTableSelection(int tableNo){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserTableSelected;
    valueMap[keyFIREventData] = StringUtils::format("Table %d Selected",tableNo);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logTableWonByUser(int tableNo){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserTableWin;
    valueMap[keyFIREventData] = StringUtils::format("Table %d Won",tableNo);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logTableLooseByUser(int tableNo){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserTableLoose;
    valueMap[keyFIREventData] = StringUtils::format("Table %d Loose",tableNo);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logUserShotTime(float time){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserTimeTakenToPlayNextShot;
    valueMap[keyFIREventData] = StringUtils::format("%f",time);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logUserShotSource(){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserShotSource;
    valueMap[keyFIREventData] = "";
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}

void LPTEvents::logNumberOfTimeStrikerStruckInJaw(int occurrences){
    ValueMap valueMap;
    valueMap[keyFIREventName] = kUserShotGotStruckInJaw;
    valueMap[keyFIREventData] = StringUtils::format("%d",occurrences);
    Value parameters = Value(valueMap);
    sendMessageWithParams("logEventWithName", parameters);
}
