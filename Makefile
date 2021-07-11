SRCS		=	action.c \
				ft_atoi.c \
				ft_isdigit.c \
				ft_isspace.c \
				init.c \
				main.c \
				thread.c \
				utils.c \
				utils_monitor.c
OBJS		= ${SRCS:.c=.o}
CC			= clang
CFLAGS		= -Wall -Wextra -Werror -pthread
NAME		= philo
RM			= rm -f

%.o :		%.c
			${CC} ${CFLAGS} -c $<
all :		${NAME}
${NAME} :	${OBJS}
			clang -o ${NAME} ${OBJS} -pthread
clean :
			${RM} ${OBJS}
fclean :	clean
			${RM} ${NAME}
re :		fclean all
.PHONY :
			all ${NAME} clean fclean re
