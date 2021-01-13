/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** Reception
*/

#ifndef RECEPTION_HPP_
#define RECEPTION_HPP_

#include <utility>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <cstdlib>
#include <fcntl.h>
#include "Exceptions.hpp"
#include "Kitchen.hpp"

namespace plazza {
    class Reception {
        public:
            Reception(int multiplier, int cooksNumber, int time);
            ~Reception() = default;
            void shell();
        private:
            std::string getStatus();
            bool parse(const std::string &userInput);
            void doKitchen();
            void removeDeadKitchens(mutex &mutex);
            size_t readInfo(int where);
            void sendInfo(int where, const std::string &info);
            void explodeCommands(const std::string &userInput, std::vector<std::string> &commands);
            void create_file_log();
            char _msgReceive[2048];
            std::vector<std::unordered_map<std::string, int>> _pipesInfo;
            std::unordered_map<std::string, int> _pizzasTypes;
            std::unordered_map<std::string, int> _pizzasSizes;
            int _multiplier;
            int _cooksNumber;
            int _time;
            bool canCheckDeath;
            bool shutdown;
    };
}

#endif /* !RECEPTION_HPP_ */
