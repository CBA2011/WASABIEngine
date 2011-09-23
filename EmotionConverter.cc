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

#include "EmotionConverter.h"
#include <stdlib.h>

using namespace std;

/*!
 * Leerer Konstruktor.
 */
EmotionConverter::EmotionConverter()
{
}

/*!
 * Leerer Destruktor.
 */
EmotionConverter::~EmotionConverter()
{
}

/*!
 * Initialisierung der stringConvertMap
 */
EmotionConverterPAD::EmotionConverterPAD()
    : EmotionConverter()
{
    //nothing
}

/*!
 * Leerer Destruktor.
 */
EmotionConverterPAD::~EmotionConverterPAD ()
{
}

// /*!
//  * Hier wird die Abbildung K(xt, yt, zt, t), wie in der Diploamarbeit beschrieben, vorgenommen.
//  */
// std::vector<float>
// EmotionConverterPAD::convertToClassType(std::vector<float> foreignData)
// {
//   std::vector<float> v;
//   if (foreignData.size() == 4){
//     float emoX = foreignData[0];
//     float moodY = foreignData[1];
//     float boredomZ = foreignData[2];
//     float dominance = foreignData[3];

//     float pValue = ((emoX + moodY) / 200);
//     float aValue = fabs(emoX / 100) - fabs(boredomZ / 100);
//     float dValue = dominance / 100;

//     v.push_back(pValue);
//     v.push_back(aValue);
//     v.push_back(dValue);
//   }
//   return v;
// }

/*!
 * Hier wird die Abbildung K(xt, yt, zt, t), wie in der Diploamarbeit beschrieben, vorgenommen.
 * NEW: Integer values are sufficient!
 */
std::vector<int>
EmotionConverterPAD::convertToClassType(std::vector<int> foreignData)
{
    std::vector<int> v;
    if (foreignData.size() == 4){
        int emoX = foreignData[0];
        int moodY = foreignData[1];
        int boredomZ = foreignData[2];
        int dominance = foreignData[3];

        int pValue = ((emoX + moodY) / 2);
        // 19th of May 2009: next line was "int aValue = emoX - boredomZ;" before.
        // But boredomZ is negatively signed after EmotionDynamics::update and we need to use the absolute value of emoX, of course.
        // Why did this ever work with this bug before?
        int aValue = abs(emoX) + boredomZ;
        int dValue = dominance;

        v.push_back(pValue);
        v.push_back(aValue);
        v.push_back(dValue);
    }
    return v;
}

/*!
 * We try our best here.
 */
std::vector<float>
EmotionConverterPAD::convertFromClassType(std::vector<float> PADData)
{
    std::vector<float> v;
    if (PADData.size() >= 3) {
        int p = (int)((float)PADData[0] * 100);
        int a = (int)((float)PADData[1] * 100);
        int d = (int)((float)PADData[2] * 100);
        int x = 0;
        int y = 0;
        int z = 0;
        if (a < 0) {
            z = a;
        }
        else {
            x = a; // Must be +/- a!
        }
        y = 2 * p;
        if (y + x > 100)
            x *= -1;
        else {
            if (y - x > -100) {
                cerr << "EmotionConverterPAD::convertFromClassType: We have a problem!" << endl;
                if (y < 0)
                    x *= -1;
            }
        }
        y += x;
        x *= -1;
        cout << "Matching PAD (" << p << ", " << a << ", " << d << ") to XYZ (" << x << ", " << y << ", " << z << ")" << endl;
        v.push_back(x);
        v.push_back(y);
        v.push_back(z);
    }
    else
        cerr << "EmotionConverterPAD::convertFromClassType: wrong size of input vector!" << endl;
    //   cerr << "WARNING: EmotionConverterPAD::convertFromClassType not implementable!" << endl;
    return v;
}

/*!
 * Liefert den Namen der Emotionkategorie, die am naechsten liegt!
 */
/* WIN32 obsolete!!!
std::string
EmotionConverterPAD::toString(std::vector<int> PADData){
  std::string s = "verwirrt";
  cout << "EmotionConverterPAD::toString: WARNING This function is obsolete! *********************************" << endl;
//   int counter = 0;
//   emoIndex = -1;
//   if (PADData.size() == 3){
//     StringVectorMMap::iterator it;
//     emoDistance = 10.0;
//     for (it = stringConvertMap.begin(); it != stringConvertMap.end(); ++it){
//       float a = (float)PADData[0] / 100 - it->second[0];
//       float b = (float)PADData[1] / 100 - it->second[1];
//       float c = (float)PADData[2] / 100 - it->second[2];
//       float tempDistance = sqrt(pow(c, 2) + pow(a, 2) + pow(b, 2));
//       //cout << "a = " << a << ", b = " << b << ", c = " << c  << "--> tempDistance = " << tempDistance << " bei " << it->first << endl;
//       if (tempDistance <= emoDistance && tempDistance <= outerRadius){
// 		s = it->first;
// 		emoDistance = tempDistance;
// 		emoIndex = counter;
//       }
//       ++counter;
//     }
//   }
  //cout << "EmotionConverterPAD::toString: s = " << s << endl;
  return s;
}
*/

