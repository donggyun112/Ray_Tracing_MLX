/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/01 15:28:02 by dongkseo         ###   ########.fr       */
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
	// make_image(view, (view)->can);
	multi_rend(view);
	mlx_put_image_to_window(view->mlx, view->win, view->img, 0, 0);
}

void	foward_back(int keycode, t_view *view)
{
	if (keycode == 125)
	{
		view->can.cam_orig = sub_vector(view->can.cam_orig, view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 126)
	{
		view->can.cam_orig = add_vector(view->can.cam_orig, view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}	
}

void	left_right(int keycode, t_view *view)
{
	if (keycode == 123)
	{
		view->can.cam_orig = sub_vector(view->can.cam_orig, view->cam.r_norm);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 124)
	{
		view->can.cam_orig = add_vector(view->can.cam_orig, view->cam.r_norm);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}	
}

void	up_down(int keycode, t_view *view)
{
	if (keycode == 24)
	{
		view->can.cam_orig.y += 1;
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 27)
	{
		view->can.cam_orig.y -= 1;
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
}

void	rotate_horizontal(int keycode, t_view *view)
{
	if (keycode == 0)
	{
		view->can.cam_dir = sub_vector(view->can.cam_dir, \
			multiple_vector(0.1, view->cam.r_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 2)
	{
		view->can.cam_dir = add_vector(view->can.cam_dir, \
			multiple_vector(0.1, view->cam.r_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
}

void	rotate_vertical(int keycode, t_view *view)
{
	if (keycode == 1)
	{
		view->can.cam_dir = add_vector(view->can.cam_dir, \
			multiple_vector(0.09, view->cam.v_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 13)
	{
		view->can.cam_dir = sub_vector(view->can.cam_dir, \
			multiple_vector(0.09, view->cam.v_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
}

void	quality(int keycode, t_view *view)
{
	if (keycode == 33)
	{
		if (view->quality_scalar >= 6)
			view->quality_scalar = 6;
		view->quality_scalar += 1;
		newwin(view);
	}
	else if (keycode == 30)
	{
		view->quality_scalar -= 1;
		newwin(view);
	}
	else if (keycode == 17)
	{
		view->quality_scalar = 1;
		newwin(view);
	}	
}

int	key_hook(int keycode, t_view *view)
{
	if (keycode == 53)
	{
		mlx_destroy_window(view->mlx, view->win);
		exit (0);
	}
	else if (keycode == 125 || keycode == 126)
		foward_back(keycode, view);
	else if (keycode == 123 || keycode == 124)
		left_right(keycode, view);
	else if (keycode == 24 || keycode == 27)
		up_down(keycode, view);
	else if (keycode == 13 || keycode == 1)
		rotate_vertical(keycode, view);
	else if (keycode == 0 || keycode == 2)
		rotate_horizontal(keycode, view);
	else if (keycode == 33 || keycode == 30 || keycode == 17)
		quality(keycode, view);
	else if (keycode == 4)
	{
		view->flag = !view->flag;
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

