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

#ifndef _AFFECTIVESTATE_H_
#define _AFFECTIVESTATE_H_
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "WASABIEngine_global.h"

// These defines are only integrated for backward compatibility!
// They have been defined in EmotionTypes.h of aceMultimodalFigure.lib before.
#define MOOD_NEUTRAL         0  // Neutrale Stimmung
#define MOOD_FRIENDLY        1  // freundlich
#define MOOD_SAD             2  // traurig
#define MOOD_ANGRY           3  // aergerlich
#define MOOD_SURPRISED       4  // ueberrascht
#define MOOD_DISGUSTED       5  // angeekelt
#define MOOD_BORED           6  // gelangweilt
#define MOOD_CONCENTRATED    7  // konzentriert
#define MOOD_FEARFUL         8  // aengstlich
#define MOOD_MAX             8  // Nummer des letzten Eintrags

class WASABIENGINESHARED_EXPORT EmotionDynamics;

/// REMARK: May get obsolete soon because of PolygonVectorMMap below?
typedef std::multimap<std::string, std::vector<float> > StringVectorMMap;

/// To keep independent of BO_Strings
WASABIENGINESHARED_EXPORT int returnIndex(std::string word, const std::string search);

/*!
 * AffectVertex used for representation of an AffectiveState in PAD-space.
 * If we want to biuld a polygon, we might use several of these
 * together with intensity values to acquire a "region in PAD-space".
*/
class WASABIENGINESHARED_EXPORT AffectVertex
{
public:
    AffectVertex();
    AffectVertex(int data[3]);
    AffectVertex(int data[3], double intens);

    int coords[3];
    float intensity;
    //* In order to highlight the Vertex if it has gained some likelihood.
    // The value MUST be updated in getLikelihood of the AffectPolygon!
    // DEFAULT: 0.0
    float likelihood;
    //* Tests the vertex for validity, i.e.
    // if all values lie within [-100, 100].
    bool valid();

private:
    void init();
    bool operator() (AffectVertex* i, AffectVertex* j) { return (i->likelihood < j->likelihood);}
};

/*! AffectPolygon used for representation of an AffectiveState in PAD-space.
 *  Uses AffectVertices as described above.
*/
class WASABIENGINESHARED_EXPORT AffectPolygon
{
public:
    AffectPolygon();
    AffectPolygon(std::vector<AffectVertex*>, std::string glMode = "GL_POINTS");
    AffectPolygon(AffectVertex*);

    std::vector<AffectVertex*> vertices;

    /*!
   *  Tests the polygon for validity, i.e.
   * if all D-values lie in the same plane.
   */
    bool valid();

    //! To add vertices to the polygon
    void addVertices(std::vector<AffectVertex*>);

    /*!
   *  min_distance and max_distance are now substituting the activation and saturation thresholds
   *  of the original implementation of 2003
   *  DEFALUTS: min_distance = outerRadius = 0.64, max_distance = innerRadius = 0.2
   */
    float min_distance;
    float max_distance;

    /*!
   *  get the GL_MODE
   *  DEFAULT: GL_POINT
   */
    std::string glMode;
    //void calculateIntensityDistribution();
    float getIntensity(int p, int a, int d);
    std::vector<float> linearInterpolation(AffectVertex const &v1, AffectVertex const &v2, int coord[]);

private:
    std::vector< std::vector<int> > intensities;
    int minX, maxX, minY, maxY;
    void init(std::string glMode = "GL_POINTS");
};

/*! An AffectiveState is the base class of all
 *  Primary and Secondary emotions.
*/
class WASABIENGINESHARED_EXPORT AffectiveState
{
public :

    /// Default Constructor
    AffectiveState();
    virtual ~AffectiveState();

    AffectiveState(std::vector<AffectPolygon*> ap_vec);
    AffectiveState(AffectPolygon* ap);

    /*! Will be used to add a point, i.e. a polygon with only one vertex, a line, i.e. a polygon with only two vertices, or
   *  a polygon with (more than two) vertices lying all in the same plane (+/- 100 D) to the existing PADValues.
   */
    bool addPolygon(std::vector< AffectPolygon* > data);

    bool addPolygon(AffectPolygon* polygon);

    /*! This will set the new lifetime in seconds.
   *  We recalculate the decayParameter to ensure the correct lifetime
   *  with respect to the dt value of the EmotionContainer emoCon->dt.
   *  We will also reset the intensity value to 1.0!
   */
    bool setLifetime(double newLifetime);

    /*! For convenience we can call this function without parameter.
   * The standard lifetime will be set.
   */
    bool setLifetime() {return setLifetime(standardLifetime);}

    /// to make it easier to "trigger" an AffectiveState
    bool trigger(double _lifetime = -1) {if (_lifetime == -1) {return setLifetime();} else {return setLifetime(_lifetime);}}

    bool setStandardLifetime(double newLifetime);

    bool setBaseIntensity(double newIntensity);

    double getLifetime() {return lifetime;}

    /*! The following enumeration denotes the possible decay functions.
   *  DEFAULT: NONE
   */
    enum decayFunctionEnum{
        NONE = 0,
        LINEAR = 1,
        EXPONENTIAL = 2,
        COSINE = 3
    };

    /*! To set a new decay function for the current emotion.
   *  We won't reset the intesity value here.
   */
    void setDecayFunction(decayFunctionEnum decayFunc, double decayParam = 0.0);

    int getDecayFunction() {return decayFunction;}

    /*! Without an argument given we try to use the EmotionContainer emoCon to
   * get the PAD values of the EmotionalAttendee.
   */
    std::vector<AffectVertex*> getLikelihood();

    /// getter und setter for the ID
    void setID(int idnew) { if (idnew <= MOOD_MAX && idnew > 0) id = idnew; else std::cout << "AffectiveState::setID: " << idnew << " out of range!" << std::endl;}
    int getID() const {return id;}

    double getIntensity() const {return intensity;}

    /// Will update the intensity value according to the decay function und relative to the dt
    void update(float dt);

    /*! This will be the OCC / Scherer / Becker string denoting the type or class of emotion
   *  DEFAULT: undefined
   */
    std::string type;

    /*! This might be a list of tokens
   *  DEFAULT: empty
   */
    std::vector<std::string> tokens;

    /*! Now we want to be able to set a reference to an emotion container emoCon
   *  such that we can easily use the EmotionContainer's variables.
   */
    void setEmotionContainer(EmotionDynamics* newEmoCon) {emoCon = newEmoCon; }
    EmotionDynamics* emoCon;

    /*! Vector of AffectPolygons
   *  Every primary as well as secondary emotion is to be represented in PAD Space
   *  DEFAULT: empty vector.
   */
    std::vector<AffectPolygon*> polygons;

    //* Inhalt auf Stream schreiben/von Stream lesen
    // optional future work
    //virtual void writeTransferable(std::ostream&) const;
    //virtual void readTransferable(std::istream&);

    /*! In case our EmotionContainer is set to "isActive == false"
   * we might call this function to MANUALLY set the likelihood.
   */
    bool setLikelihood(float newLikelihood);

    /// Dump to stream for debugging
    void dump(std::ostream&);

    /*! updating all likelihoods of this affective state.
   * Should be called within update(float dt).
   */
    void updateLikelihood();
protected:

    /*! Will return the likelihood of the affective state to become aware.
  All the magic is to be done here:
  1. We will calculate the distance between PAD and affective state's Polygons.
  2. We take into account min/max_distances to calculate activation.
  3. likelihood(t) = activation(t) * intensity(t)
  If we have several polygons in the vector, we have to take the best match in step 1, i.e. the one with the closest distance.
  If the affective state is represented as a polygon, we take the minimum distance to the POLYGON instead of the POINTS.
  */
    std::vector<AffectVertex*> updateLikelihood(std::vector<int> PAD);

    /*! Every affective state (primary or secondary) may have a "lifetime" now.
  Together with the decay function below, we can realize a context dependent
  turning on/off of esp. secondary emotions.
  A lifetime of "-1.0" is interpreted as "always on" and should be the DEFAULT value.
  */
    double lifetime;

    /*! We might set a standard lifetime for every AS
   *  DEFAULT: -1.0, i.e. always on
   */
    double standardLifetime;

    /*! This can be set at the beginning and will be used when lifetime == -1
   *  DEFAULT: 1.0
   */
    double baseIntensity;

    /*! For backward compatibility we include an id being one of the "MOODS" defined above
   *  DEFAULT: MOOD_NEUTRAL.
   */
    int id;

    decayFunctionEnum decayFunction;

    /*! A possible parameter for the decay functions.
   *  DEFAULT: 1.0
   */
    double decayParameter;

    /*! To set the intensity if it lies within [0.0, 1.0]
   *  returns success or failure
   */
    bool setIntensity(double newIntensity);


    /*! Keeping track of the AffectiveState's likelihood.
   *  Updated automatically within update(float dt).
   */
    std::vector<AffectVertex*> likelihoods;

private:
    /*! At last we need an intensity value for the affective state that is continously updated by
   *  the decay function.
   * DEFAULT: 1.0, i.e. always on.
   */
    double intensity;

    void init();
};


#endif // _AFFECTIVESTATE_H_
