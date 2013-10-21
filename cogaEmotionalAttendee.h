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

#ifndef EMOTIONALATTENDEE_H
#define EMOTIONALATTENDEE_H

#include "EmotionDynamics.h"
#include "EmotionConverter.h"
#include "cogaAttendee.h"
#include "WASABIEngine_global.h"

class WASABIENGINESHARED_EXPORT cogaEmotionalAttendee: public cogaAttendee
{
public:

    cogaEmotionalAttendee();
    ~cogaEmotionalAttendee();
    //int pers_xPos, pers_yPos, pers_zPos, pers_dominance, pers_PValue, pers_AValue, pers_DValue;
    std::string PADstring, PADstring_last;
    EmotionDynamics* EmoConPerson;
    EmotionConverterPAD* PADconverter;
    int getXPos();
    int getYPos();
    int getZPos();
    int getXTens();
    int getYTens();
    int getSlope();
    int getMass();
    int getAlpha();
    int getBeta();
    int getFactor();
    int getUpdateRate();
    int getPValue();
    int getAValue();
    int getDValue();
    void setXPos(int xpos);
    void setYPos(int ypos);
    void setZPos(int zpos);
    void setPValue(int pval);
    void setAValue(int aval);
    void setDValue(int dval);
    void setSlope(int sval);
    void setMass(int mval);
    void setAlpha(int aval);
    void setBeta(int bval);
    void setFactor(int fval);
    void setXTens(int xtens);
    void setYTens(int ytens);
    void setUpdateRate(int updateRate);
    void setName(std::string name);
    void setGlobalID(std::string newGlobalID);
    void updatePADstring();
    bool update();
    void resetForces();
    bool simulationOn;
    std::string convertToEmoMLPAD(int number);
    std::string intToString(int number);
    std::string getPBuffer();
    std::string getABuffer();
    std::string getDBuffer();
    void resetBuffer();
    std::string PBuffer;
    std::string ABuffer;
    std::string DBuffer;
    bool initialized;
private:
    //EXTENSTION:
    int PADBufferCounter;
    void updateBuffer();
    //END OF EXTENSION
    void doMapping();
};
#endif
