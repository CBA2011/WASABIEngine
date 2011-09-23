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

#include  "PrimaryEmotion.h"

using namespace std;

PrimaryEmotion::PrimaryEmotion()
{
    init();
}

PrimaryEmotion::PrimaryEmotion(std::vector<AffectPolygon*> ap_vec)
    : AffectiveState(ap_vec)
{
    init();
}

PrimaryEmotion::PrimaryEmotion(AffectPolygon* ap)
    : AffectiveState(ap)
{
    init();
}

PrimaryEmotion::~PrimaryEmotion()
{
    // nothing
}

void
PrimaryEmotion::init() {
    baseIntensity = 0.5;
    cout << "PrimaryEmotion::init()" << endl;
}

bool
PrimaryEmotion::addToSecondaryEmotion(SecondaryEmotion* se) {
    // we might want to check for conflicts here before adding.
    se->addPrimaryEmotion(this);
    usedBySecondaryEmotions.push_back(se);
    return true;
}
