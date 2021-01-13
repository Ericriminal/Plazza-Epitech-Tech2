/*
** EPITECH PROJECT, 2020
**
** File description:
**
*/

#ifndef _mutex_
#define _mutex_

#include <mutex>

class mutex {
    public :
        mutex();
        ~mutex();
        void lock();
        void unlock();
        void trylock();
    private :
        std::mutex mutexValue;
};

#endif
