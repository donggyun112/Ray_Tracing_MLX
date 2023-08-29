/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/29 22:41:35 by dongkseo         ###   ########.fr       */
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
	make_image(view, (view)->can);
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
		view->can.cam_orig = sub_vector(view->can.cam_orig, view->can.cam_dir);
		view->quality_scalar = -2;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 126)
	{
		view->can.cam_orig = add_vector(view->can.cam_orig, view->can.cam_dir);
		view->quality_scalar = -2;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 123)
	{
		view->can.cam_orig = sub_vector(view->can.cam_orig, view->cam.r_norm);
		view->quality_scalar = -2;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 124)
	{
		view->can.cam_orig = add_vector(view->can.cam_orig, view->cam.r_norm);
		view->quality_scalar = -2;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 24)
	{
		view->can.cam_orig.y -= 2;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 27)
	{
		view->can.cam_orig.y += 2;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 13)
	{
		view->can.cam_dir.y += 0.05;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 1)
	{
		view->can.cam_dir.y -= 0.05;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 12)
	{
		view->can.cam_dir.z -= 0.05;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 14)
	{
		view->can.cam_dir.z += 0.05;
		view->cam = camera(view->can);
		newwin(view);
	}
	if (keycode == 33)
	{
		if (view->quality_scalar >= 6)
			view->quality_scalar = 6;
		view->quality_scalar += 1;
		newwin(view);
	}
	if (keycode == 30)
	{
		view->quality_scalar -= 1;
		newwin(view);
	}
	if (keycode == 17)
	{
		view->quality_scalar = -2;
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

int	rgb_to_int(int color[])
{
	return ((color[RED] << 16) | (color[GREEN] << 8) \
	| color[BLUE]);
}

