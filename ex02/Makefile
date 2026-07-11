# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yolaoual <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/17 15:57:39 by yolaoual          #+#    #+#              #
#    Updated: 2025/11/17 15:57:45 by yolaoual         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = PmergeMe
SRCS =  PmergeMe.cpp main.cpp
OBJS = ${SRCS:.cpp=.o}
CMD = c++
CMDFLAGS = -Wall -Wextra -Werror -I. -std=c++98

all: $(NAME)

.cpp.o:
	$(CMD) $(CMDFLAGS) -c $< -o $(<:.cpp=.o)

$(NAME): $(OBJS)
	$(CMD) $(CMDFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY:
