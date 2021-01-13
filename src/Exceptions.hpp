/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** Exceptions
*/

#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

#include <exception>
#include <string>

namespace plazza {
    class PlazzaException: public std::exception {
        public:
            PlazzaException(
                const std::string &message, const std::string &component
            );

            const std::string &getComponent() const;

            const char *what() const noexcept override;

        protected:
            std::string _message;
            std::string _component;
    };

    class ReceptionException: public PlazzaException {
        public:
            ReceptionException(
                const std::string &message, const std::string &component
            );
    };

    class ArgumentException: public PlazzaException {
        public:
            ArgumentException(
                const std::string &message, const std::string &component
            );
    };
};

#endif /* !EXCEPTIONS_HPP_ */
