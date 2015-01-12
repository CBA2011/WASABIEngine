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

#ifndef EmotionConverter_H
#define EmotionConverter_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <math.h>
//#include <aceMultimodalFigure/EmotionTypes.h>
#include "AffectiveState.h"
#include "WASABIEngine_global.h"



/*!
 * Abstrakte Basisklasse fuer die Abbildung vom Dynamik-Raum in einen Raum zur Kategorisierung
 */
class WASABIENGINESHARED_EXPORT EmotionConverter
{

public:
    EmotionConverter ();
    virtual ~EmotionConverter ();
    virtual std::vector<int> convertToClassType( std::vector<int> ) = 0;
    virtual std::vector<float> convertFromClassType( std::vector<float> ) = 0;
};


/*!
 * Abbildung vom Dynamik-Raum in den PAD-Raum nach Mehrabian
 */
class WASABIENGINESHARED_EXPORT EmotionConverterPAD: public EmotionConverter
{

public:
    EmotionConverterPAD ();
    ~EmotionConverterPAD ();

    std::vector<int> convertToClassType( std::vector<int> foreignData);
    std::vector<float> convertFromClassType( std::vector<float> PADData);
    std::string toString( std::vector<int> PADData);

    // The variables below are now obsolete.
    // The AffectiveStates keep track of this data themselves.
    /*   int emoIndex; // indicating the index of the nearest emotional term */
    /*   float emoDistance; // holding the distance to actual PAD term */
    /*   float outerRadius, innerRadius; */
    /*   StringVectorMMap stringConvertMap; */

};

#endif


