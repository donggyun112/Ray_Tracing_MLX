# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/12 15:27:04 by jinhyeop          #+#    #+#              #
#    Updated: 2023/07/28 15:32:31 by jinhyeop         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = libft.a
M_SRCS = ft_isalpha.c ft_memcmp.c ft_putnbr_fd.c ft_strjoin.c ft_strnstr.c \
	   ft_isascii.c ft_memcpy.c ft_putstr_fd.c ft_strlcat.c ft_strrchr.c \
	   ft_atoi.c ft_isdigit.c ft_memmove.c ft_split.c ft_strlcpy.c \
	   ft_strtrim.c ft_bzero.c ft_isprint.c ft_memset.c ft_strchr.c \
	   ft_strlen.c ft_substr.c ft_calloc.c ft_itoa.c ft_putchar_fd.c \
	   ft_strdup.c ft_strmapi.c ft_tolower.c ft_isalnum.c ft_memchr.c \
	   ft_putendl_fd.c ft_striteri.c ft_strncmp.c ft_toupper.c \
	   get_next_line_bonus.c get_next_line_utils_bonus.c ft_strtod.c
B_SRCS = $(M_SRCS) ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
		 ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
		 ft_lstmap.c ft_strtoll.c
M_OBJCS = $(M_SRCS:.c=.o)
B_OBJCS = $(B_SRCS:.c=.o)

ifdef BONUS
	OBJCS = $(B_OBJCS)
else
	OBJCS = $(M_OBJCS)
endif

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJCS)
	ar crv $(NAME) $(OBJCS)

bonus:
	$(MAKE) BONUS=1 $(NAME)

clean:
	@rm -f $(B_OBJCS)

fclean: clean
	@rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re bonus
