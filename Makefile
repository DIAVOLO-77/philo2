NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

SRC = main.c philo.c utils.c time.c init.c monitoring_routine.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	@$(RM) $(OBJ)

fclean: clean
	@$(RM) $(NAME)

re: fclean all