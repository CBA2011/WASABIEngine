#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>

class Thread
{
    private:
        pthread_t thread;
        static void * dispatch(void *);

    protected:
        virtual void run() = 0;

    public:
        void start();
        void join();
        virtual ~Thread();
};

#endif
