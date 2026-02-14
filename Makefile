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

SRC=main.cpp                         \
	config_parser/Config.cpp         \
	config_parser/Tokens.cpp         \
	config_parser/LocationConfig.cpp \
	config_parser/ServerConfig.cpp   \
	event_handler/EventHandler.cpp   \
	event_handler/Socket.cpp         \
	event_handler/ClientSocket.cpp   \
	event_handler/ListenSocket.cpp   \
	event_handler/SocketFactory.cpp  \
	event_handler/SignalSocket.cpp   \
	http/Request.cpp                 \
	http/RequestParser.cpp

SOURCES=$(addprefix src/, $(SRC))

INC=config_parser/ event_handler/ http/

INCLUDES=$(addprefix -I src/, $(INC))

OBJ=$(SOURCES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all config clean fclean re