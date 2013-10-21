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

#include "cogaEmotionalAttendee.h"
#include "EmotionContainer.h"
#include <iostream>
#include <sstream>

using namespace std;

cogaEmotionalAttendee::cogaEmotionalAttendee()
    : cogaAttendee()
{
    //pers_xPos = pers_yPos = pers_zPos = 0;
    //pers_PValue = pers_AValue = pers_DValue = 0;
    //pers_dominance = 100;
    PADstring = PADstring_last = "";
    _name = "John Doe";
    EmoConPerson = new EmotionDynamics();
    //EmoConPerson->setUID(_name);
    //EmoConPerson->uid_protected = false;
    PADconverter = new EmotionConverterPAD();
    simulationOn = true;
    resetBuffer();
    initialized = false;
}

cogaEmotionalAttendee::~cogaEmotionalAttendee()
{
    delete EmoConPerson;
    delete PADconverter;
}

void
cogaEmotionalAttendee::setName(std::string name)
{
    _name = name;
}

void cogaEmotionalAttendee::setGlobalID(std::string newGlobalID)
{
    if (newGlobalID == "undef") {
        std::ostringstream o;
        o << newGlobalID << localID;
        newGlobalID = o.str();
    }
    globalID = newGlobalID;
}
int
cogaEmotionalAttendee::getXPos()
{
    if (EmoConPerson)
        return EmoConPerson->xPos;
    return 0;
}

int
cogaEmotionalAttendee::getYPos()
{
    if (EmoConPerson)
        return EmoConPerson->yPos;
    return 0;
}

int
cogaEmotionalAttendee::getZPos()
{
    if (EmoConPerson)
        return EmoConPerson->zPos;
    return 0;
}

int
cogaEmotionalAttendee::getXTens()
{
    if (EmoConPerson)
        return EmoConPerson->xTens;
    return 0;
}

int
cogaEmotionalAttendee::getYTens()
{
    if (EmoConPerson)
        return EmoConPerson->yTens;
    return 0;
}

int
cogaEmotionalAttendee::getSlope()
{
    if (EmoConPerson)
        return EmoConPerson->slope;
    return 0;
}

int
cogaEmotionalAttendee::getMass()
{
    if (EmoConPerson)
        return EmoConPerson->mass;
    return 0;
}

int
cogaEmotionalAttendee::getAlpha()
{
    if (EmoConPerson)
        return EmoConPerson->xReg;
    return 0;
}

int
cogaEmotionalAttendee::getBeta()
{
    if (EmoConPerson)
        return EmoConPerson->yReg;
    return 0;
}

int
cogaEmotionalAttendee::getFactor()
{
    if (EmoConPerson)
        return EmoConPerson->boredom;
    return 0;
}

int
cogaEmotionalAttendee::getUpdateRate()
{
    if (EmoConPerson)
        return (int) 1/(EmoConPerson->dt);
    return 0;
}

int
cogaEmotionalAttendee::getPValue()
{
    if (EmoConPerson)
        return EmoConPerson->pValue;
    return 0;
}

int
cogaEmotionalAttendee::getAValue()
{
    if (EmoConPerson)
        return EmoConPerson->aValue;
    return 0;
}

int
cogaEmotionalAttendee::getDValue()
{
    if (EmoConPerson)
        return EmoConPerson->dValue;
    return 0;
}

void
cogaEmotionalAttendee::setXPos(int xpos)
{
    if (EmoConPerson)
        EmoConPerson->xPos = EmoConPerson->sxt = EmoConPerson->sxlast = xpos;
}

void
cogaEmotionalAttendee::setYPos(int ypos)
{
    if (EmoConPerson)
        EmoConPerson->yPos = EmoConPerson->syt = EmoConPerson->sylast = ypos;
}

void
cogaEmotionalAttendee::setZPos(int zpos)
{
    if (EmoConPerson)
        EmoConPerson->zPos = zpos;
}

void
cogaEmotionalAttendee::setPValue(int pval)
{
    if (EmoConPerson)
        EmoConPerson->pValue = pval;
}

void
cogaEmotionalAttendee::setAValue(int aval)
{
    if (EmoConPerson)
        EmoConPerson->aValue = aval;
}

void
cogaEmotionalAttendee::setDValue(int dval)
{
    if (EmoConPerson) {
        // We have to do set the dom_target here, because
        // the dValue will be reset after each simulation
        // step.
        //EmoConPerson->dValue = dval;
        EmoConPerson->dom_target = dval;
    }
}

void
cogaEmotionalAttendee::setSlope(int sval)
{
    if (EmoConPerson)
        EmoConPerson->slope = sval;
}

void
cogaEmotionalAttendee::setMass(int mval)
{
    if (EmoConPerson)
        EmoConPerson->mass = mval;
}

void
cogaEmotionalAttendee::setAlpha(int aval)
{
    if (EmoConPerson)
        EmoConPerson->xReg = aval;
}

void
cogaEmotionalAttendee::setBeta(int bval)
{
    if (EmoConPerson)
        EmoConPerson->yReg = bval;
}

void
cogaEmotionalAttendee::setFactor(int fval)
{
    if (EmoConPerson)
        EmoConPerson->boredom = fval;
}

void
cogaEmotionalAttendee::setXTens(int xtens)
{
    if (EmoConPerson)
        EmoConPerson->xTens = xtens;
}

void
cogaEmotionalAttendee::setYTens(int ytens)
{
    if (EmoConPerson)
        EmoConPerson->yTens = ytens;
}

void
cogaEmotionalAttendee::setUpdateRate(int updateRate)
{
    if (EmoConPerson)
        EmoConPerson->dt = 1/(float)updateRate;
}


/*!
 * Hier wird bei einer Veränderung des PADstrings das 'Max sagt: ...' aktualisiert.<br>
 */
void
cogaEmotionalAttendee::updatePADstring()
{
    if (!EmoConPerson){
        cerr << "cogaEmotionalAttendee::updatePADstring: no EmoConPerson found!" << endl;
        return;
    }
    ASVectorMMap ASLikely = EmoConPerson->getAffectLikelihoods();
    if (!ASLikely.empty()) {
        PADstring_last = PADstring;
        PADstring = "";
        ASVectorMMap::iterator asvmmapiter = ASLikely.begin();
        for (; asvmmapiter != ASLikely.end(); asvmmapiter++) {
            PADstring.append("&");
            PADstring.append(asvmmapiter->first->type);
            PADstring.append("=");
            std::ostringstream o;
            if (o << asvmmapiter->second.at(0)->likelihood) {
                PADstring.append(o.str());
            }
            //PADstring.append(")");
        }
        //cout << "cogaEmotionalAttendee::updatePADstring: " << PADstring << endl;
    }
}

void
cogaEmotionalAttendee::doMapping()
{
    std::vector<int> v;
    v.push_back(getXPos());
    v.push_back(getYPos());
    v.push_back(getZPos());
    v.push_back(getDValue());
    std::vector<int> padValues = PADconverter->convertToClassType(v);
    // NEW we also update the pad-Values of the EmotionContainer
    //cout << "cogaEmotionalAttendee::updatePADstring: p " << padValues[0]
    //    << ", a " << padValues[1]
    //   << ", d " << padValues[2] << endl;
    if (EmoConPerson != 0 && padValues.size() == 3) {
        EmoConPerson->pValue = padValues[0];
        EmoConPerson->aValue = padValues[1];
        EmoConPerson->dValue = padValues[2];
    }
    else {
        cout << "WARNING: cogaEmotionalAttendee::doMapping: could not update EmotionContainer!" << endl;
    }
}

bool
cogaEmotionalAttendee::update()
{
    if (!EmoConPerson){
        std::cout << "cogaEmotionalAttendee::update(): No EmoConPerson, returning false!" << std::endl;
        return false;
    }
    if (!simulationOn) {
        return false;
    }
    EmoConPerson->update();
    updatePADstring();
    doMapping();
    //EXTENSION3:
    if (PADBufferCounter >= 500){
        resetBuffer();
    }
    updateBuffer();
    //END OF EXTENSION3
    return true;
}

void
cogaEmotionalAttendee::resetForces() {
    if (!EmoConPerson){
        std::cout << "cogaEmotionalAttendee::resetSimulationValues(): No EmoConPerson, returning!" << std::endl;
        return;
    }
    EmoConPerson->vxt = EmoConPerson->vxlast = EmoConPerson->axt = EmoConPerson->axlast = 0;
    EmoConPerson->vyt = EmoConPerson->vylast = EmoConPerson->ayt = EmoConPerson->aylast = 0;
}
//EXTENSION4:
void
cogaEmotionalAttendee::resetBuffer() {
    PBuffer = ABuffer = DBuffer = "";
    PADBufferCounter = 0;
}

void
cogaEmotionalAttendee::updateBuffer() {
    std::string strPVal, strAVal, strDVal;

    //strCurrentBuffer = PBuffer;
    strPVal = convertToEmoMLPAD(EmoConPerson->pValue);
    PBuffer = PBuffer + strPVal + " ";

    //strCurrentBuffer = ABuffer;
    strAVal = convertToEmoMLPAD(EmoConPerson->aValue);
    ABuffer = ABuffer + strAVal + " ";

    //strCurrentBuffer = DBuffer;
    strDVal = convertToEmoMLPAD(EmoConPerson->dValue);
    DBuffer = DBuffer + strDVal + " ";

    (PADBufferCounter)++;
}


std::string
cogaEmotionalAttendee::getPBuffer()
{
    return this->PBuffer;

}


std::string
cogaEmotionalAttendee::getABuffer()
{
    return this->ABuffer;

}

std::string
cogaEmotionalAttendee::getDBuffer()
{
    return this->DBuffer;

}


std::string
cogaEmotionalAttendee::convertToEmoMLPAD(int number)
{
    float fPADValue;
    std::string s;
    std::stringstream out;

    fPADValue = ((float)number + 100)/200;
    out << fPADValue;
    s = out.str();
    return s;
}

std::string
cogaEmotionalAttendee::intToString(int number)
{
    std::string s;
    std::stringstream out;
    std::cout << "???????????" << std::endl;
    out << number;
    s = out.str();
    return s;
}

//END OF EXTENSION4
