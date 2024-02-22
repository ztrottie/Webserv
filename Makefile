NAME	=	webserv

CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -std=c++98 -Wno-shadow -Wshadow

SRC_DIR	=	src/
BIN_DIR	=	bin/


SRCS	=	main.cpp \
			Server.cpp \
			Webserv.cpp \
			parsing.cpp \
			utils.cpp \
			Router.cpp\
			Location.cpp\
			Request.cpp\
			parsing_utils.cpp\
			assignation_parsing.cpp\
			parsing_location.cpp
			Response.cpp

OBJS	=	$(addprefix $(BIN_DIR), ${SRCS:.cpp=.o})

$(BIN_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

all: $(BIN_DIR) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)	-o $(NAME)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

clean:
	@rm -fr $(BIN_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

run: re all
	@./webserv

.PHONY:	all clean fclean re