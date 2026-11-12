# **************************************************************************** #
#                                   CONFIG                                     #
# **************************************************************************** #

NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98
SRCS =	src/Client.cpp\
		src/CGI.cpp\
		src/Request.cpp\
		src/Response.cpp\
		src/Server.cpp\
		src/ConfigData.cpp\
		src/TokenStream.cpp\
		src/LocationBuilder.cpp\
		src/ServerBuilder.cpp\
		src/ConfigParser.cpp\
		src/main.cpp

HEADERS =	includes/Client.hpp\
			includes/CGI.hpp\
			includes/Request.hpp\
			includes/Response.hpp\
			includes/Server.hpp\
			includes/ConfigData.hpp\
			includes/TokenStream.hpp\
			includes/LocationBuilder.hpp\
			includes/ServerBuilder.hpp\
			includes/ConfigParser.hpp

OBJS = $(SRCS:.cpp=.o)

# **************************************************************************** #
#                                   RULES                                      #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(HEADERS)
	@$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean 