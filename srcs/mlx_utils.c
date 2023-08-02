/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/01 23:47:48 by jinhyeop         ###   ########.fr       */
/*   Updated: 2023/08/01 22:04:05 by dongkseo         ###   ########.fr       */

/*                                                                            */
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

int	rgb_to_int(int color[])
{
	return ((color[RED] & 0xFF << 16) | (color[GREEN] & 0xFF << 8) \
		| (color[BLUE] & 0xFF));
}
	return ((color[RED] & 0xFF << 16) | (color[GREEN] & 0xFF << 8) | (color[BLUE] & 0xFF));
}
