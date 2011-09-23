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

#ifndef _SECONDARYEMOTION_H_
#define _SECONDARYEMOTION_H_
#include "AffectiveState.h"
#include "WASABIEngine_global.h"


class WASABIENGINESHARED_EXPORT PrimaryEmotion;
//----------------------------------------------------
// Klasse: SecondaryEmotion
// Vaterklassen: AffectiveState
//----------------------------------------------------
class WASABIENGINESHARED_EXPORT SecondaryEmotion : public AffectiveState
{  
public :

    //* Default Constructor
    SecondaryEmotion();
    virtual ~SecondaryEmotion();

    SecondaryEmotion(std::vector<AffectPolygon*> ap_vec);
    SecondaryEmotion(AffectPolygon* ap);

    bool loadFromFile(std::string filename);

    //* If a primary emotion is added, this function has to be called
    void addPrimaryEmotion(PrimaryEmotion* pe);

protected:
    //* Here we track the connections to primary emotions
    std::vector<PrimaryEmotion*> usesPrimaryEmotions;
};

#endif // _SECONDARYEMOTION_H_
