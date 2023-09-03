/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/03 15:39:20 by seodong-gyu      ###   ########.fr       */
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
	if (keycode == 1)
	{
		view->can.cam_orig = sub_vector(view->can.cam_orig, view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 13)
	{
		view->can.cam_orig = add_vector(view->can.cam_orig, view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}	
}

void	left_right(int keycode, t_view *view)
{
	if (keycode == 0)
	{
		view->can.cam_orig = sub_vector(view->can.cam_orig, view->cam.r_norm);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 2)
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
	if (keycode == 124)
	{
		view->can.cam_dir = sub_vector(view->can.cam_dir, \
			multiple_vector(0.1, view->cam.r_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 123)
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
	if (keycode == 126)
	{
		view->can.cam_dir = add_vector(view->can.cam_dir, \
			multiple_vector(0.09, view->cam.v_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 125)
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

t_vec3 rotate_around_axis(t_vec3 vec, t_vec3 axis, float angle)
{
	const t_vec3 k = norm_vec(axis);
	const float cos_angle = cos(angle);
	const float sin_angle = sin(angle);

	t_vec3 rotated_vec;

	rotated_vec.x = vec.x * (cos_angle + (1 - cos_angle) * k.x * k.x) + 
					vec.y * ((1 - cos_angle) * k.x * k.y - sin_angle * k.z) +
					vec.z * ((1 - cos_angle) * k.x * k.z + sin_angle * k.y);

	rotated_vec.y = vec.x * ((1 - cos_angle) * k.y * k.x + sin_angle * k.z) + 
					vec.y * (cos_angle + (1 - cos_angle) * k.y * k.y) +
					vec.z * ((1 - cos_angle) * k.y * k.z - sin_angle * k.x);

	rotated_vec.z = vec.x * ((1 - cos_angle) * k.z * k.x - sin_angle * k.y) + 
					vec.y * ((1 - cos_angle) * k.z * k.y + sin_angle * k.x) +
					vec.z * (cos_angle + (1 - cos_angle) * k.z * k.z);

	return rotated_vec;
}

void move_focus(int scalra, t_view *view, float sensitivity)
{
	static int count;
	int x, y;
	float yaw, pitch;
	t_vec3 right;

	if (count > scalra)
	{
		mlx_mouse_get_pos(view->win, &x, &y);
		yaw = (x - view->can.width / 2) * sensitivity;
		pitch = (y - view->can.height / 2) * sensitivity;
		right = vector_product(view->can.cam_dir, (t_vec3){0.0f, 1.0f, 0.0f});
		view->can.cam_dir = rotate_around_axis(view->can.cam_dir, (t_vec3){0.0f, 1.0f, 0.0f}, -yaw);
		view->can.cam_dir = rotate_around_axis(view->can.cam_dir, right, -pitch);
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->cam = camera(view->can);
		newwin(view);
		mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
		count = 0;
	}
	count++;
}

int	key_hook(int keycode, t_view *view)
{
	if (keycode == 53)
	{
		mlx_destroy_window(view->mlx, view->win);
		exit (0);
	}
	else if (keycode == 125 || keycode == 126)
		rotate_vertical(keycode, view);
	else if (keycode == 0 || keycode == 2)
		left_right(keycode, view);
	else if (keycode == 24 || keycode == 27)
		up_down(keycode, view);
	else if (keycode == 13 || keycode == 1)
		foward_back(keycode, view);
	else if (keycode == 124 || keycode == 123)
		rotate_horizontal(keycode, view);
	else if (keycode == 33 || keycode == 30 || keycode == 17)
		quality(keycode, view);
	else if (keycode == 4)
		view->flag = !view->flag;
	else if (keycode == 35)
	{
		view->stop = !view->stop;
		if (view->stop)
		{
			mlx_mouse_hide();
			mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
		}
		else
			mlx_mouse_show();
	}
	if ((keycode == 13 || keycode == 1 || keycode == 0 || keycode == 2) && view->stop)
	{
		view->focus = 1;
		move_focus(0, view, 0.007);
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
	return ((color[RED] << 16) | (color[GREEN] << 8) \
	| color[BLUE]);
}

