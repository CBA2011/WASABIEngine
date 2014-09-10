/********************************************************************************
**
** [W]ASABI [A]ffect [S]imulation [A]rchitecture for [B]elievable [I]nteractivity
**
** Copyright (C) 2011 Christian Becker-Asano.
** All rights reserved.
** Contact: Christian Becker-Asano (christian@becker-asano.de)
**
** This file is part of the WASABIEngine library.
**
** The WASABIEngine library is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** The WASABIEngine library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with the WASABIEngine library.  If not, see <http://www.gnu.org/licenses/>
**
********************************************************************************/

#ifndef WASABIENGINE_H_
#define WASABIENGINE_H_
#include "WASABIEngine_global.h"
#include "cogaEmotionalAttendee.h"
#include <stdio.h>
#include <iostream>

class WASABIENGINESHARED_EXPORT WASABIEngine {
private:
    // the number of IDs also shows the number of Attendees, because the IDs start from 1 here.
    int nextID;
    int currentAgentsCounter;
    int getNextID();

    std::vector<cogaEmotionalAttendee*> attendeesToRemove;

public:
    int MaxSimulations;
    WASABIEngine(std::string emotionclass = "primary");
    virtual ~WASABIEngine();
    void initClass();
    cogaEmotionalAttendee* getEAfromID(int uid = 1);
    cogaEmotionalAttendee* getEAfromID(std::string globalID);
    //cogaEmotionalAttendee* ownEmoAttendee;
    std::vector< cogaEmotionalAttendee* > emoAttendees;
    std::string Class;
    // loading the files <filebasename>[.emo_dyn|.emo_pad], i.e. init.emo_dyn and init.emo_pad
    //bool loadAffectiveStates(std::string filebasename = "init", int uid = 1);
    void initAllEAs();
    bool initEA(cogaEmotionalAttendee* ea);
    bool update();
    bool getPADString(std::string& padString, int uid = 1);
    bool emotionalImpulse(int impulse, int uid = 1);
    bool resetToZero(int uid = 1);
    bool setXForce(int value, int uid = 1);
    bool setYForce(int value, int uid = 1);
    bool setSlope(int value, int uid = 1);
    bool setMass(int value, int uid = 1);
    bool setUpdateRate(int value, int uid = 1);
    bool setAlpha(int value, int uid = 1);
    bool setBeta(int value, int uid = 1);
    bool setFactor(int value, int uid = 1);
    void setMaxSimulations(int max);
    //returns the localID
    int addEmotionalAttendee(std::string name, std::string globalID = "undef");
    bool removeAttendee(std::string globalId);
    int removeAllAttendeesOf(std::string owner);
};


#endif /* WASABIENGINE_H_ */
