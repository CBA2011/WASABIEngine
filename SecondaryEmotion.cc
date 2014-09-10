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

#include  "SecondaryEmotion.h"
#include <fstream>      // for file-I/O
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;

SecondaryEmotion::SecondaryEmotion()
{
    // nothing so far
}

SecondaryEmotion::SecondaryEmotion(std::vector<AffectPolygon*> ap_vec)
    : AffectiveState(ap_vec)
{
    // nothing
}

SecondaryEmotion::SecondaryEmotion(AffectPolygon* ap)
    : AffectiveState(ap)
{
    // nothing
}

bool
SecondaryEmotion::loadFromFile(string filename){
    ifstream file(filename.c_str());
    if (!file) { // bailing out
        cerr << "SecondaryEmotion::SecondaryEmotion: filename \"" << filename << "\" not found!" << endl;
        return false;
    } // ok, we will read the contents now
    bool returnValue = true;
    string line;
    string keyword;
    while (getline(file, line)){
        //cout << "outerloop: line = " << line << endl;
        if (line.length() > 0 && (line.at(0)) != '#') { //comments begin with '#'
            istringstream isline(line);
            string gl_mode;
            vector<AffectVertex*> av_vec;
            AffectPolygon* ap;
            int v_num = 1;
            int v_num_count = 0;
            string token1, token2, token3, token4;
            char* endptr;
            double doubleValue;
            if (isline >> keyword) {
                switch (returnIndex(keyword, "polygon_begin vertex lifetime baseIntensity type tokens_begin decayFunction")) {
                case 1: // polygon_begin
                    cout << "SecondaryEmotion::SecondaryEmotion: loading polygon" << endl;
                    if (!(isline >> gl_mode)) {
                        cerr << "SecondaryEmotion::loadFromFile: ERROR no gl_mode provided!" << endl;
                        returnValue = false;
                    }
                    if (gl_mode == "QUAD") {
                        v_num = 4;
                    }
                    // trying to load the vertices
                    while (keyword != "polygon_end" && getline(file, line) && line.length() > 0 && (line.at(0)) != '#') {
                        //cout << "###" << line << endl;
                        istringstream isline2(line);
                        if (!(isline2 >> keyword) || (keyword != "vertex" && keyword != "polygon_end")) {
                            cerr << "SecondaryEmotion::loadFromFile: ERROR no or wrong keyword found!" << endl;
                            break;
                        }
                        if (keyword == "vertex"){
                            if (v_num_count < v_num) {
                                if (!(isline2 >> token1 && isline2 >> token2 && isline2 >> token3 && isline2 >> token4)){
                                    cerr << "SecondaryEmotion::loadFromFile: ERROR one of PAD or intensity missing!" << endl;
                                    cerr << "line: " << line << endl;
                                    break;
                                }
                                int PAD[3] = {atoi(token1.c_str()), atoi(token2.c_str()), atoi(token3.c_str())};
                                doubleValue = strtod(token4.c_str(), &endptr);
                                AffectVertex* av = new AffectVertex(PAD, doubleValue);
                                av_vec.push_back(av);
                            }
                            else {
                                v_num_count++;
                                cerr << "SecondaryEmotion::loadFromFile: WARNING expected only " << v_num << " vertices, got " << v_num_count << " already!" << endl;
                            }
                        }
                    }
                    if (av_vec.size() > 0) {
                        cout << "#######################" << endl;
                        ap = new AffectPolygon(av_vec, gl_mode);
                        cout << "#######################" << endl;
                        returnValue = addPolygon(ap);
                    }
                    break;
                case 2: // vertex
                    cerr << "SecondaryEmotion::loadFromFile: WARNING found vertex without prior polygon_begin! Ignoring.." << endl;
                    break;
                case 3: // lifetime
                    cout << "lifetime" << endl;
                    if (!(isline >> token1)) {
                        cerr << "SecondaryEmotion::loadFromFile: ERROR no <double> lifetime found!" << endl;
                        break;
                    }
                    doubleValue = strtod(token1.c_str(), &endptr);
                    if (!setStandardLifetime(doubleValue) || !setLifetime(doubleValue)) {
                        cerr << "SecondaryEmotion::loadFromFile: WARNING invalid lifetime " << doubleValue << endl;
                    }
                    break;
                case 4: // baseIntensity
                    //cout << "intensity" << endl;
                    if (!(isline >> token1)) {
                        cerr << "SecondaryEmotion::loadFromFile: ERROR no <double> baseIntensity found!" << endl;
                        break;
                    }
                    doubleValue = strtod(token1.c_str(), &endptr);
                    //cout << "intens = " << doubleValue << endl;
                    if (!setBaseIntensity(doubleValue)) {
                        cerr << "SecondaryEmotion::loadFromFile: WARNING invalid baseIntensity " << doubleValue << endl;
                    }
                    break;
                case 5: // type
                    //cout << "type" << endl;
                    if (!(isline >> token1)) {
                        cerr << "SecondaryEmotion::loadFromFile: ERROR no <string> type found!" << endl;
                        break;
                    }
                    type = token1;
                    break;
                case 6: // tokens_begin
                    //cout << "tokens_begin" << endl;
                    if (!(isline >> gl_mode)) {
                        cerr << "SecondaryEmotion::loadFromFile: ERROR no type for tokens provided!" << endl;
                        returnValue = false;
                    }
                    // trying to load tokens
                    while (token1 != "tokens_end" && getline(file, line) && line.length() > 0 && (line.at(0)) != '#') {
                        //cout << "###" << line << endl;
                        istringstream isline2(line);
                        if (!(isline2 >> token1)){
                            cerr << "SecondaryEmotion::loadFromFile: ERROR no token found!" << endl;
                            cerr << "line: " << line << endl;
                            break;
                        }
                        if (token1 != "tokens_end"){
                            cout << "generated token: " << (gl_mode + "_" + token1) << endl;
                            tokens.push_back(gl_mode + "_" + token1);
                        }
                    }
                    break;
                case 7: // decayFunction
                    //cout << "decayFunction" << endl;
                    if (!(isline >> token1)) {// && isline >> token2)) {
                        cout << "SecondaryEmotion::loadFromFile: ERROR no <int> decayFunction found!" << endl;
                        break;
                    }
                    //doubleValue = strtod(token2.c_str(), &endptr);
                    if (token1 == "NONE") {
                        setDecayFunction(NONE);
                    }
                    else if (token1 == "LINEAR") {
                        setDecayFunction(LINEAR);
                    }
                    else if (token1 == "EXPONENTIAL") {
                        setDecayFunction(EXPONENTIAL);
                    }
                    else if (token1 == "COSINE") {
                        setDecayFunction(COSINE);
                    }
                    else {
                        cout << "SecondaryEmotion::loadFromFile: WARNING decayFunction " << token1 << " unknown!" << endl;
                    }
                    break;
                default: // unknown keyword
                    cout << "SecondaryEmotion::loadFromFile: WARNING unknown keyword " << keyword << endl;
                }
            }
        } // end if not '#'
    } // end while
    return returnValue;
}

SecondaryEmotion::~SecondaryEmotion()
{
    // nothing
}

void
SecondaryEmotion::addPrimaryEmotion(PrimaryEmotion* pe) {
    usesPrimaryEmotions.push_back(pe);
}
