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

#ifndef EmotionContainer_H
#define EmotionContainer_H

#include <iostream>
#include "AffectiveState.h"
#include "WASABIEngine_global.h"

typedef std::multimap<AffectiveState*, std::vector<AffectVertex*> > ASVectorMMap;
typedef std::multimap<std::string, std::vector<float> > AffectTypeLikelihoodsMMap;

/*!
 * Container-Klasse fuer die Position, die im Dynamik-Raum erreicht werden soll.
 * Hier wird die Gueltigkeit der Koordinaten verwaltet.
 */
class WASABIENGINESHARED_EXPORT EmoPos2Reach
{

public:
    EmoPos2Reach ();
    EmoPos2Reach(const EmoPos2Reach &emoPos);
    virtual ~EmoPos2Reach ();
    int getXPos2Reach();
    int getYPos2Reach();
    void setPositions2Reach(int xPos, int yPos);
    void setXValid(bool valid);
    void setYValid(bool valid);
    bool getXValid();
    bool getYValid();

protected:
    int xPos2Reach, yPos2Reach;
    bool xValid, yValid;

};

/*!
 * Diese Klasse kapselt alle physikalischen Variablen des Dynamik-Raumes.
 */
class WASABIENGINESHARED_EXPORT EmotionContainer
{
    friend class WASABIENGINESHARED_EXPORT cogaEmotionalAttendee;
public:
    // Konstruktor
    EmotionContainer ();
    EmotionContainer(const EmotionContainer &emoCon);
    // Destruktor
    virtual ~EmotionContainer ();

    //protected:
    double dt; // time difference (see timer_calculate!)

    //void setUID(std::string uid);
    //std::string getUID();

    virtual void writeTransferable(std::ostream &ostr) const;
    virtual void readTransferable(std::istream&);

    virtual void update(float _dt);
    virtual void init();

    //void update();
    std::string getType() {return type;}

    //* NEW: We keep a list of affective states here:
    // As every EmotionalAttendee automatically generates an EmotionContainer
    // we keep track of possible AffectiveStates here. These AffectiveStates might
    // be realized as Primary or Secondary Emotions both of them being derived from
    // AffectiveState.
    // These AffectiveStates have a function "getLikelihood(std::vector<int> PAD)" returning
    // the awareness likelihood as a float value. We just have to iterate this vector at every
    // simulation step now, calling the "update(float dt)" functions of each AffectiveState.
    std::vector<AffectiveState*> affectiveStates;

    //* To read the actual MultiMap of likelihoods > 0.0
    // Format: ((AffectiveState*, vector<AffectVertex*>), ..)
    // We may directly access the highest likelihood using:
    // > ASVectorMMap::iterator asvmmapiter = asvmmap.begin();
    // > iter_asvmmap->second.at(0)->likelihood;
    ASVectorMMap getAffectLikelihoods() {return asvmmap;}

    //* Useful if we only now the type name
    AffectiveState* getAffectiveStateByType(std::string type);

    void dumpAffectiveStates(std::ostream &ostr);

    //* Returns the MOOD_ID of the AffectiveState with the highest likelihood
    // emotionclass might be given as either any, primary or secondary, so far.
    // DEFAULT: any
    int getASId(std::string emotionclass = "any");

    //* Returns the type of the AffectiveState with the highest likelihood
    // emotionclass might be given as either any, primary or secondary, so far.
    // DEFAULT: any
    std::string getASType(std::string emotionclass = "any");

    //* Returns the highest likelihood of all AffectiveStates
    // emotionclass might be given as either any, primary or secondary, so far.
    // DEFAULT: any
    float getASLikelihood(std::string emotionclass = "any");

    //* Returns the bool isActive
    bool isActive() {return active;}

    //* Holds a simpler representation of the actual affective states together with
    // the likelihoods.
    // Format: ((string type, vector<float> (MOOD_ID, likelihoods), ..)
    // This is needed, if the affectiveStates Vector is not neccessary and thus not present.
    // We keep it up to date whenever updateAffectLikelihoods is called!
    // Only this mmap is transfered in the EmotionMessage!
    AffectTypeLikelihoodsMMap atlmmap;

    //* To trigger an AS from the outside, i.e. the EmotionAgent.
    // Returning true in case of success, false otherwise (e.g., if type not found)
    bool triggerAS(std::string _type, double newLifetime = -1.0);

    //* To update the MultiMap containing the likelihoods > 0.0.
    // Is called automatically within update(float dt).
    // NEW: by setting forcePADupdate = true we can force the EmotionContainer to call as->updateLikelihood(),
    // which will use the current PAD values to calculate each AffectiveState's likelihood accordingly.
    // This is used when we don't want to call "update" before calling this function.
    void updateAffectLikelihoods(bool forcePADupdate = false);
    bool buildPrimaryEmotion(std::vector<float> v, std::string type, std::string decayFunc);

protected:
    //std::string _uid;
    std::string type;
    ASVectorMMap asvmmap;
    //bool uid_protected; // should be set, if EmotionContainer belongs to an EmotionalAttendee

    //* The active indicates whether we are continously updating our
    // affectiveStates ourselves (active = true) by update() being called
    // or if we are just parsing EmotionMessages and copy their values into
    // our affectiveStates (active = false).
    // DEFAULT: false, i.e. we are only listening.
    bool active;
    bool buildSecondaryEmotion(std::string secfilename);
};

#endif
