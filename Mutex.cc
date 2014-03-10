#include "mutex.h"

Mutex::Mutex()
{
   counter = 0;
}

bool Mutex::trylock(){

    counter++;
    if(counter > 1){
        counter--;
        return false;
    }

    return true;
}

void Mutex::unlock(){
    counter--;
}
