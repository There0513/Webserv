SRCS	=	srcs/main.cpp\
			srcs/Sockets/SimpleSocket.cpp\
			srcs/Sockets/ConnectingSocket.cpp\
			srcs/Sockets/BindingSocket.cpp\
			srcs/Sockets/ListeningSocket.cpp\
			srcs/Server/SimpleServer.cpp\
			srcs/Server/testServer.cpp\
			srcs/Config/parseConfig.cpp

OBJS	=	${SRCS:.c=.o}

CXX		=	g++

CFLAGS	=	-std=c++11 #-Wall -Werror -Wextra #-std=c++98

NAME	=	webserv

${NAME}	:	${OBJS}
		${CXX} ${CFLAGS} ${OBJS} -o ${NAME} 

all		:	${NAME}

clean	:

fclean	:	clean
			rm -rf ${NAME}
			$(MAKE) -C . clean

re	:	fclean all

.PHONY	:	all clean fclean re
