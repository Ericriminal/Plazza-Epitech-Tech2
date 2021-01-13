##
## EPITECH PROJECT, 2020
## CCP_plazza_2019 [WSL: Ubuntu]
## File description:
## Makefile
##

NAME	=	plazza

RM	=	rm -rf

CXX	=	g++

SRC	=	./src/main.cpp		\
		./src/Exceptions.cpp	\
		./src/Reception.cpp	\
		./src/Kitchen.cpp	\
		./src/Storage.cpp	\
		./src/mutex.cpp

OBJ	=	$(SRC:.cpp=.o)

CXXFLAGS	=

CPPFLAGS	=

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CXX) -o $(NAME) $(OBJ) -lpthread

clean:
	$(RM) $(NAME)

fclean:	clean
	$(RM) $(OBJ)

re:	fclean all

.PHONY:	all clean fclean $(NAME)
