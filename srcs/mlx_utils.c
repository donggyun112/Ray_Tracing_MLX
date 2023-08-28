/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/25 01:30:40 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */

#include "../includes/minirt.h"

int	win_destroy(t_view *view)
{
	mlx_destroy_window(view->mlx, view->win);
	exit(0);
	return (0);
}

void	newwin(t_view *view)
{
	view->img = mlx_new_image(view->mlx, view->can.width, view->can.height);
	view->addr = mlx_get_data_addr(view->img, &view->bits_per_pixel, \
	&view->line_length, &view->endian);
	make_image(view, (view)->can, (view)->cam);
	mlx_put_image_to_window(view->mlx, view->win, view->img, 0, 0);
}

int	key_hook(int keycode, t_view *view)
{
	if (keycode == 53)
	{
		mlx_destroy_window(view->mlx, view->win);
		exit (0);
	}
	else if (keycode == 125)
	{
		view->can.cam_orig.x += 1;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 126)
	{
		view->can.cam_orig.x -= 1;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 24)
	{
		view->can.cam_orig.y -= 4;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 27)
	{
		view->can.cam_orig.y += 4;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 37)
	{
		view->can.cam_dir.y += 0.05;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 40)
	{
		view->can.cam_dir.y -= 0.05;
		view->cam = camera(view->can);
		newwin(view);
	}
	printf("%d\n", keycode);
	return (0);
}

void	my_mlx_pixel_put(t_view *mlx, int x, int y, unsigned int color)
{
	char	*dst;

	dst = mlx->addr + (y * mlx->line_length + x * (mlx->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int rgb_to_int(int color[])
{
	return ((color[RED] << 16) | (color[GREEN] << 8) \
	| color[BLUE]);
}

int my_rand()
{
	static unsigned long seed = 123456789;

	seed = (1103515245*seed + 12345) % 2147483648;
	return (seed);
}

double my_rand_double()
{
	return ((double)my_rand() / 2147483648);
}

double my_rand_double_range(double min, double max)
{
	return min + (max - min) * my_rand_double();
}
