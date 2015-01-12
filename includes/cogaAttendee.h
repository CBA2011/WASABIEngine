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

#ifndef COGAATTENDEE_H
#define COGAATTENDEE_H

#include "WASABIEngine_global.h"
#include <iostream>

class WASABIENGINESHARED_EXPORT cogaAttendee{

public:
    cogaAttendee();
    virtual ~cogaAttendee();
    virtual void setName(std::string name) = 0;
    virtual std::string getName() {return _name;}
    virtual void setLocalID(int id) {localID = id;}
    virtual int getLocalID() {return localID;}
    virtual std::string getGlobalID() {return globalID;}
    virtual void setGlobalID(std::string newGlobalID) = 0;
    virtual std::string getOwner(){return owner;}
    virtual void setOwner(std::string newOwner){owner = newOwner;}
protected:
    std::string _name;
    int localID;
    std::string globalID;
    static int globalCount;
    std::string owner;
};
#endif
