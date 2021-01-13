/*
** EPITECH PROJECT, 2020
** kitchen
** File description:
** kitchen
*/

#include "Kitchen.hpp"

void Kitchen::sendInfo(const char *info)
{
    write(this->_push_tube, info, 2048);
}

int Kitchen::readInfo()
{
    return read(this->_reception_tube, this->msgReceive, 2048);
}

Kitchen::Kitchen(int numberCooks, int bakeTimeMultiplier,
    int reloadTimer, int push_tube, int reception_tube)
:storage(reloadTimer)
{
    this->numberCooks = numberCooks;
    this->bakeTimeMultiplier = bakeTimeMultiplier;
    this->shutdown = false;
    this->_push_tube = push_tube;
    this->_reception_tube = reception_tube;
    this->idleStart = std::chrono::system_clock::now();
    for (int count = 0; count < this->numberCooks; count++) {
        this->cookStatus[count] = 0;
    }
}

std::string Kitchen::getPizzaName(PizzaType pizzaType)
{
    switch (pizzaType) {
        case PizzaType::Regina :
            return ("Regina");
        case PizzaType::Margarita :
            return ("Margarita");
        case PizzaType::Americana :
            return ("Americana");
        case PizzaType::Fantasia :
            return ("Fantasia");
        default :
            return ("error");
    }
}

std::string Kitchen::getPizzaSize(PizzaSize pizzaSize)
{
    switch (pizzaSize) {
        case PizzaSize::S :
            return ("S");
        case PizzaSize::M :
            return ("M");
        case PizzaSize::L :
            return ("L");
        case PizzaSize::XL :
            return ("XL");
        case PizzaSize::XXL :
            return ("XXL");
        default :
            return ("error");
    }
}

int Kitchen::getWaitTime(PizzaType pizzaType)
{
    switch (pizzaType) {
        case PizzaType::Regina :
            return 2 * this->bakeTimeMultiplier;
        case PizzaType::Margarita :
            return 1 * this->bakeTimeMultiplier;
        case PizzaType::Americana :
            return 2 * this->bakeTimeMultiplier;
        case PizzaType::Fantasia :
            return 4 * this->bakeTimeMultiplier;
        default :
            return 0;
    }
}

void Kitchen::write_in_log(std::string type, std::string name)
{
    std::string all = type + " " + name + "\n";
    FILE *fp;
    fp=fopen("log","a");
    fputs(all.c_str(), fp);
    fclose(fp);
}

void Kitchen::doCook(mutex &mutex, const int cookId)
{
    bool doingPizza = false;
    bool hasIngredient = false;
    int waitTime = 0;
    PizzaType pizzaType;
    PizzaSize pizzaSize;
    auto start = std::chrono::system_clock::now();
    auto now = std::chrono::system_clock::now();

    while (!shutdown) {
        mutex.lock();
        if (!doingPizza && this->pizzasOrder.size() != 0) {
            doingPizza = true;
            pizzaType = PizzaType(this->pizzasOrder.back().front());
            pizzaSize = PizzaSize(this->pizzasOrder.back().back());
            this->pizzasOrder.pop_back();
            waitTime = this->getWaitTime(pizzaType);
            this->cookStatus[cookId] = 1;
            hasIngredient = this->storage.checkIngredient(pizzaType);
            start = std::chrono::system_clock::now();
        } else if (doingPizza) {
            if (hasIngredient) {
                now = std::chrono::system_clock::now();
                if (std::chrono::duration_cast<std::chrono::seconds>(now-start).count() >= waitTime) {
                    doingPizza = false;
                    hasIngredient = false;
                    this->cookStatus[cookId] = 0;
                    write_in_log(this->getPizzaName(pizzaType), this->getPizzaSize(pizzaSize));
                    std::cout << "Pizza " << this->getPizzaName(pizzaType) << " "
                    << this->getPizzaSize(pizzaSize) << " done." << std::endl;
                }
            } else {
                hasIngredient = this->storage.checkIngredient(pizzaType);
                start = std::chrono::system_clock::now();
            }
        } else {
            // yield
        }
        mutex.unlock();
    }
}

std::string Kitchen::getStatusCook()
{
    std::string statusCook = "\n";

    for (int count = 1; this->numberCooks  >= count; count++) {
        if (this->cookStatus[count - 1] == 1)
            statusCook += "Cook nb." + std::to_string(count) + " is active.\n";
        else
            statusCook += "Cook nb." + std::to_string(count) + " is inactive.\n";
    }
    return statusCook;
}

void Kitchen::sendStatus()
{
    std::string status;

    status = this->storage.getStock();
    status += this->getStatusCook();
    this->sendInfo(status.c_str());
}

void Kitchen::addOrder(int pizza, int size)
{
    this->pizzasOrder.push_back(std::vector<int> {pizza, size});
}

void Kitchen::getPizza()
{
    int pizzaType = 0;
    int pizzaSize = 0;
    std::stringstream ss(std::string(this->msgReceive));
    std::string tmp;
    std::string tmp2;

    ss >> tmp;
    ss >> tmp2;
    pizzaType = std::stoi(tmp);
    if (pizzaType == 0) {
        return;
    }
    pizzaSize = std::stoi(tmp2);
    if (pizzaSize == 0) {
        return;
    }
    this->addOrder(pizzaType, pizzaSize);
    this->sendInfo("y");
}

void Kitchen::getOrders()
{
    if (readInfo() < 1)
        return;

    if (strncmp(this->msgReceive, "status", 6) == 0)
        this->sendStatus();
    else if (strncmp(this->msgReceive, "exit", 4) == 0)
        this->shutdown = true;
    else {
        if (this->pizzasOrder.size() >= this->numberCooks * 2 - this->getCookActivity())
            this->sendInfo("n");
        else
            this->getPizza();
    }
}

int Kitchen::getCookActivity()
{
    int workingCook = 0;
    for (int count = 0; count < this->numberCooks; count++)
        if (this->cookStatus[count] == 1)
            workingCook++;
    return workingCook;
}

bool Kitchen::checkCookActivity()
{
    for (int count = 0; count < this->numberCooks; count++)
        if (this->cookStatus[count] == 1)
            return true;
    return false;
}

void Kitchen::checkActivity()
{
    if (this->pizzasOrder.size() == 0 && !this->checkCookActivity()) {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now-idleStart).count() >= 5) {
            this->shutdown = true;
            this->sendInfo("d");
        }
    } else
        this->idleStart = std::chrono::system_clock::now();
}

void Kitchen::kitchenInstance()
{
    std::vector<std::thread> cooks;
    mutex newMutex;

    for (int count = 0; count < this->numberCooks; count++)
        cooks.push_back(std::thread(&Kitchen::doCook, this, std::ref(newMutex), count));
    while (!shutdown) {
        this->getOrders();
        this->checkActivity();
        this->storage.checkTimer();
    }
    for (auto &eachThread : cooks)
        eachThread.join();
    std::cout << "Kitchen is closed" << std::endl;
}
