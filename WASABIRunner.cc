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

#include <iostream>
#include "cogaEmotionalAttendee.h"

cogaEmotionalAttendee* ownEmoAttendee;
cogaEmotionalAttendee* otherEmoAttendee;
std::string Character_Name;
std::string Class;
std::string option;
bool filelogging;

using namespace std;

/*! WASABIRunner is just for testing and might
 *  serve as an example of how to use the library.
 */
int main(int argc, char* argv[])
{
    ownEmoAttendee = new cogaEmotionalAttendee();
    otherEmoAttendee = new cogaEmotionalAttendee();

    std::cout << "argc = " << argc << ", argv[0] = " << argv[0] << std::endl;
    cout << "Give command line option \"-with-filelogging\" to enable filelogging!" << endl;
    cout << "Give command line option \"-with-emotions:<Class>\" to set the emotion class to <Class>, i.e. primary or secondary!" << endl;
    cout << "Give command line option \"-with-agent:<Agentname>\" to set the Character_Name to <Agentname> for Communication!" << endl;
    Character_Name = "Max";
    Class = "primary";
    if (argc > 1){
        for (int i = 1; i < argc; ++i){
            option = argv[i];
            if (option != "-") {
                std::cout << "Option = " << option.c_str() << std::endl;
                if (option == "-with-filelogging"){
                    filelogging = true;
                    cout << "filelogging enabled" << endl;
                }
                unsigned int pos = option.find("-with-agent:", 0);
                if (pos != std::string::npos) { //(option == "Emma" || option == "EMMA") {
                    Character_Name = option.substr(12);
                    std::cout << "Character_Name set to \"" << Character_Name << "\"" << std::endl;
                }
                else {
                    cout << "Option: -with-agent not found" << endl;
                }
                unsigned int pos2 = option.find("-with-emotions:", 0);
                if (pos2 != std::string::npos) {
                    cout << "FOUND -with-emotions option!" << endl;
                    Class = option.substr(15);
                    std::cout << "Class set to \"" << Class << "\"" << std::endl;
                }
                else {
                    cout << "Option: -with-emotions not found" << endl;
                }
            }
        }
    }
    cout << "Character_Name = " << Character_Name << "!" << endl;
    ownEmoAttendee->setGlobalID(Character_Name.c_str());

    while (true){
        //System.sleep(1);
    }
    return 0;
}

