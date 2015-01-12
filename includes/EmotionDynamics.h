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

#ifndef EMOTIONDYNAMICS_H
#define EMOTIONDYNAMICS_H

#include "EmotionContainer.h"
#include "WASABIEngine_global.h"


/*!
 * This class implements the emotion dynamics.
 * It is derived from EmotionContainer and reimplements the update-function
 */
class WASABIENGINESHARED_EXPORT EmotionDynamics: public EmotionContainer
{

public:
    EmotionDynamics ();
    ~EmotionDynamics ();
    EmotionDynamics(const EmotionDynamics &emoCon);
    virtual void update(float dt);
    void update();

    virtual void init();
    std::string dynFilename;
    std::string padFilename;
    std::string xmlFilename;
    bool initEmoDyn();
    bool initEmoPAD();

    //bool loadAffectiveStates(std::string filename, std::string fileType);

    virtual void writeTransferable(std::ostream &ostr) const;
    virtual void readTransferable(std::istream&);
    // physic variables
    int spon_target, prev_target, dom_target;
    double mass; // mass of sphere (should be 1?)
    double sxlast, sylast, sxt, syt, sdom, sdomlast; // current positions
    double vxlast, vylast, vxt, vyt, vdom, vdomlast; // last and current velocities
    double axlast, aylast, axt, ayt, adom, adomlast; // last and current acceleration
    double z;
    bool xSignChange, ySignChange;
    int xSign, ySign;
    EmoPos2Reach* positions2Reach;
    int xTens, yTens, slope, xReg, yReg, boredom, mean, prevalence;

    float lastUpdate;

    // This Message is extending the EmotionContainer and might be used for data of the emotion recognition as well now!
    // It is allways held up to date by updateEmotionMessage in AC_Qt3EmotionsAgent for Max!
    // NOW OBSOLETE, see below!
    // MaxEmotionMessage* actualEmotionMessage;

    //  int domTarget;
    // NEW: mood_id is added to make class EmotionContainer in "EmotionTypes.hh" from MultimodalFigure obsolete.
    // Was formerly only encoded in the Message itself (but I am not so resistant to Stefan's advice as he seems to believe :-) )
    // NEW Instead of mood_id and mood_id_last we now keep track of the AffectiveStates in the ASVectorMMap of
    // EmotionContainer. The same holds for mood_weight and mood_weight_last
    int xPos, yPos, zPos, pValue, aValue, dValue;
    //float mood_weight, mood_weight_last;
    //std::string mood_description;
};

#endif
