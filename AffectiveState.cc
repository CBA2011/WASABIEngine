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

#include  "AffectiveState.h"
#include  "EmotionDynamics.h"
#include <math.h>
#include <algorithm>
#include <limits>
//#define INFINITY numeric_limits<double>::infinity( );

using namespace std;

int
returnIndex(string word, const string line)
{
    string::size_type begIdx, endIdx;
    const string delims(" ");
    if (line.empty()) {
        return 0;
    }
    begIdx = line.find_first_not_of(delims);
    //cout << "line = \"" << line << "\"" << endl;
    //cout << "word = \"" << word << "\"" << endl;
    int wordcount = 1;
    while (begIdx != string::npos) {
        endIdx = line.find_first_of(delims, begIdx);
        if (endIdx == string::npos) {
            endIdx = line.length();
        }
        //cout << "\nbegIdx = " << begIdx << ", endIdx = " << endIdx << endl;
        string token = line.substr(begIdx, (endIdx - begIdx));
        //cout << "EmotionContainer::returnIndex: token (" << token << ") == word (" << word << ") ? ";
        if (word == token) {
            //cout << "--> yes, returning " << wordcount << endl;
            return wordcount;
        }
        //cout << "--> no, searching on" << endl;
        wordcount++;
        begIdx = line.find_first_not_of(delims, endIdx);
    }
    return 0;
}



AffectiveState::AffectiveState()
{
    init();
}

void
AffectiveState::init() {
    lifetime = -1.0;
    standardLifetime = -1.0;
    decayFunction = AffectiveState::NONE;
    intensity = 0.0;
    id = -1;
    type = "undefined";
    decayParameter = 1.0;
    emoCon = 0;
    baseIntensity = 1.0;
    cout << "AffectiveState::init()" << endl;
}

AffectiveState::~AffectiveState()
{
    // nothing
}

AffectiveState::AffectiveState(std::vector<AffectPolygon*> ap_vec) {
    init();
    addPolygon(ap_vec);
}

AffectiveState::AffectiveState(AffectPolygon* ap) {
    init();
    vector<AffectPolygon*> ap_vec;
    ap_vec.push_back(ap);
    addPolygon(ap_vec);
}

bool
AffectiveState::setLifetime(double newLifetime) {
    std::cout << "AffectiveState::setLifetime: got lifetime " << newLifetime << "!" << std::endl;
    if (newLifetime < 0 && newLifetime != -1.0) {
        cout << "AffectiveState::setLifetime: value invalid " << newLifetime << "!" << endl;
        return false;
    }
    lifetime = newLifetime;
    intensity = 1.0;
    if (emoCon) {
        if (decayFunction == LINEAR){
            decayParameter = (emoCon->dt)/lifetime;
            cout << "AffectiveState::setLifetime: decayParameter = " << decayParameter << "!" << endl;
        }
        if (decayFunction == EXPONENTIAL) {
            decayParameter = 1 - (decayParameter * emoCon->dt);
        }
    }
    else {
        cout << "AffectiveState::setLifetime: WARNING decayParameter not changed!" << endl;
    }

    return true;
}

bool
AffectiveState::setStandardLifetime(double newLifetime) {
    if (newLifetime < 0 && newLifetime != -1.0) {
        cout << "AffectiveState::setStandardLifetime: value invalid " << newLifetime << "!" << endl;
        return false;
    }
    standardLifetime = newLifetime;
    return true;
}

bool
AffectiveState::setBaseIntensity(double newIntensity) {
    if (newIntensity < 0 || newIntensity > 1.0) {
        cout << "AffectiveState::setIntensity: value invalid " << newIntensity << "!" << endl;
        return false;
    }
    baseIntensity = newIntensity;
    return true;
}


void
AffectiveState::setDecayFunction(decayFunctionEnum decayFunc, double decayParam) {
    cout << "function " << decayFunc << ", param " << decayParam << endl;
    decayFunction = decayFunc;
    if (decayParam > 0.0){
        decayParameter = decayParam;
    }
    else if (decayFunction == LINEAR) {
        decayParameter = 0.01;
    }
}

/*! Calculate the likelihood of this AffectiveState with respect to the data
 *  in the EmotionContainer emoCon, if it is set.
 */
void
AffectiveState::updateLikelihood() {
    if (emoCon) {
        vector<int> PADValues;
        PADValues.push_back(emoCon->pValue);
        PADValues.push_back(emoCon->aValue);
        PADValues.push_back(emoCon->dValue);
        likelihoods = updateLikelihood(PADValues);
        return;
    }
    cout << "AffectiveState::updateLikelihood(): " << type << ", no emoCon set! Please provide PAD values." << endl;
    vector<AffectVertex*> v;
    likelihoods = v;
}

/*! Calculate the likelihood of this AffectiveState with respect to the data
 *  in the EmotionContainer emoCon, if it is set.
 */
vector<AffectVertex*>
AffectiveState::getLikelihood() {
    return likelihoods;
}

/*! Calculate the likelihood of the AffectiveState using:
 *  1. The PAD values as present in the vector PAD.
 *  2. The min/max_distances.
 *  3. The intensity of the AffectiveState.
 */
vector<AffectVertex*> AffectiveState::updateLikelihood(vector<int> PAD) {
    vector<AffectVertex*> v;
    if (PAD.size() != 3) {
        cout << "AffectiveState::getLikelihood: PAD.size() != 3, returning empty vector!" << endl;
        return v;
    }
    vector<AffectPolygon*>::iterator iter_ap;
    int polycount = 0;
    for (iter_ap = polygons.begin(); iter_ap != polygons.end(); ++iter_ap) {
        vector<AffectVertex*> verts = (*iter_ap)->vertices;
        // Do we have a single point or a complete vertex here?
        if (verts.size() == 1) { // i.e. a point
            polycount++;
            if (intensity > 0.0) { // Only if AffectiveState is not OFF
                AffectVertex* vertex = verts[0];
                float a = ((float) (PAD[0] - vertex->coords[0])) / 100;
                float b = ((float) (PAD[1] - vertex->coords[1])) / 100;
                float c = ((float) (PAD[2] - vertex->coords[2])) / 100;
                float tempDistance = sqrt(pow(c, 2) + pow(a, 2) + pow(b, 2));
                if (tempDistance <= (*iter_ap)->min_distance) {
                    if (tempDistance < (*iter_ap)->max_distance) {
                        tempDistance = (*iter_ap)->max_distance;
                    }
                    float activation = 1.0;
                    if ((*iter_ap)->min_distance > (*iter_ap)->max_distance) {
                        activation = 1 - (tempDistance
                                          - (*iter_ap)->max_distance)
                                / ((*iter_ap)->min_distance
                                   - (*iter_ap)->max_distance);
                    }
                    vertex->likelihood = activation * intensity; // NOTE: we take the AffectiveState's intensity here!!!
                    if (vertex->likelihood < 0.001) {
                        vertex->likelihood = 0.0;
                    } else {
                        //dump(cout);
                        v.push_back(vertex);
                    }
                } // end if (tempDistance ..)
                else {
                    vertex->likelihood = 0.0;
                }
            } // end if (verts[0]->intensity ..)
        } // end if (verts.size() ..)
        else if (verts.size() == 2) { // i.e. a line
            cout << "AffectiveState::getLikelihood: TODO line!" << endl;
            // calculate the distance between point and line here
        } else if (verts.size() > 2) { // some polygon
            //cout << "updateLikelihood for " << type << endl;
            if (verts[0]->intensity > 0.0) { // Only if Vertex is not OFF
                float polygon_likelihood = (*iter_ap)->getIntensity(PAD.at(0),
                                                                    PAD.at(1), PAD.at(2));
                if (polygon_likelihood > 0.005) {
                    //cout << "AffectiveState::updateLikelihood: got likelihood " << polygon_likelihood
                    //     << " for " << type << endl;
                    polygon_likelihood *= getIntensity();
                } else {
                    polygon_likelihood = 0.0;
                }
                if (polygon_likelihood > 0.005) {
                    vector<AffectVertex*>::iterator iter_verts;
                    for (iter_verts = verts.begin(); iter_verts != verts.end(); iter_verts++) {
                        (*iter_verts)->likelihood = polygon_likelihood;
                        v.push_back((*iter_verts));
                    }
                }
            }
        }
    } // end for
    sort(v.begin(), v.end());
    return v;
}

bool
AffectiveState::addPolygon(std::vector<AffectPolygon*> data){
    if (data.empty()){
        return false;
    }

    vector< AffectPolygon* >::iterator iter_polys;
    for(iter_polys=data.begin(); iter_polys!=data.end(); iter_polys++) {
        if ((*iter_polys)->valid()){
            polygons.push_back((*iter_polys));
        }
    }
    return true;
}

bool
AffectiveState::addPolygon(AffectPolygon* polygon){
    if (polygon == 0 || !(polygon->valid())) {
        return false;
    }

    vector< AffectPolygon* > data;
    data.push_back(polygon);
    return addPolygon(data);
}

void
AffectiveState::update(float dt){
    //cout << "AffectiveState::update: " << type << " with dt " << dt << " now!" << endl;
    //cout << "lifetime = " << lifetime << endl;
    if (lifetime > 0.0){
        lifetime -= dt;
    }
    switch (decayFunction) {
    case NONE:
        break;
    case LINEAR:
        intensity -= decayParameter;
        //cout << "intensity = " << intensity << endl;
        break;
    case EXPONENTIAL:
        intensity *= decayParameter;
        break;
    case COSINE:
        cout << "AffectiveState::update: COSINE TODO" << endl;
        break;
    default:
        cout << "AffectiveState::update: mode " << decayFunction << " is unknown!" << endl;
    }
    if (lifetime == -1.0) {
        intensity = baseIntensity;
    }
    else if (lifetime < 0.01) {
        intensity = 0.0;
        lifetime = 0.0;
    }
    if (intensity < 0.01) {
        intensity = baseIntensity;
        lifetime = 0.0;
    }

    // We dont need to update the intensities of every Vertex here!
    //   vector<AffectPolygon*>::iterator iter_poly = polygons.begin();
    //   for (; iter_poly != polygons.end(); iter_poly++){
    //     if ((*iter_poly)->vertices.size() == 1) { // i.e. a single point
    //       (*iter_poly)->vertices.at(0)->intensity = intensity;
    //     }
    //   }
    //cout << "AffectiveState::update: type " << type << " with intensity now " << intensity << ", lifetime now " << lifetime << endl;
    // at last we update the likelihoods
    updateLikelihood();
}

bool
AffectiveState::setIntensity(double newIntensity){
    if (newIntensity > 1.0 || newIntensity < 0.0){
        return false;
    }
    intensity = newIntensity;
    return true;
}

void
AffectiveState::dump(ostream& ostr)
{
    int poly_count = 0;
    int vertex_count = 0;
    ostr << "#######################" << endl
         << "# AffectiveState \"" << type << "\""
         << " has " << polygons.size() << " polygon(s) " << endl;
    vector< AffectPolygon* >::iterator iter_polys;
    for(iter_polys=polygons.begin(); iter_polys!=polygons.end(); iter_polys++) {
        if ((*iter_polys)->valid()){
            poly_count++;
            vertex_count = 0;
            ostr << "# - polygon (" << poly_count << ") number of vertices "
                 << ((*iter_polys)->vertices).size() << endl;
            vector< AffectVertex* >::iterator iter_verts;
            for(iter_verts=(*iter_polys)->vertices.begin(); iter_verts!=(*iter_polys)->vertices.end(); iter_verts++) {
                vertex_count++;
                ostr << "# - - vertex (" << vertex_count << ") coords = ("
                     << (*iter_verts)->coords[0] << ", "
                     << (*iter_verts)->coords[1] << ", "
                     << (*iter_verts)->coords[2] << "), "
                     << "likelihood = " << (*iter_verts)->likelihood << endl;
            }
        }
    }
    ostr << "#######################" << endl;
}

bool
AffectiveState::setLikelihood(float newLikelihood)
{
    if (!emoCon || emoCon->isActive()) {
        cout << "AffectiveState::setLikelihood: no emoCon or emoCon is active!" << endl;
        return false;
    }
    vector<AffectPolygon*>::iterator iter_ap;
    for (iter_ap = polygons.begin(); iter_ap != polygons.end(); ++iter_ap){
        vector<AffectVertex*> verts = (*iter_ap)->vertices;
        vector<AffectVertex*>::iterator iter_av;
        for (iter_av = verts.begin(); iter_av != verts.end(); ++iter_av){
            (*iter_av)->likelihood = newLikelihood;
        }
    }
    return true;
}



AffectVertex::AffectVertex()
{
    init();
}

AffectVertex::AffectVertex(int data[3])
{
    init();
    coords[0] = data[0];
    coords[1] = data[1];
    coords[2] = data[2];
}

AffectVertex::AffectVertex(int data[3], double intens)
{
    init();
    coords[0] = data[0];
    coords[1] = data[1];
    coords[2] = data[2];
    if (intens >= 0.0 && intens <= 1.0) {
        intensity = intens;
    }
    else {
        cout << "AffectVertex::AffectVertex: Given intensity value "
             << intens << " not within [0.0, 1.0] --> setting 1.0." << endl;
        intensity = 1.0;
    }
}

void
AffectVertex::init(){
    // The following default value lets the plain vertex be invalid.
    coords[0] = coords[1] = coords[2] = -101;
    intensity = 1.0;
    likelihood = 0.0;
}

bool
AffectVertex::valid(){
    for (int index = 0; index <= 2; index++){
        if (coords[index] > 100 || coords[index] < -100){
            return false;
        }
    }
    return true;
}

AffectPolygon::AffectPolygon()
{
    init();
}

AffectPolygon::AffectPolygon(vector<AffectVertex*> av_vec, string glMode)
{
    init(glMode);
    addVertices(av_vec);
    //cout << "#######################" << endl;
}

AffectPolygon::AffectPolygon(AffectVertex* av)
{
    init();
    vector<AffectVertex*> av_vec;
    av_vec.push_back(av);
    addVertices(av_vec);
}

void
AffectPolygon::init(string gl_mode) {
    min_distance = 0.64; // same as outerRadius
    max_distance = 0.2; // same as innerRadius
    glMode = gl_mode; // Dont want to include OpenGL headers here
    minX = minY = 101; // invalid value!
    maxX = maxY = -101;
}

void
AffectPolygon::addVertices(vector<AffectVertex*> verts){
    vector< AffectVertex* >::iterator iter_vertices;
    for(iter_vertices=verts.begin(); iter_vertices!=verts.end(); iter_vertices++) {
        if ((*iter_vertices)->valid()){
            if ((*iter_vertices)->coords[0] < minX) {
                minX = (*iter_vertices)->coords[0];
            }
            if ((*iter_vertices)->coords[0] > maxX) {
                maxX = (*iter_vertices)->coords[0];
            }
            if ((*iter_vertices)->coords[1] < minY) {
                minY = (*iter_vertices)->coords[1];
            }
            if ((*iter_vertices)->coords[1] > maxY) {
                maxY = (*iter_vertices)->coords[1];
            }
            vertices.push_back((*iter_vertices));
        }
    }
}


bool
AffectPolygon::valid(){
    if (vertices.empty()){
        return false;
    }
    vector< AffectVertex* >::iterator iter_vertices;
    int dominance = -101;
    for(iter_vertices=vertices.begin(); iter_vertices!=vertices.end(); iter_vertices++) {
        if ((*iter_vertices)->valid()){
            if (dominance == -101) {
                dominance = (*iter_vertices)->coords[2];
            }
            else if (dominance != (*iter_vertices)->coords[2]) {
                cout << "AffectPolygon::valid(): " << dominance << " != " << (*iter_vertices)->coords[2] << " returning false!" << endl;
                return false;
            }
        }
        else {
            cout << "AffectPolygon::valid(): vertex invalid, returning false!" << endl;
            return false;
        }
    }
    return true;
}

float
AffectPolygon::getIntensity(int p, int a, int d){
    if (minX < -100 || minY < -100 || maxX > 100 || maxY > 100 || vertices.empty()){
        cout << "AffectPolygon::getIntensity: no valid min/max values!" << endl;
        return 0.0;
    }
    if (glMode != "QUAD"){
        cout  << "AffectPolygon::getIntensity: only QUAD supported so far!" << endl;
        return 0.0;
    }
    float returnValue = 0.0;
    //cout << "AffectPolygon::getIntensity: (p, a, d) = (" << p << ", " << a << ", " << d << ")" << endl;
    if (p >= minX && p <= maxX && a >= minY && a <= maxY) {
        int* coords1 = vertices.at(0)->coords;
        int* coords2 = vertices.at(1)->coords;
        int* coords3 = vertices.at(2)->coords;
        int* coords4 = vertices.at(3)->coords;
        int* co[4];
        co[0] = coords1;
        co[1] = coords2;
        co[2] = coords3;
        co[3] = coords4;
        //cout << "vertex1: (" << coords1[0] << ", " << coords1[1] << ", " << coords1[2] << ")" << endl;
        //cout << "vertex2: (" << coords2[0] << ", " << coords2[1] << ", " << coords2[2] << ")" << endl;
        float m[4] = {0.0, 0.0, 0.0, 0.0};
        int x[4];
        int y[4];
        float b[4];
        for (int i = 0; i <= 3; i++) {
            //cout << "\ni = " << i << endl;
            int alpha = i;
            int beta = (alpha + 1) % 4;
            if (co[alpha][2] != d) {
                //cout << "AffectPolygon::getIntensity: invalid dominance value " << d << "!" << endl;
                return 0.0;
            }
            y[alpha] = co[alpha][0];
            b[alpha] = 0;
            //cout << "(" << co[alpha][0] << ", " << co[alpha][1] << ") --- (" << co[beta][0] << ", " << co[beta][1] << ")" << endl;
            // y = m * x + b
            // m = (beta(y) - alpha(y)) / (beta(x) - alpha(x))
            // b = alpha(y) - m * alpha(x)
            if (co[alpha][0] != co[beta][0]) {
                m[alpha] = (float)(co[beta][1] - co[alpha][1]) / (float)(co[beta][0] - co[alpha][0]);
                b[alpha] = co[alpha][1] - m[alpha] * co[alpha][0];
                //cout << "m = " << m[alpha] << ", b = " << b[alpha] << endl;
                y[alpha] = (int)floor( ((m[alpha] * (float)p) + b[alpha]) + 0.5);
            }
            else {
                m[alpha] = INFINITY; // infinte
                y[alpha] = co[alpha][0];
                x[alpha] = p; // instead of infinity
                b[alpha] = INFINITY;
            }
            //cout << " (p,y[" << alpha << "]) = (" << p << "," << y[alpha] << ")" << endl;
        }
        // does the point lie within the polygon?
        // x = (y - b) / m
        for (int index_i = 0; index_i < 4; index_i++) {
            if (x[index_i] != p) {
                if (m[index_i] != 0){
                    x[index_i] = (a - b[index_i]) / m[index_i];
                }
                else {
                    x[index_i] = p; // instead of infinity
                }
                //cout << "x[" << index_i << "] = (" << a << " - " << b[index_i] << ") / " << m[index_i] << " = " << x[index_i] << endl;
            }
            //cout << " (a,x[" << index_i << "]) = (" << a << "," << x[index_i] << ")" << endl;
        }
        //cout << "(x0, y0) = (" << x[0] << ", " << y[0] << "), x3 = " << x[3] << endl;
        if (a <= y[0] && p <= x[1] && a >= y[2] && p >= x[3]) {
            vector<float> tempVec1, tempVec2;
            int xy[2];
            xy[0] = p;
            xy[1] = a;
            //cout << "AffectPolygon::getIntensity: on polygon!" << endl;
            if (a <= max(co[0][1], co[1][1]) && a > min(co[0][1], co[1][1])) { //
                //cout << "crossing line 0" << endl;
                if (tempVec1.empty()) {
                    tempVec1 = linearInterpolation(const_cast <AffectVertex&> (*vertices.at(0)), const_cast <AffectVertex&> (*vertices.at(1)), xy);
                }
                else {
                    tempVec2 = linearInterpolation(const_cast <AffectVertex&> (*vertices.at(0)), const_cast <AffectVertex&> (*vertices.at(1)), xy);
                }
            }
            if (a <= max(co[1][1], co[2][1]) && a > min(co[1][1], co[2][1])) { //
                //cout << "crossing line 1" << endl;
                if (tempVec1.empty()) {
                    tempVec1 = linearInterpolation(const_cast <AffectVertex&> (*vertices.at(1)), const_cast <AffectVertex&> (*vertices.at(2)), xy);
                }
                else {
                    tempVec2 = linearInterpolation(const_cast <AffectVertex&> (*vertices.at(1)), const_cast <AffectVertex&> (*vertices.at(2)), xy);
                }
            }
            if (a <= max(co[2][1], co[3][1]) && a > min(co[2][1], co[3][1])) { //
                //cout << "crossing line 2" << endl;
                if (tempVec1.empty()) {
                    tempVec1 = linearInterpolation(const_cast <AffectVertex&> (*vertices.at(2)), const_cast <AffectVertex&> (*vertices.at(3)), xy);
                }
                else {
                    tempVec2 = linearInterpolation(const_cast <AffectVertex&> (*vertices.at(2)), const_cast <AffectVertex&> (*vertices.at(3)), xy);
                }
            }
            if (a <= max(co[3][1], co[0][1]) && a > min(co[3][1], co[0][1])) { //
                //cout << "crossing line 3" << endl;
                if (tempVec1.empty()) {
                    tempVec1 = linearInterpolation(const_cast <AffectVertex&> (*vertices.at(3)), const_cast <AffectVertex&> (*vertices.at(0)), xy);
                }
                else {
                    tempVec2 = linearInterpolation(const_cast <AffectVertex&> (*vertices.at(3)), const_cast <AffectVertex&> (*vertices.at(0)), xy);
                }
            }
            if (tempVec1.empty() || tempVec2.empty()){
                //cout << "AffectPolygon::getIntensity: ERROR one of the Vectors is empty!" << endl;
                return 0.0;
            }
            returnValue = tempVec1.at(0);
            if (tempVec1.at(1) != tempVec2.at(1)) {
                returnValue = (p - tempVec1.at(1)) / (tempVec2.at(1) - tempVec1.at(1))
                        * (tempVec2.at(0) - tempVec1.at(0))
                        + tempVec1.at(0);
            }
        }
    }
    //cout << "AffectPolygon::getIntensity: returning " << returnValue << endl;
    return returnValue;
}

vector<float>
AffectPolygon::linearInterpolation(AffectVertex const &v1, AffectVertex const &v2, int coord[])
{
    vector<float> returnVector;
    int xy = 1;
    if (v1.coords[1] == v2.coords[1]) {
        cout << "AffectPolygon::linearInterpolation: switching to x!" << endl;
        xy = 0;
    }
    int yx = (xy + 1) % 2;
    //cout << "v2.intensity = " << v2.intensity << ", v1.intensity = " << v1.intensity
    //     << ". coord[0] = " << coord[0] << ", coord[1] = " << coord[1] << endl;
    float factor = ((float)(coord[xy] - v1.coords[xy]) / (float)(v2.coords[xy] - v1.coords[xy]));
    float returnValue = (v2.intensity - v1.intensity)
            * factor
            + v1.intensity;
    returnVector.push_back(returnValue);
    //cout << (v2.intensity - v1.intensity) << " * " << factor << " + " << v1.intensity << " = " << returnValue << endl;
    int yx_coord = ((float)(coord[xy] - v1.coords[xy]) / (float)(v2.coords[xy] - v1.coords[xy])) * (float)(v2.coords[yx] - v1.coords[yx]) + v1.coords[yx];
    if (xy == 1){
        //cout << "AffectPolygon::linearInterpolation: returning " << returnValue << " at (" << yx_coord << ", " << coord[1] << ")" << endl;
        returnVector.push_back((float)yx_coord);
        returnVector.push_back((float)coord[1]);
    }
    else {
        //cout << "AffectPolygon::linearInterpolation: returning " << returnValue << " at (" << coord[0] << ", " << yx_coord << ")" << endl;
        returnVector.push_back((float)coord[0]);
        returnVector.push_back((float)yx_coord);
    }
    return returnVector;
}
