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

#ifndef _PRIMARYEMOTION_H_
#define _PRIMARYEMOTION_H_
#include <iostream>
#include "AffectiveState.h"
#include "SecondaryEmotion.h"
#include "WASABIEngine_global.h"


//----------------------------------------------------
// Klasse: PrimaryEmotion
// Vaterklassen: AffectiveState
//----------------------------------------------------
class WASABIENGINESHARED_EXPORT PrimaryEmotion : public AffectiveState
{  
public :

    //* Default Constructor
    PrimaryEmotion();
    virtual ~PrimaryEmotion();

    PrimaryEmotion(std::vector<AffectPolygon*> ap_vec);
    PrimaryEmotion(AffectPolygon* ap);

    //* If a secondary emotion wants to use the primary emotion, this function has to be called
    bool addToSecondaryEmotion(SecondaryEmotion* se);

protected:
    //* Here we might track the connections to secondary emotions
    std::vector<SecondaryEmotion*> usedBySecondaryEmotions;

private:
    void init();
};

#endif // _PRIMARYEMOTION_H_
