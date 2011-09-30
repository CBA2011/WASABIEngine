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

#include "EmotionContainer.h"
#include "PrimaryEmotion.h"
#include "SecondaryEmotion.h"
#include "EmotionDynamics.h"
#include <fstream>      // for file-I/O
#include <typeinfo>
#include <sstream>
using namespace std;

/*!
 * Setzt xPos2Reach und yPos2Reach auf 0, booleans auf false
 */
EmoPos2Reach::EmoPos2Reach ()
{
    xPos2Reach = yPos2Reach = 0;
    xValid = false;
    yValid = false;
}

EmoPos2Reach::EmoPos2Reach(const EmoPos2Reach &emoPos) {
    xPos2Reach = emoPos.xPos2Reach;
    yPos2Reach = emoPos.yPos2Reach;
    xValid = emoPos.xValid;
    yValid = emoPos.yValid;
}

/*!
 * nichts
 */
EmoPos2Reach::~EmoPos2Reach ()
{
}

/*!
 * returns the x position target
 */
int
EmoPos2Reach::getXPos2Reach()
{
    return xPos2Reach;
}

/*!
 * returns the y position target
 */
int
EmoPos2Reach::getYPos2Reach()
{
    return yPos2Reach;
}

/*!
 * Keeps track of limits
 */
void
EmoPos2Reach::setPositions2Reach(int xPos, int yPos)
{
    xPos2Reach = xPos;
    if (xPos2Reach > 100)
        xPos2Reach = 100;
    if (xPos2Reach < -100)
        xPos2Reach = -100;
    yPos2Reach = yPos;
    if (yPos2Reach > 100)
        yPos2Reach = 100;
    if (yPos2Reach < -100)
        yPos2Reach = -100;
}

/*!
 * Checks validity of x
 */
bool
EmoPos2Reach::getXValid()
{
    return xValid;
}

/*!
 * Checks validity of y
 */
bool
EmoPos2Reach::getYValid()
{
    return yValid;
}

/*!
 * Setzt xValid.
 */
void
EmoPos2Reach::setXValid(bool valid)
{
    xValid = valid;
}

/*!
 * Setzt yValid.
 */
void
EmoPos2Reach::setYValid(bool valid)
{
    yValid = valid;
}

/*!
 * inits the values
 */
EmotionContainer::EmotionContainer()
{
    init();
}

void
EmotionContainer::init()
{
    //action = EmotionMessage::UNDEFINED;
    //_uid = "MAX0";
    //uid_protected = false;
    //actualEmotionMessage = new MaxEmotionMessage();
    type = "plain";
    dt = 0.04; // 25 Hz! NEW in WINDOWS VERSION WIN32
    active = false;
}

/*!
 * Simple Copy Constructor
 */
EmotionContainer::EmotionContainer(const EmotionContainer &emoCon)
//  : _uid (emoCon._uid),
    : active (emoCon.active),
      //    uid_protected (emoCon.uid_protected),
      dt (emoCon.dt),
      type (emoCon.type),
      atlmmap (emoCon.atlmmap),
      asvmmap (emoCon.asvmmap)
{
    if (emoCon.active) {
        cout << "ACTIVE copy const" << endl;
    }
}

EmotionContainer::~EmotionContainer()
{
    //nothing
}

/*
void
EmotionContainer::setUID(std::string uid)
{
  if (uid_protected){
    std::cerr << "UID of EmotionContainer protected -> cannot be set!" << std::endl;
    return;
  }
  _uid = uid;
}

std::string
EmotionContainer::getUID()
{
  return _uid;
}
*/
void
EmotionContainer::writeTransferable(std::ostream &ostr) const
{
    ostr << " " << type << " " << dt << " atlmmap_begin ";
    AffectTypeLikelihoodsMMap::const_iterator atlmmapiter = atlmmap.begin();
    //cout << "atlmmap.size() = " << atlmmap.size() << endl;
    for (; atlmmapiter != atlmmap.end(); atlmmapiter++) {
        //cout << atlmmapiter->first << " " << endl;;
        ostr << atlmmapiter->first << " ";
        //ostr << atlmmapiter->second.at(0) << " ";
        for (unsigned int index = 0; index < atlmmapiter->second.size(); index++) {
            ostr << atlmmapiter->second.at(index) << " ";
            //cout << atlmmapiter->second.at(index) << " ";
        }
    }
    ostr << "atlmmap_end ";
    //cout << "atlmmap_end " << endl;
}

void
EmotionContainer::readTransferable(std::istream &istr)
{
    istr >> type >> dt;
    string keyword;
    double doubleValue = 1.0;
    //cout << "istr = (" << istr.rdbuf() << ")" << endl;
    if (istr.good()) {
        istr >> keyword;
        if (keyword == "atlmmap_begin") {
            atlmmap.clear();
            string token;
            istr >> token;
            //cout << "EmotionContainer::readTransferable: token = " << token << endl;
            while (token != "atlmmap_end") {
                string type = token;
                vector<float> float_vec;
                while (istr.good() && (doubleValue != 0.0)) {
                    istr >> token;
                    std::stringstream sstr(token);
                    // 	  if (!(istr >> token)) {
                    // 	    cerr << "EmotionContainer::readTransferable: ERROR more tokens expected!" << endl;
                    // 	    return;
                    // 	  }
                    //cout << "EmotionContainer::readTransferable: (while2) token = " << token << endl;

                    sstr >> doubleValue;// = atof(token.c_str()); // returns 0 in case of error!
                    //cout << "doubleValue = " << doubleValue << endl;
                    // BUG: If MOOD_ID == 0 we have a problem here!
                    // Might never occur as 0 means MOOD_NEUTRAL
                    // DEFAULT value is -1!
                    if (doubleValue != 0.0){
                        float_vec.push_back(doubleValue);
                    }
                }
                //cout << "EmotionContainer::readTransferable: (while1) token = " << token << endl;
                if (!float_vec.empty()) {
                    atlmmap.insert( make_pair(type, float_vec));
                    // If this EmotionContainer is not actively updated,
                    // we try to update the affectiveStates manually here!
                    if (!active && !affectiveStates.empty()) {
                        AffectiveState* as = getAffectiveStateByType(type);
                        if (as) {
                            as->setLikelihood(float_vec.at(0));
                        }
                    }
                }
                doubleValue = 1.0;
            }
        }
    }
}

/*! We perform an update of all AffectiveStates here, such that
 *  the intensity values are recalculated
 */
void
EmotionContainer::update(float _dt)
{
    // We assume that this means the EmotionContainer is actively updated
    // rather that passively filled with data by EmotionMessages.
    //  cout << "EmotionContainer::update: setting active!" << endl;
    active = true;
    vector<AffectiveState*>::iterator iter_as;
    for(iter_as=affectiveStates.begin(); iter_as!=affectiveStates.end(); iter_as++) {
        (*iter_as)->update(_dt);
    }
    updateAffectLikelihoods();
}

AffectiveState*
EmotionContainer::getAffectiveStateByType(string type)
{
    vector<AffectiveState*>::iterator iter_as;
    for (iter_as = affectiveStates.begin(); iter_as!=affectiveStates.end(); iter_as++) {
        if ((*iter_as)->type == type){
            return (*iter_as);
        }
    }
    cerr << "ERROR: getAffectiveStateByType: No AffectiveState with type " << type << " found! Returning 0. " << endl;
    return 0;
}

void
EmotionContainer::dumpAffectiveStates(std::ostream &ostr)
{
    int as_count = 0;
    vector< AffectiveState* >::iterator iter_as;
    for(iter_as=affectiveStates.begin(); iter_as!=affectiveStates.end(); iter_as++) {
        as_count++;
        cout << "(" << as_count << ")" << endl;
        (*iter_as)->dump(ostr);
    }
}

/*! This function returns a StringVectorMultiMap with the following structure:
 *  {(string type, <vector float> likelihoods), ..}, where
 *  "<vector float> likelihoods" contains the likelihoods of every single AffectPolygon
 *  the AffectiveState "type" consists of.
 */
void
EmotionContainer::updateAffectLikelihoods(bool forcePADupdate)
{
    asvmmap.clear();
    atlmmap.clear();
    vector<AffectiveState*>::iterator iter_as;
    for(iter_as=affectiveStates.begin(); iter_as!=affectiveStates.end(); iter_as++) {
        if (forcePADupdate) {
            (*iter_as)->updateLikelihood();
        }
        vector<AffectVertex*> lh = (*iter_as)->getLikelihood();
        if (!lh.empty()) {
            asvmmap.insert( make_pair((*iter_as), lh));
            vector<AffectVertex*>::iterator iter_av;
            vector<float> float_vec;
            float_vec.push_back((float)(*iter_as)->getID());
            for(iter_av=lh.begin(); iter_av!=lh.end(); iter_av++) {
                //cout << "AS: " << (*iter_as)->type << ", ID: " << (*iter_as)->getID() << ", lh: " << (*iter_av)->likelihood << endl;
                float_vec.push_back((*iter_av)->likelihood);
            }
            atlmmap.insert( make_pair((*iter_as)->type, float_vec));
            //cout << "EmotionContainer::updateAffectLikelihoods: atlmmap->second.size() = " << float_vec.size() << endl;
        }
    }
}


int
EmotionContainer::getASId(string emotionclass){
    float lh = 0.0;
    if (isActive()){
        ASVectorMMap::iterator iter_asvmmap = asvmmap.begin();
        AffectiveState* as = 0;
        for (; iter_asvmmap != asvmmap.end(); iter_asvmmap++) {
            bool check = false;
            if (emotionclass == "any") {
                check = true;
            }
            else if (emotionclass == "primary" && typeid(*(iter_asvmmap->first)) == typeid(PrimaryEmotion)) {
                check = true;
            }
            else if (emotionclass == "secondary" && typeid(*(iter_asvmmap->first)) == typeid(SecondaryEmotion)) {
                check = true;
            }
            if (check){
                if (iter_asvmmap->second.at(0)->likelihood > lh) {
                    lh = iter_asvmmap->second.at(0)->likelihood;
                    as = iter_asvmmap->first;
                }
            }
        }
        if (as) {
            return (as->getID());
        }
    }
    else {
        //cout << "EmotionContainer::getASId: we are passive!" << endl;
        AffectTypeLikelihoodsMMap::iterator iter_atlmmap = atlmmap.begin();
        int id = 0;
        //cout << "EmotionContainer::getASId: atlmmap.size() = " << atlmmap.size() << endl;
        for (; iter_atlmmap != atlmmap.end(); iter_atlmmap++) {
            //cout << "iter_atlmmap->second.size() = " << iter_atlmmap->second.size() << endl;
            bool check = false;
            if (emotionclass == "any") {
                check = true;
            }
            else if (emotionclass == "primary" && typeid(*getAffectiveStateByType((iter_atlmmap->first))) == typeid(PrimaryEmotion)) {
                check = true;
            }
            else if (emotionclass == "secondary" && typeid(*getAffectiveStateByType((iter_atlmmap->first))) == typeid(SecondaryEmotion)) {
                check = true;
            }
            if (check){
                if (iter_atlmmap->second.at(1) > lh) {
                    lh = iter_atlmmap->second.at(1);
                    id = (int)iter_atlmmap->second.at(0);
                }
            }
        }
        //cout << "EmotionContainer::getASId: id = " << id << endl;
        return id;
    }
    return -1; // i.e. failure
}

std::string
EmotionContainer::getASType(string emotionclass){
    float lh = 0.0;
    if (isActive()){
        ASVectorMMap::iterator iter_asvmmap = asvmmap.begin();
        AffectiveState* as = 0;
        for (; iter_asvmmap != asvmmap.end(); iter_asvmmap++) {
            bool check = false;
            if (emotionclass == "any") {
                check = true;
            }
            else if (emotionclass == "primary" && typeid(*(iter_asvmmap->first)) == typeid(PrimaryEmotion)) {
                check = true;
            }
            else if (emotionclass == "secondary" && typeid(*(iter_asvmmap->first)) == typeid(SecondaryEmotion)) {
                check = true;
            }
            if (check){
                if (iter_asvmmap->second.at(0)->likelihood > lh) {
                    lh = iter_asvmmap->second.at(0)->likelihood;
                    as = iter_asvmmap->first;
                }
            }
        }
        if (as) {
            return (as->type);
        }
    }
    else {
        //cout << "EmotionContainer::getASId: we are passive!" << endl;
        AffectTypeLikelihoodsMMap::iterator iter_atlmmap = atlmmap.begin();
        string _type = "undef";
        //cout << "EmotionContainer::getASId: atlmmap.size() = " << atlmmap.size() << endl;
        for (; iter_atlmmap != atlmmap.end(); iter_atlmmap++) {
            //cout << "iter_atlmmap->second.size() = " << iter_atlmmap->second.size() << endl;
            bool check = false;
            if (emotionclass == "any") {
                check = true;
            }
            else if (emotionclass == "primary" && typeid(*getAffectiveStateByType((iter_atlmmap->first))) == typeid(PrimaryEmotion)) {
                check = true;
            }
            else if (emotionclass == "secondary" && typeid(*getAffectiveStateByType((iter_atlmmap->first))) == typeid(SecondaryEmotion)) {
                check = true;
            }
            if (check){
                if (iter_atlmmap->second.at(1) > lh) {
                    lh = iter_atlmmap->second.at(1);
                    _type = iter_atlmmap->first;
                }
            }
        }
        //cout << "EmotionContainer::getASId: id = " << id << endl;
        return _type;
    }
    return "undef"; // i.e. failure
}

float
EmotionContainer::getASLikelihood(string emotionclass){
    float lh = 0.0;
    if (active) {
        ASVectorMMap::iterator iter_asvmmap = asvmmap.begin();
        AffectiveState* as = 0;
        for (; iter_asvmmap != asvmmap.end(); iter_asvmmap++) {
            bool check = false;
            if (emotionclass == "any") {
                check = true;
            }
            else if (emotionclass == "primary" && typeid(*(iter_asvmmap->first)) == typeid(PrimaryEmotion)) {
                check = true;
            }
            else if (emotionclass == "secondary" && typeid(*(iter_asvmmap->first)) == typeid(SecondaryEmotion)) {
                check = true;
            }
            if (check){
                if (iter_asvmmap->second.at(0)->likelihood > lh) {
                    lh = iter_asvmmap->second.at(0)->likelihood;
                    as = iter_asvmmap->first;
                }
            }
        }
        if (as) {
            return lh;
        }
    }
    else {
        //cout << "EmotionContainer::getaslikelihood: atlmmap.size() = " << atlmmap.size() << endl;
        AffectTypeLikelihoodsMMap::iterator iter_atlmmap = atlmmap.begin();
        for (; iter_atlmmap != atlmmap.end(); iter_atlmmap++) {
            //cout << "iter_atlmmap->second.size() = " << iter_atlmmap->second.size() << endl;
            //cout << iter_atlmmap->first << endl;
            bool check = false;
            if (emotionclass == "any") {
                check = true;
            }
            else if (emotionclass == "primary" && typeid(*getAffectiveStateByType((iter_atlmmap->first))) == typeid(PrimaryEmotion)) {
                check = true;
            }
            else if (emotionclass == "secondary" && typeid(*getAffectiveStateByType((iter_atlmmap->first))) == typeid(SecondaryEmotion)) {
                check = true;
            }
            if (check){
                if (iter_atlmmap->second.at(1) > lh) {
                    lh = iter_atlmmap->second.at(1);
                }
            }
        }
        //cout << "EmotionContainer::getASLikelihood: lh = " << lh << endl;
        return lh;
    }
    return 0.0; // i.e. failure
}

bool
EmotionContainer::buildPrimaryEmotion(std::vector<float> v, std::string type, std::string decayFunc)
{
    if (v.size() != 7) {
        cerr << "EmotionContainer::buildPrimaryEmotion: vector.size() != 7, returning 0 pointer!" << endl;
        return false;
    }
    int pad[3];
    //  cout << "D = " << v.at(2) << " - " << (int)(v.at(2) * 100) << endl;
    pad[0] = (int)(v.at(0) * 100);
    pad[1] = (int)(v.at(1) * 100);
    pad[2] = (int)(v.at(2) * 100);
    AffectVertex* av = new AffectVertex(pad);
    // Now we pass it to an AffectPolygon
    AffectPolygon* ap = new AffectPolygon(av);
    ap->max_distance = v.at(4);
    //cout << "### max = " << v.at(4) << endl;
    ap->min_distance = v.at(5);
    //cout << "### min = " << v.at(5) << endl;
    //* We have to check for the type if there is a PrimaryEmotion
    // in affectiveStates with the same type already.
    vector<AffectiveState*>::iterator iter_as;
    AffectiveState* as_found = 0;
    for (iter_as=(affectiveStates).begin(); (as_found == 0) && iter_as!=(affectiveStates).end(); iter_as++) {
        if ((*iter_as)->type == type) {
            as_found = (*iter_as);
            //cout << endl << "FOUND id " << v.at(3) << endl;
        }
    }
    if (as_found != 0) {
        cout << "EmotionContainer::buildPrimaryEmotion: found " << as_found->type << ". Adding polygon!" << endl;
        return as_found->addPolygon(ap);
    }
    else {
        cout << "EmotionContainer::buildPrimaryEmotion: no type " << type << " found. Adding PrimaryEmotion!" << endl;
        PrimaryEmotion* pe = new PrimaryEmotion(ap);
        pe->type = type;
        pe->setID((int)v.at(3));
        pe->setBaseIntensity(v.at(6));
        if (decayFunc == "LINEAR"){
            pe->setDecayFunction(AffectiveState::LINEAR);
        }
        else if (decayFunc == "EXPONENTIAL"){
            pe->setDecayFunction(AffectiveState::EXPONENTIAL);
        }
        else if (decayFunc == "COSINE") {
            pe->setDecayFunction(AffectiveState::COSINE);
        }
        EmotionDynamics* me = dynamic_cast<EmotionDynamics*>(this);
        if (me) {
            pe->setEmotionContainer(me);
        }
        // Just a test
        //pe->setDecayFunction(AffectiveState::EXPONENTIAL, 0.8);
        //pe->setLifetime(5.0);
        affectiveStates.push_back(pe);
    }
    return true;
}

bool
EmotionContainer::buildSecondaryEmotion(string secfilename)
{
    cout << "-----------------\nEmotionContainer::buildSecondaryEmotion: Trying to load " << secfilename << endl;
    SecondaryEmotion* se = new SecondaryEmotion();
    EmotionDynamics* me = dynamic_cast<EmotionDynamics*>(this);
    if (me) {
        se->setEmotionContainer(me);
    }
    if (!(se->loadFromFile(secfilename))) {
        cerr << "EmotionContainer::buildSecondaryEmotion: ERROR no SecondaryEmotion created!" << endl;
        return false;
    }
    affectiveStates.push_back(se);
    cout << "EmotionContainer::buildSecondaryEmotion: END\n-----------------" << endl;
    return true;
}

bool
EmotionContainer::triggerAS(string _type, double newLifetime){
    AffectiveState* as = getAffectiveStateByType(_type);
    if (!as) {
        return false;
    }
    if (newLifetime != -1) {
        std::cout << "EmotionContainer::triggerAS: lifetime = " << newLifetime << std::endl;
        return as->trigger(newLifetime);
    }
    return as->trigger();
}
