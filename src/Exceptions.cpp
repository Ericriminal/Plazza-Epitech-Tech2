/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019 [WSL: Ubuntu]
** File description:
** Exceptions
*/

#include "Exceptions.hpp"

plazza::PlazzaException::PlazzaException(const std::string &message,
const std::string &component) {
    this->_message = message;
    this->_component = component;
}

const std::string &plazza::PlazzaException::getComponent() const {
    return (this->_component);
}

const char *plazza::PlazzaException::what() const noexcept {
    return (this->_message.c_str());
}

plazza::ReceptionException::ReceptionException(const std::string &message,
const std::string &component): plazza::PlazzaException(message, component) {
}

plazza::ArgumentException::ArgumentException(const std::string &message,
const std::string &component): plazza::PlazzaException(message, component) {
}
