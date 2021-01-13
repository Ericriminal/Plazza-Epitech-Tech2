/*
** EPITECH PROJECT, 2020
** Storage.hpp
** File description:
** Storage
*/

#ifndef STORAGE_HPP_
#define STORAGE_HPP_

#include <chrono>
#include <map>
#include <vector>
#include "Pizzas.hpp"

class Storage {
    public :
        Storage(int reloadTimer);
        ~Storage();
        bool checkIngredient(PizzaType pizza);
        void removeIngredient(PizzaType pizza);
        void restockIngredients();
        void checkTimer();
        std::string getStock();
    private :
        std::chrono::time_point<std::chrono::system_clock> start;
        int reloadTimer;
        const std::vector<std::string> allIngredients = {"doe", "tomato", "gruyere", "ham", "mushrooms", "eggplant", "chiefLove", "goatCheese", "steak"};
        std::map<std::string, int> ingredients;
};

#endif
