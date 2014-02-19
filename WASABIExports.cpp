#include "WASABIExports.h"

void checkWasabiInstance(){
    if(wasabi == NULL){
        wasabi = new WASABIEngine("secondary");
        wasabi->setMaxSimulations(1000);
    }
}

int addAgent(char* globalId){
    checkWasabiInstance();

    int localId = wasabi->addEmotionalAttendee(globalId, globalId);
    cogaEmotionalAttendee* attendee = wasabi->getEAfromID(localId);
    bool agentInitialized = wasabi->initEA(attendee);
    if(agentInitialized){
        return localId;
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

    wasabi->update();
}

//TODO add boolean return value
void removeAttendee(int localId){
    wasabi->removeAttendee(localId);
}

//TODO return integer return value
void removeAllAttendees(){
    checkWasabiInstance();

    wasabi->removeAllAttendees();
}

void onDestroy(){
    delete(wasabi);
    wasabi = NULL;
}
