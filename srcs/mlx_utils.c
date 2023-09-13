/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/14 01:31:23 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
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
	if (!view->stop)
		return ;
	mlx_clear_window(view->mlx, view->win);
	multi_rend(view);
	mlx_put_image_to_window(view->mlx, view->win, view->img, 0, 0);
	string_put(view);
}

void	make_cylinder_cap2(t_cylinder *cy)
{
	free(cy->ucap);
	free(cy->lcap);
	make_cylinder_cap(cy);
}

void	my_mlx_pixel_put(t_view *mlx, int x, int y, unsigned int color)
{
	char	*dst;

	dst = mlx->addr + (y * mlx->line_length + x * (mlx->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	rgb_to_int(int color[])
{
	return ((color[RED] << 16) | (color[GREEN] << 8) \
	| color[BLUE]);
}
