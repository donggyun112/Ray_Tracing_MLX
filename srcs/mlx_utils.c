/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/04 04:37:13 by jinhyeop         ###   ########.fr       */
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

int	key_hook(int keycode, t_view *view)
{
	if (keycode == 53)
	{
		mlx_destroy_window(view->mlx, view->win);
		exit (0);
	}
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
