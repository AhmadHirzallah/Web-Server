CXX     = c++
CXXFLAGS= -Wall -Wextra -Werror -std=c++98 -I. 	-g -O0
NAME    = parser_test

SRCS    = main.cpp Request.cpp RequestParser.cpp
OBJS    = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
