#include "mutex.h"
Mutex::Mutex(){
       pthread_mutex_init(&mutex, 0);
   }
   Mutex::~Mutex(){
      pthread_mutex_destroy(&mutex);
  }
  void Mutex::lock(){
      pthread_mutex_lock(&mutex);
  }
  void Mutex::unlock(){
      pthread_mutex_unlock(&mutex);
  }
  bool Mutex::trylock()  {
      return (pthread_mutex_trylock(&mutex) == 0);
  }
