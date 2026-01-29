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

NAME=libconfig.a

CXX=c++

CXXFLAGS= -Wall -Werror -Wextra -std=c++98

SRC=LocationConfig.cpp ServerConfig.cpp Tokens.cpp Config.cpp

OBJ=$(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re