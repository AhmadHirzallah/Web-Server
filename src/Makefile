# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: drahwanj <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/29 20:25:04 by drahwanj          #+#    #+#              #
#    Updated: 2026/01/29 20:25:04 by drahwanj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=webserv

CXX=c++

CXXFLAGS=-Wall -Werror -Wextra -std=c++98

SRC=main.cpp

OBJ=$(SRC:.cpp=.o)

CONFIG_A=config_parser/libconfig.a

LINK=$(CONFIG_A)

all: config $(NAME)

$(NAME): $(CONFIG_A) $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) $(LINK)

config:
	make -C config_parser

clean:
	make -C config_parser clean
	rm -rf $(OBJ)

fclean: clean
	make -C config_parser fclean
	rm -rf $(NAME)
	

re: fclean all

.PHONY: all config clean fclean re