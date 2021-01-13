/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019 [WSL: Ubuntu]
** File description:
** main
*/

#include <iostream>
#include "Exceptions.hpp"
#include "Reception.hpp"

int main(int ac, char **av)
{
    if (ac != 4)
        return (84);

    int multiplier;
    int cooksNumber;
    int time;

    try {
        multiplier = std::stoi(av[1]);
        cooksNumber = std::stoi(av[2]);
        time = std::stoi(av[3]);

        if (cooksNumber < 1 || time < 0)
            throw plazza::ArgumentException("Wrong argument", "Argument");

    } catch (const std::invalid_argument &exception) {
        std::cerr << "Invalid argument: " << exception.what() << std::endl;
        return (84);
    } catch (const std::out_of_range &exception) {
        std::cerr << "Invalid argument: " << exception.what() << std::endl;
        return (84);
    } catch (plazza::ArgumentException &exception) {
        std::cerr << "Argument exception: " << exception.what() << std::endl;
        return (84);
    } catch (std::exception &exception) {
        std::cerr << "Unknown exception" << std::endl;
        return (84);
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return (84);
    }

    plazza::Reception reception(multiplier, cooksNumber, time);

    reception.shell();
    return (0);
}
