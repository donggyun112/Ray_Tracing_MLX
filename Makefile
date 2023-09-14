# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/02 15:17:44 by jinhyeop          #+#    #+#              #
#    Updated: 2023/09/14 10:06:47 by jinhyeop         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -O3
CLIB = -Llibft -lft -Lmlx -lmlx -framework OpenGL -framework Appkit -Imlx -lm

NAME = minirt

MAN_DIR = ./mandatory/srcs
MAN_FILE = minirt.c mlx_utils.c plane.c camera.c parse.c \
		vector.c simple_vec_cal.c color.c shadow_utils.c shadow.c \
		angle_ref.c angle_cos.c calculate_utils.c parse_obj.c parse_utils.c \
		sphere.c cylinder.c error_check.c
BONUS_DIR = ./srcs
BONUS_FILE = minirt.c mlx_utils.c parse.c \
		bumpmap.c color.c cylinder_texture1.c cylinder_texture2.c cylinder.c \
		diffuse.c plane_texture.c plane.c raycasting.c reflection.c shadow.c \
		shadow_obj.c simple_vec_cal.c vector.c sphere.c sphere_texture.c \
		texture.c utils.c error_handling.c init_cylinder.c init_plane.c \
		init_sphere.c init_view.c parse_utils.c mlx_key_hook.c grep_obj.c \
		write_rtfile.c write_rtfile_pl.c write_rtfile_view.c write_rtfile_obj.c \
		move_obj.c move_grep_obj.c move_cam.c mlx_string.c backup_obj.c \
		copy_cylinder_plane.c copy_obj_interface.c copy_sphere.c save_ppmfile.c \
		quality.c backup_obj.c init_data.c move_obj_interface.c render.c \
		multi_thread.c mlx_mouse.c mlx_loop_hook.c define_color.c 
MAN_SRCS = $(addprefix $(MAN_DIR)/, $(MAN_FILE))
BONUS_SRCS = $(addprefix $(BONUS_DIR)/, $(BONUS_FILE))
MAN_OBJS = $(MAN_SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

ifdef BONUS
	OBJS = $(BONUS_OBJS)
else
	OBJS = $(MAN_OBJS)
endif

MLX_NAME = mlx
MLX_DIR = ./mlx
MLX = $(addprefix $(MLX_DIR)/, libmlx.a)

LIBFT_NAME = libft
LIBFT_DIR = ./libft
LIBFT = $(addprefix $(LIBFT_DIR)/, libft.a)

all: $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) all
	$(MAKE) -C $(MLX_DIR) all
	$(CC) $(CFLAGS) $(CLIB) $^ -o $@

bonus:
	$(MAKE) BONUS=1 $(NAME)

clean:
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(MAKE) -C $(MLX_DIR) clean
	@rm -f $(MAN_OBJS)
	@rm -f $(BONUS_OBJS)
	@echo "All object files removed"

fclean: clean
	@rm -f $(NAME)
	@echo "Executable file removed"

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all bonus clean fclean re
