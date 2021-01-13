/*
** EPITECH PROJECT, 2020
** Storage.cpp
** File description:
** Storage
*/

#include "Storage.hpp"

Storage::Storage(int reloadTimer)
{
    this->reloadTimer = reloadTimer;
    this->start = std::chrono::system_clock::now();
    this->ingredients.insert(std::pair<std::string,int>("doe", 5));
    this->ingredients.insert(std::pair<std::string,int>("tomato", 5));
    this->ingredients.insert(std::pair<std::string,int>("gruyere", 5));
    this->ingredients.insert(std::pair<std::string,int>("ham", 5));
    this->ingredients.insert(std::pair<std::string,int>("mushrooms", 5));
    this->ingredients.insert(std::pair<std::string,int>("steak", 5));
    this->ingredients.insert(std::pair<std::string,int>("eggplant", 5));
    this->ingredients.insert(std::pair<std::string,int>("goatCheese", 5));
    this->ingredients.insert(std::pair<std::string,int>("chiefLove", 5));
}

Storage::~Storage()
{

}

void Storage::restockIngredients()
{
    for (std::string ingredient : this->allIngredients) {
        if (this->ingredients.find(ingredient)->second != 5)
            this->ingredients.find(ingredient)->second++;
    }
}

void Storage::checkTimer()
{
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

    if (elapsed_seconds >= this->reloadTimer) {
        this->start = now;
        this->restockIngredients();
    }
}

bool Storage::checkIngredient(PizzaType pizza)
{
    switch (pizza) {
        case PizzaType::Regina :
            if (this->ingredients.find("doe")->second > 0
                && this->ingredients.find("tomato")->second > 0
                && this->ingredients.find("gruyere")->second > 0
                && this->ingredients.find("ham")->second > 0
                && this->ingredients.find("mushrooms")->second > 0) {
                    this->removeIngredient(pizza);
                    return true;
                }
            else
                return false;
            break;
        case PizzaType::Margarita :
            if (this->ingredients.find("doe")->second > 0
                && this->ingredients.find("tomato")->second > 0
                && this->ingredients.find("gruyere")->second > 0) {
                    this->removeIngredient(pizza);
                    return true;
                }
            else
                return false;
            break;
        case PizzaType::Americana :
            if (this->ingredients.find("doe")->second > 0
                && this->ingredients.find("tomato")->second > 0
                && this->ingredients.find("gruyere")->second > 0
                && this->ingredients.find("steak")->second > 0) {
                    this->removeIngredient(pizza);
                    return true;
                }
            else
                return false;
            break;
        case PizzaType::Fantasia :
            if (this->ingredients.find("doe")->second > 0
                && this->ingredients.find("tomato")->second > 0
                && this->ingredients.find("eggplant")->second > 0
                && this->ingredients.find("goatCheese")->second > 0
                && this->ingredients.find("chiefLove")->second > 0) {
                    this->removeIngredient(pizza);
                    return true;
                }
            else
                return false;
            break;
        default :
            return false;
    }
}

void Storage::removeIngredient(PizzaType pizza)
{
    switch (pizza) {
        case PizzaType::Regina :
            this->ingredients.find("doe")->second--;
            this->ingredients.find("tomato")->second--;
            this->ingredients.find("gruyere")->second--;
            this->ingredients.find("ham")->second--;
            this->ingredients.find("mushrooms")->second--;
            break;
        case PizzaType::Margarita :
            this->ingredients.find("doe")->second--;
            this->ingredients.find("tomato")->second--;
            this->ingredients.find("gruyere")->second--;
            break;
        case PizzaType::Americana :
            this->ingredients.find("doe")->second--;
            this->ingredients.find("tomato")->second--;
            this->ingredients.find("gruyere")->second--;
            this->ingredients.find("steak")->second--;
            break;
        case PizzaType::Fantasia :
            this->ingredients.find("doe")->second--;
            this->ingredients.find("tomato")->second--;
            this->ingredients.find("eggplant")->second--;
            this->ingredients.find("goatCheese")->second--;
            this->ingredients.find("chiefLove")->second--;
            break;
    }
}

std::string Storage::getStock()
{
    std::string stock;

    stock = "doe -> " + std::to_string(this->ingredients.find("doe")->second) + "\n";
    stock += "tomato -> " + std::to_string(this->ingredients.find("tomato")->second) + "\n";
    stock += "gruyere -> " + std::to_string(this->ingredients.find("gruyere")->second) + "\n";
    stock += "ham -> " + std::to_string(this->ingredients.find("ham")->second) + "\n";
    stock += "mushrooms -> " + std::to_string(this->ingredients.find("mushrooms")->second) + "\n";
    stock += "steak -> " + std::to_string(this->ingredients.find("steak")->second) + "\n";
    stock += "eggplant -> " + std::to_string(this->ingredients.find("eggplant")->second) + "\n";
    stock += "goatCheese -> " + std::to_string(this->ingredients.find("goatCheese")->second) + "\n";
    stock += "chiefLove -> " + std::to_string(this->ingredients.find("chiefLove")->second) + "\n";
    return stock;
}
