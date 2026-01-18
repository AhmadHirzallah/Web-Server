# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/16 22:39:35 by drahwanj          #+#    #+#              #
#    Updated: 2026/01/16 22:39:36 by drahwanj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=configparser

CXX=c++

CXXFLAGS= -Wall -Werror -Wextra -std=c++98 -DPARSE_DEBUG

SRC=main.cpp LocationConfig.cpp ServerConfig.cpp ConfigParser.cpp

OBJ=$(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re