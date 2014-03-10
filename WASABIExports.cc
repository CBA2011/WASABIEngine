#include "WASABIExports.h"

//bool locked = false;

void checkWasabiInstance(){
    if(wasabi == NULL){
        wasabi = new WASABIEngine("secondary");
        wasabi->setMaxSimulations(1000);
    }
}

bool instanceExists(){
    if(wasabi == NULL){
        return false;
    }
    else{
        return true;
    }
}

int addAgent(char* globalId){
    checkWasabiInstance();

//    if(!locked)
    if(!mutex.trylock()){
//        locked = true;
        int localId = wasabi->addEmotionalAttendee(globalId, globalId);
        cogaEmotionalAttendee* attendee = wasabi->getEAfromID(localId);
        bool agentInitialized = wasabi->initEA(attendee);
        if(agentInitialized){
//            locked = false;
            mutex.unlock();
            return localId;
        }
        else{
//            locked = false;
            mutex.unlock();
            return -1;
        }
    }
    else{
        return -1;
    }
}

char* getPad(int localId){
    checkWasabiInstance();

    std::string padString;
    wasabi->getPADString(padString, localId);

    char* returnValue = new char[padString.length()];
    strcpy(returnValue, padString.c_str());

    return returnValue;
}

bool isInitialized(int localId){
    cogaEmotionalAttendee* attendee = wasabi->getEAfromID(localId);
    return  attendee->initialized;
}

void updateWasabi(){
    checkWasabiInstance();

//    if(!locked){
      if(!mutex.trylock()){
//        locked = true;
        wasabi->update();
//        locked = false;
        mutex.unlock();
    }
}

//TODO add boolean return value
void removeAttendee(int localId){
//    if(!locked){
      if(!mutex.trylock()){
//        locked = true;
        wasabi->removeAttendee(localId);
//        locked = false;
        mutex.unlock();
    }
}

//TODO return integer return value
void removeAllAttendees(){
    checkWasabiInstance();
//    if(!locked){
    if(!mutex.trylock()){
//        locked = true;
        wasabi->removeAllAttendees();
        mutex.unlock();
//        locked = false;
    }
}

void onDestroy(){
    delete(wasabi);
    wasabi = NULL;
}
