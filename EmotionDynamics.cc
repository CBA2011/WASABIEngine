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

#include  "EmotionDynamics.h"
#include <fstream>      // for File-I/O
#include <iostream>
#include <sstream>

using namespace std;

EmotionDynamics::EmotionDynamics()
{
    EmotionContainer::init();
    init();
}

void
EmotionDynamics::init()
{
    mass = 1000;
    sxlast = sylast = sxt = syt = 0;
    prevalence = 0; // implemented by Marius Klug 4.7.12
    sdom = sdomlast = 100;
    vxlast = vylast = vxt = vyt = vdom = vdomlast = 0;
    axlast = aylast = axt = ayt = adom = adomlast = 0;
    z = 0;
    positions2Reach = new EmoPos2Reach();
    xSignChange = false;
    ySignChange = false;
    xSign = ySign = 0;
    dom_target = 100;
    boredom = 50;
    spon_target = prev_target = 0;
    pValue = aValue = 0;
    dValue = 100;
    type = "dynamic";
    lastUpdate = 0.;
    dynFilename = "init.emo_dyn";
    padFilename = "init.emo_pad";
    xmlFilename = "WASABI_agent_default.xml";
}

/*!
 * Deleting positions2Reach
 */
EmotionDynamics::~EmotionDynamics()
{
    delete positions2Reach;
    positions2Reach = 0;
}

/*!
 * Simple Copy Constructor
 */
EmotionDynamics::EmotionDynamics(const EmotionDynamics &emoCon)
    : EmotionContainer( emoCon)
{
    mass = emoCon.mass;
    sxlast = emoCon.sxlast;
    sylast = emoCon.sylast;
    sxt = emoCon.sxt;
    syt = emoCon.syt;
    sdom = emoCon.sdom;
    sdomlast = emoCon.sdomlast;
    vxlast = emoCon.vxlast;
    vylast = emoCon.vylast;
    vxt = emoCon.vxt;
    vyt = emoCon.vyt;
    vdom = emoCon.vdom;
    vdomlast = emoCon.vdomlast;
    axlast = emoCon.axlast;
    aylast = emoCon.aylast;
    axt = emoCon.axt;
    ayt = emoCon.ayt;
    adom = emoCon.adom;
    adomlast = emoCon.adomlast;
    dt = emoCon.dt; // 50 Hz!
    z = emoCon.z;
    positions2Reach = new EmoPos2Reach(*emoCon.positions2Reach);
    xSignChange = emoCon.xSignChange;
    ySignChange = emoCon.ySignChange;
    xSign = emoCon.xSign;
    ySign = emoCon.ySign;
    dom_target = emoCon.dom_target;
    boredom = emoCon.boredom;
    spon_target = emoCon.dom_target;
    prev_target = emoCon.prev_target;
    pValue = emoCon.pValue;
    aValue = emoCon.aValue;
    dValue = emoCon.dValue;
    xPos = emoCon.xPos;
    yPos = emoCon.yPos;
    zPos = emoCon.zPos;
    xTens = emoCon.xTens;
    yTens = emoCon.yTens;
    slope = emoCon.slope;
    xReg = emoCon.xReg;
    yReg = emoCon.yReg;
}

/*! If we dont get a dt value, we use our internal dt */
void
EmotionDynamics::update()
{
    update(dt);
}



/*!
 * Physics calculation for x-value (emotion) as described in PhD thesis.<p>
 * In addition, the y-value (mood) is calculated as follows:
 * By means of EmoPos2Reach contained in EmotionContainer, of which this class is a subclass,
 * an integer yPos2Reach is being managed as well as a boolean, which can be checked
 * using EmoPos2Reach->getYValid().
 * If this returns true, the achor point of the spiral spring for y is temporarily attached to the yPos2Reach value
 * (indicated by a blue circle in PAD space). Consequently, a force is applied to the reference point, by which it is
 * driven towards yPos2Reach. After reaching it, the forces are reset to zero and yPos2Reach->setValid(false) executed.
 * The y coordinate is, thus, again driven back to 0 over time.
 */
void
EmotionDynamics::update(float _dt) {
    //cout << "EmotionDynamics::update: BEGIN" << endl;
    int yTensTemp = yTens;
    double tempPosY = sylast;
    bool yOk = positions2Reach->getYValid();
    int yPos2Reach = positions2Reach->getYPos2Reach();
    if (yOk) {
        tempPosY = sylast - yPos2Reach;
        //yTensTemp *= 3;
        if (tempPosY > 0){
            if (ySign > 0){
                ySignChange = true;
                ySign = 0;
            }
            else {
                ySign = -1;
                ySignChange = false;
            }
        }
        if (tempPosY < 0){
            if (ySign < 0){
                ySignChange = true;
                ySign = 0;
            }
            else {
                ySign = 1;
                ySignChange = false;
            }
        }
        if ((ySignChange == true) || (int)tempPosY == 0){// && yPos2Reach != 0){
            cout << "Position REACHED !!!!!!" << endl;
            ySign = 0;
            ySignChange = true;
            positions2Reach->setYValid(false);// = -101;
            positions2Reach->setPositions2Reach(positions2Reach->getXPos2Reach(), 0);
            vylast = 0;
            aylast = 0;
        }
    }

    // Fxt = -xTens * xPos, axt = Fx / mass --> axt = (-xTens * xPos) / mass
    double Fx = (-(xTens) * (sxlast));
    double Fy = (-yTensTemp * (tempPosY - prevalence)); // prevalence for the mood
    //cout << "Fx = " << Fx << endl;
    //cout << "sxlast = " << sxlast << endl;
    //cout << "xTens = " << xTens << endl;

    axt = Fx / mass;
    //xcout << "-- axt = " << axt << endl;
    vxt = axt * dt + vxlast;
    sxt = vxt * dt + sxlast;
    if ((sxt > 0 && sxlast < 0) || (sxt < 0 && sxlast > 0)) {
        xPos = 0;
        sxt = sxlast = 0;
        vxlast = 0;
        axlast = 0;
    }
    else {
        xPos = (int)sxt;
        vxlast = vxt;
        axlast = axt;
        sxlast = sxt;
    }
    ayt = Fy / mass;
    vyt = ayt * dt + vylast;
    syt = vyt * dt + sylast;
    syt += sxt * ((float)(slope) / 100) / mass;

    if (sxt > 100)
        sxt = 100;
    if (syt > 100)
        syt = 100;
    if (sxt < -100)
        sxt = -100;
    if (syt < -100)
        syt = -100;

    if (((syt > prevalence && sylast < prevalence) || (syt < prevalence && sylast > prevalence)) // prevalence for the mood
            && !(positions2Reach->getYValid())) {
        yPos = prevalence;
        syt = sylast = prevalence;
        vylast = 0;
        aylast = 0;
    }
    else {
        yPos = (int)syt;
        vylast = vyt;
        aylast = ayt;
        sylast = syt;
    }

    if (xPos < xReg && yPos < (yReg + prevalence) && xPos > -(xReg) && yPos > -(yReg + prevalence)){ // boredom has to be calculated with prevalence
        if (z > -100) {
            z -= ((float)(boredom) / 1000);
        }
    }
    else {
        z = 0;
    }
    zPos = (int)z;
    // dominance update!
    if (dValue != dom_target){
        //     double Fdom = (-60 * (sdomlast - dom_target));
        //     adom = Fdom / mass;
        //     vdom = adom * dt + vdomlast;
        //     sdom = vdom * dt + sdomlast;
        //     dValue = (int)sdom;
        //     vdomlast = vdom;
        //     adomlast = adom;
        //    sdomlast = sdom;
        dValue = sdom = (int)dom_target;
    }
    else {
        dValue = (int)sdom;
        sdomlast = sdom;
        vdomlast = 0;
        adomlast = 0;
    }
    //cout << "EmotionDynamics::update: xPos = " << xPos << ", yPos = " << yPos << ", zPos = " << zPos << endl;
    EmotionContainer::update(_dt);
}

void
EmotionDynamics::writeTransferable(std::ostream &ostr) const
{

    EmotionContainer::writeTransferable(ostr);
    ostr << " " << xPos << " " << yPos << " " << zPos
         << " " << xTens << " " << yTens << " " << slope
         << " " << xReg << " " << yReg << " " << boredom << " " << mass
         << " " << pValue << " " << aValue << " " << dValue
         << " " << spon_target << " " << prev_target << " " << dom_target;
    //   cout << " " << xPos << " " << yPos << " " << zPos
    //        << " " << xTens << " " << yTens << " " << slope
    //        << " " << xReg << " " << yReg << " " << boredom << " " << mass
    //        << " " << pValue << " " << aValue << " " << dValue
    //        << " " << spon_target << " " << prev_target << " " << dom_target;
    //   cout << endl;
    //<< " " << mood_description << " ";
}

void
EmotionDynamics::readTransferable(std::istream &istr)
{
    EmotionContainer::readTransferable(istr);
    istr >> xPos >> yPos >> zPos
         >> xTens >> yTens >> slope
         >> xReg >> yReg >> boredom >> mass
         >> pValue >> aValue >> dValue
         >> spon_target >> prev_target >> dom_target;
    //>> mood_description;
}

bool
EmotionDynamics::initEmoDyn(){
    ifstream file(dynFilename.c_str());
    string line;
    if (file) {
        string word1;
        string word2;
        while (getline(file, line)) {
            if ((line.at(0)) != '#') { //comments begin with '#'
                istringstream isline(line);
                if ((line.at(0)) != '>') { //secondary emotion includes start with '>'
                    isline >> word1;
                    //cout << line << endl;
                    if (!(word1.empty())) {
                        cout << "(" << word1 << ")" << endl;
                        isline >> word2;
                        //cout << "-->(" << word2 << ")" << endl;
                        std::stringstream sstr(word2);
                        if (!(word2.empty())) {
                            switch (returnIndex(word1,
                                                "xTens yTens slope mass xReg yReg boredom prevalence")) {
                            case 1: //xTens
                                sstr >> xTens;
                                //xTens = intval(word2.c_str());
                                break;
                            case 2: //yTens
                                sstr >> yTens;// = intval(word2.c_str());
                                break;
                            case 3: //slope
                                sstr >> slope; // = intval(word2.c_str());
                                break;
                            case 4: //mass
                                sstr >> mass; // = atoi(word2.c_str());
                                break;
                            case 5: //xReg
                                sstr >> xReg; // = atoi(word2.c_str());
                                break;
                            case 6: //yReg
                                sstr >> yReg; // = atoi(word2.c_str());
                                break;
                            case 7: //boredom
                                sstr >> boredom;// = atoi(word2.c_str());
                                break;
                            case 8: //prevalence
                                sstr >> prevalence;// = atoi(word2.c_str());
                                if (prevalence > 100){ // prevalence must be set between -100 and 100
                                    prevalence = 100;
                                }
                                else if (prevalence < -100){
                                    prevalence = -100;
                                }
                                break;
                            default:
                                cout << "Error: Unknown keyword \"" << word1
                                     << "\"!" << endl;
                            }
                        }
                    }
                } // end not '>'
                else { // i.e. line.at(0) == '>', we include another file here
                    // we expect "> filename.sec" for inclusion of secondary emotion
                    string greaterthan; // will be ignored
                    string secfilename; // filename is already in use!
                    string::size_type idx = dynFilename.find_last_of('/');
                    if (idx == string::npos) { // No slash found!
                        cout
                                << "EmotionDynamics::initEmoDyn: no path info extracted for SecondaryEmotion!"
                                << endl;
                        isline >> greaterthan >> secfilename;
                        cout << "secfilename: " << secfilename << endl;
                        buildSecondaryEmotion(secfilename);
                    } else {
                        string path = dynFilename.substr(0, idx + 1);
                        isline >> greaterthan >> secfilename;
                        string f = path + secfilename;
                        cout << "secfilename: " << f << endl;
                        buildSecondaryEmotion(f);
                    }
                } // end '>' include
            } // end if ((line.at(0)) != '#')
        }//end while
        file.close();
    } //if (file)
    else {
        cerr
                << "EmotionDynamics::initEmoDyn: ERROR Could not open files!"
                << endl;
        return false; //could not open file;
    }
    return true;
}


bool
EmotionDynamics::initEmoPAD() {
    ifstream file(padFilename.c_str());
    string line;
    if (file) {
        string word1;
        string word2;
        while (getline(file, line)) {
            if ((line.at(0)) != '#') { //comments begin with '#'
                istringstream isline(line);
                if ((line.at(0)) != '>') { //secondary emotion includes start with '>'
                    isline >> word1;
                    //cout << line << endl;
                    if (!(word1.empty())) {
                        cout << "(" << word1 << ")" << endl;
                        isline >> word2;
                        //cout << "-->(" << word2 << ")" << endl;
                        if (!(word2.empty())) { //<EmotionCategory> P A D MOOD_XYZ max_distance min_distance
                            float A;
                            float D;
                            string MOOD_XYZ;
                            float max_distance, min_distance, standardIntens;
                            string decayFunction;
                            std::vector<float> v;
                            if (isline >> A && isline >> D && isline
                                    >> MOOD_XYZ && isline >> max_distance
                                    && isline >> min_distance && isline
                                    >> standardIntens) {
                                v.clear();
                                std::stringstream sstr(word2);
                                float P;
                                sstr >> P;
                                //float P = atof(word2.c_str());
                                if (P < -1)
                                    P = -1;
                                if (P > 1)
                                    P = 1;
                                if (A < -1)
                                    A = -1;
                                if (A > 1)
                                    A = 1;
                                if (D < -1)
                                    D = -1;
                                if (D > 1)
                                    D = 1;
                                if (max_distance < 0.0)
                                    max_distance = 0.0;
                                if (min_distance < 0.0)
                                    min_distance = 0.0;
                                if (max_distance > min_distance)
                                    max_distance = min_distance;
                                //cout << "MOOD: " << MOOD_XYZ << "P " << P << " A " << A << " D " << D << ", max " << max_distance << ", min " << min_distance << endl;
                                v.push_back(P);
                                v.push_back(A);
                                v.push_back(D);
                                switch (returnIndex(
                                            MOOD_XYZ,
                                            "MOOD_ANGRY MOOD_SAD MOOD_FRIENDLY MOOD_CONCENTRATED MOOD_SURPRISED MOOD_BORED MOOD_NEUTRAL MOOD_FEARFUL")) {
                                case 1: // MOOD_ANGRY
                                    v.push_back(MOOD_ANGRY);
                                    break;
                                case 2: // MOOD_SAD
                                    v.push_back(MOOD_SAD);
                                    break;
                                case 3: // MOOD_FRIENDLY
                                    v.push_back(MOOD_FRIENDLY);
                                    break;
                                case 4: // MOOD_CONCENTRATED
                                    v.push_back(MOOD_CONCENTRATED);
                                    break;
                                case 5: // MOOD_SURPRISED
                                    v.push_back(MOOD_SURPRISED);
                                    break;
                                case 6: // MOOD_BORED
                                    v.push_back(MOOD_BORED);
                                    break;
                                case 7: // MOOD_NEUTRAL
                                    v.push_back(MOOD_NEUTRAL);
                                    break;
                                case 8: // MOOD_FEARFUL
                                    v.push_back(MOOD_FEARFUL);
                                    break;
                                default:
                                    cerr << "MOOD_ID: " << MOOD_XYZ
                                         << " not known!" << endl;
                                    break;
                                }
                                v.push_back(max_distance);
                                v.push_back(min_distance);
                                v.push_back(standardIntens);
                                if (!(isline >> decayFunction)) {
                                    std::cout
                                            << "EmotionDynamics::initEmoPAD: No decayFunction given, assuming type NONE!"
                                            << std::endl;
                                    decayFunction = "NONE";
                                }
                                if (!buildPrimaryEmotion(v, word1,
                                                         decayFunction)) {
                                    cout
                                            << "EmotionDynamics::initEmoPAD: could not build PrimaryEmotion!"
                                            << endl;
                                }
                            }
                        }
                    }
                } // end not '>'
                else { // i.e. line.at(0) == '>', we include another file here
                    // we expect "> filename.sec" for inclusion of secondary emotion
                    string greaterthan; // will be ignored
                    string secfilename; // filename is already in use!
                    string::size_type idx = padFilename.find_last_of('/');
                    if (idx == string::npos) { // No slash found!
                        cout
                                << "EmotionDynamics::initEmoPAD: no path info extracted for SecondaryEmotion!"
                                << endl;
                        isline >> greaterthan >> secfilename;
                        cout << "secfilename: " << secfilename << endl;
                        buildSecondaryEmotion(secfilename);
                    } else {
                        string path = padFilename.substr(0, idx + 1);
                        isline >> greaterthan >> secfilename;
                        string f = path + secfilename;
                        cout << "secfilename: " << f << endl;
                        buildSecondaryEmotion(f);
                    }
                } // end '>' include
            } // end if ((line.at(0)) != '#')
        }//end while
        file.close();
    } //if (file)
    else {
        cerr
                << "EmotionDynamics::initEmoPAD: ERROR Could not open files!"
                << endl;
        return false; //could not open file;
    }
    return true;
}

