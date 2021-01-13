/*
** EPITECH PROJECT, 2020
**
** File description:
**
*/

#ifndef KITCHEN_HPP_
#define KITCHEN_HPP_

#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include "Storage.hpp"
#include "mutex.hpp"

class Kitchen {
    public:
        Kitchen(int numberCooks, int bakeTimeMultiplier, int reloadTimer, int push_tube, int reception_tube);
        ~Kitchen() = default;
        void kitchenInstance();
    private:
        int readInfo();
        void sendInfo(const char *info);
        void sendStatus();
        void addOrder(int pizza, int size);
        void getPizza();
        void doCook(mutex &mutex, const int cookId);
        void getOrders();
        void checkActivity();
        void write_in_log(std::string type, std::string name);
        std::string getPizzaName(PizzaType pizzaType);
        std::string getPizzaSize(PizzaSize pizzaSize);
        int getWaitTime(PizzaType pizzaType);
        std::string getStatusCook();
        bool checkCookActivity();
        int getCookActivity();
        Storage storage;
        int _push_tube;
        int _reception_tube;
        FILE *fp;
        std::chrono::time_point<std::chrono::system_clock> idleStart;
        std::vector<std::vector<int>> pizzasOrder;
//        std::map<int, int> cookStatus;
        std::unordered_map<int, int> cookStatus;
        int numberCooks;
        int bakeTimeMultiplier;
        bool shutdown;
        char msgReceive[2048];
};

#endif /* !KITCHEN_HPP_ */
