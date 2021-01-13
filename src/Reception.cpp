/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019 [WSL: Ubuntu]
** File description:
** Reception
*/

#include "Reception.hpp"

void plazza::Reception::create_file_log()
{
    FILE *fp;
    fp=fopen("log","w+");
    fclose(fp);
}

plazza::Reception::Reception(int multiplier, int cooksNumber, int time) {
    create_file_log();
    this->_pizzasTypes["regina"] = 1;
    this->_pizzasTypes["margarita"] = 2;
    this->_pizzasTypes["americana"] = 4;
    this->_pizzasTypes["fantasia"] = 8;

    this->_pizzasSizes["s"] = 1;
    this->_pizzasSizes["m"] = 2;
    this->_pizzasSizes["l"] = 4;
    this->_pizzasSizes["xl"] = 8;
    this->_pizzasSizes["xxl"] = 16;

    this->_multiplier = multiplier;
    this->_cooksNumber = cooksNumber;
    this->_time = time;
    this->shutdown = false;
    this->canCheckDeath = false;
}

void plazza::Reception::sendInfo(int where, const std::string &info) {
    write(where, info.c_str(), 2048);
}

size_t plazza::Reception::readInfo(int where) {
    if (read(where, this->_msgReceive, 2048) < 1)
        return (1);
    return (0);
}

void plazza::Reception::doKitchen() {
    int statusPipe[2];
    int commandPipe[2];

    if (pipe(statusPipe) != 0 || pipe(commandPipe) != 0) {  // named pipe
        throw;
    }
    pid_t pid = fork();
    if (pid == -1) {
        throw;
    }
    else if (pid == 0) {
        Kitchen kitchen(this->_cooksNumber, this->_multiplier, this->_time, statusPipe[1], commandPipe[0]);
        fcntl(commandPipe[0], F_SETFL, O_NONBLOCK);
        fcntl(statusPipe[1], F_SETFL, O_NONBLOCK);
        kitchen.kitchenInstance();
        exit(0);
    } else {
        std::unordered_map<std::string, int> pipeInfos;
        pipeInfos["read"] = statusPipe[0];
        pipeInfos["write"] = commandPipe[1];
        fcntl(commandPipe[1], F_SETFL, O_NONBLOCK);
        fcntl(statusPipe[0], F_SETFL, O_NONBLOCK);
        this->_pipesInfo.push_back(pipeInfos);
    }
}

void plazza::Reception::explodeCommands(const std::string &userInput, std::vector<std::string> &commands) {
    std::stringstream data(userInput);
    std::string command;

    while (std::getline(data, command, ';')) {
        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) { return (std::tolower(c)); });
        commands.push_back(command);
    }
}

std::string plazza::Reception::getStatus() {
    std::string status;

    for (int kitchenNumber = 0; kitchenNumber < this->_pipesInfo.size(); kitchenNumber++) {
        this->sendInfo(this->_pipesInfo[kitchenNumber]["write"], "status");
        while (this->readInfo(this->_pipesInfo[kitchenNumber]["read"]) == 1);
        status += "\n\nKitchen " + std::to_string(kitchenNumber) + "\n" + std::string(this->_msgReceive) + "\n";
    }
    return (status);
}

void plazza::Reception::removeDeadKitchens(mutex &mutex) {
    while (!this->shutdown) {
        if (this->canCheckDeath) {
            for (int i = 0; i < this->_pipesInfo.size(); i++) {
                if (this->readInfo(this->_pipesInfo[i]["read"]) == 0 && this->_msgReceive[0] == 'd') {
                    this->_pipesInfo.erase(this->_pipesInfo.begin() + i);
                    i -= 1;
                }
            }
        }
    }
}

bool plazza::Reception::parse(const std::string &userInput) {
    if (userInput == "exit") {
        this->canCheckDeath = false;
        for (int i = 0; i < this->_pipesInfo.size(); i++)
            this->sendInfo(this->_pipesInfo[i]["write"], "exit");
        return (false);
    }
    else if (userInput == "status") {
        this->canCheckDeath = false;
        std::string status = this->getStatus();
        std::cout << status << std::endl;
        this->canCheckDeath = true;
        return (true);
    }

    std::regex command_pattern("[\\s]*(regina|margarita|americana|fantasia)[\\s]+(s|m|xl|xxl)[\\s]+x[1-9][0-9]*[\\s]*");
    std::vector<std::string> commands;

    this->explodeCommands(userInput, commands);
    for (const std::string &command : commands) {
        std::stringstream ss(command);
        std::string pizzaType;
        std::string pizzaSize;
        std::string temp;

        ss >> pizzaType;
        ss >> pizzaSize;
        ss >> temp;

        int pizzasNumber = std::stoi(temp.substr(temp.find("x") + 1, temp.length()));

        if (std::regex_match(command, command_pattern) == false)
            continue;

        this->canCheckDeath = false;
        for (int i = 0; i < pizzasNumber; i++) {
            kitchen_loop:
            int kitchenNumber = 0;

            for (; kitchenNumber < this->_pipesInfo.size(); kitchenNumber++) {
                std::string test(std::to_string(this->_pizzasTypes[pizzaType]) + " " + std::to_string(this->_pizzasSizes[pizzaSize]));
                this->sendInfo(this->_pipesInfo[kitchenNumber]["write"], test);
                while (this->readInfo(this->_pipesInfo[kitchenNumber]["read"]) == 1);
                if (this->_msgReceive[0] == 'y') {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    break;
                }
            }
            if (kitchenNumber == this->_pipesInfo.size()) {
                this->doKitchen();
                goto kitchen_loop;
            }
        }
    }
    this->canCheckDeath = true;
    return (true);
}

void plazza::Reception::shell() {
    bool isRunning = true;
    std::string userInput;
    mutex mutex;
    std::thread catchDeathKitchen(&plazza::Reception::removeDeadKitchens, this, std::ref(mutex));

    std::cout << "Special Command : \nexit : leave the program\nstatus : give status of kitchen\n" << std::endl;
    std::cout << "> What's gonna be your order?" << std::endl;
    while (isRunning && std::getline(std::cin, userInput)) {
        try {
            isRunning = this->parse(userInput);
        } catch (std::exception) {
            std::cout << "Invalid command" << std::endl;
        }
    }
    this->shutdown = true;
    catchDeathKitchen.join();
    std::cout << "Have a wonderful day sir." << std::endl;
}
