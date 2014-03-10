#ifndef MUTEX_H
#define MUTEX_H

class Mutex
{
public:
    Mutex();

    bool trylock();
    void unlock();

private:
    int counter;
};

#endif // MUTEX_H
