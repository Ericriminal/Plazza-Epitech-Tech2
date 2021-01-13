/*
** EPITECH PROJECT, 2020
**
** File description:
**
*/

#include "mutex.hpp"


mutex::mutex()
{
}

mutex::~mutex()
{
}

void mutex::lock()
{
    this->mutexValue.lock();
}

void mutex::unlock()
{
    this->mutexValue.unlock();
}

void mutex::trylock()
{
    this->mutexValue.try_lock();
}
