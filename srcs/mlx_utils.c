/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 12:17:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/07 03:05:56 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */

#include "../includes/minirt.h"

void	leaks(void) //remove
{
	system("leaks --list minirt");
}

int	win_destroy(t_view *view)
{
	mlx_destroy_window(view->mlx, view->win);
	// atexit(leaks); //remove
	exit(0);
	return (0);
}

void	newwin(t_view *view)
{
	if (!view->stop)
		return ;
	mlx_destroy_image(view->mlx, view->img);
	view->img = mlx_new_image(view->mlx, view->can.width, view->can.height);
	view->addr = mlx_get_data_addr(view->img, &view->bits_per_pixel, \
	&view->line_length, &view->endian);
	multi_rend(view);
	mlx_put_image_to_window(view->mlx, view->win, view->img, 0, 0);
}

void	foward_back(int keycode, t_view *view)
{
	if (keycode == S)
	{
		view->can.cam_orig = sub_vector(view->can.cam_orig, view->can.cam_dir);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->clik_status)
			move_obj(keycode, view);
		newwin(view);
	}
	else if (keycode == W)
	{
		view->can.cam_orig = add_vector(view->can.cam_orig, view->can.cam_dir);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->clik_status)
			move_obj(keycode, view);
		newwin(view);
	}	
}

void	move_obj(int keycode, t_view *view)
{
	t_sphere	*sp;
	t_cylinder	*cy;

	if (view->grep.type == SP)
		sp = view->grep.obj;
	else if (view->grep.type == CY)
		cy = view->grep.obj;
	else
		return ;
	if (keycode == A && view->grep.type == SP)
		sp->center = sub_vector(sp->center, view->cam.r_norm);
	else if (keycode == A && view->grep.type == CY)
		cy->center = sub_vector(cy->center, view->cam.r_norm);
	else if (keycode == D && view->grep.type == SP)
		sp->center = add_vector(sp->center, view->cam.r_norm);
	else if (keycode == D && view->grep.type == CY)
		cy->center = add_vector(cy->center, view->cam.r_norm);
	else if (keycode == S && view->grep.type == SP)
		sp->center = sub_vector(sp->center, view->can.cam_dir);
	else if (keycode == S && view->grep.type == CY)
		cy->center = sub_vector(sp->center, view->can.cam_dir);
	else if (keycode == W && view->grep.type == SP)
		sp->center = add_vector(sp->center, view->can.cam_dir);
	else if (keycode == W && view->grep.type == CY)
		cy->center = add_vector(sp->center, view->can.cam_dir);
}

void	left_right(int keycode, t_view *view)
{
	if (keycode == A)
	{
		view->can.cam_orig = sub_vector(view->can.cam_orig, view->cam.r_norm);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->clik_status)
			move_obj(keycode, view);
		newwin(view);
	}
	else if (keycode == D)
	{
		view->can.cam_orig = add_vector(view->can.cam_orig, view->cam.r_norm);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		if (view->grep.grep == ON && view->clik_status)
			move_obj(keycode, view);
		newwin(view);
	}	
}

void	up_down(int keycode, t_view *view)
{
	if (keycode == 24)
	{
		view->can.cam_orig.y += 1;
		if (view->quality_scalar >= -4)
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 27)
	{
		view->can.cam_orig.y -= 1;
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
}

void	rotate_horizontal(int keycode, t_view *view)
{
	if (keycode == 123)
	{
		view->can.cam_dir = sub_vector(view->can.cam_dir, \
			multiple_vector(0.1, view->cam.r_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 124)
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
	if (keycode == 125)
	{
		view->can.cam_dir = add_vector(view->can.cam_dir, \
			multiple_vector(0.09, view->cam.v_norm));
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->quality_scalar = -4;
		view->cam = camera(view->can);
		newwin(view);
	}
	else if (keycode == 126)
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
	if (keycode == Q_UP)
	{
		if (view->quality_scalar >= 6)
			view->quality_scalar = 6;
		view->quality_scalar += 1;
	}
	else if (keycode == Q_DOWN)
		view->quality_scalar -= 1;
	else if (keycode == T)
		view->quality_scalar = 1;
	else if (keycode == Q2)
		view->quality_scalar = -10;
	if (keycode == Q1)
	{
		view->quality_scalar = 6;
		newwin(view);
		pause_system(view);
	}
	else
		newwin(view);
}

t_vec3	rotate_around_axis(t_vec3 vec, t_vec3 axis, float angle)
{
	const t_vec3	k = norm_vec(axis);
	const float		cos_angle = cos(angle);
	const float		sin_angle = sin(angle);
	t_vec3			rotated_vec;

	rotated_vec.x = vec.x * (cos_angle + (1 - cos_angle) * k.x * k.x) + \
					vec.y * ((1 - cos_angle) * k.x * k.y - sin_angle * k.z) + \
					vec.z * ((1 - cos_angle) * k.x * k.z + sin_angle * k.y);
	rotated_vec.y = vec.x * ((1 - cos_angle) * k.y * k.x + sin_angle * k.z) + \
					vec.y * (cos_angle + (1 - cos_angle) * k.y * k.y) + \
					vec.z * ((1 - cos_angle) * k.y * k.z - sin_angle * k.x);
	rotated_vec.z = vec.x * ((1 - cos_angle) * k.z * k.x - sin_angle * k.y) + \
					vec.y * ((1 - cos_angle) * k.z * k.y + sin_angle * k.x) + \
					vec.z * (cos_angle + (1 - cos_angle) * k.z * k.z);
	return (rotated_vec);
}

void	grep_obj(int x, int y, t_view *view)
{
	float	vp_idx[2];
	t_ray3 ray;

	vp_idx[0] = view->can.ratio * 2.0 * ((float)x) / (float)view->can.width;
	vp_idx[1] = 2.0 * ((float)y) / (float)view->can.height;
	ray = create_ray(view->cam, vp_idx[0], vp_idx[1]);
	ray.type = -1;
	ray.pix[0] = x;
	ray.pix[1] = y;
	intersection(&ray, view->can.obj);
	if (ray.type == SP)
	{
		view->grep.obj = ray.obj;
		view->grep.type = SP;
		view->grep.grep = ON;
	}
	else if (ray.type == CY)
	{
		view->grep.obj = ray.obj;
		view->grep.type = CY;
		view->grep.grep = ON;
	}
	else
		view->grep.grep = OFF;
}

void	move_focus(int scalra, t_view *view, float sensitivity)
{
	static int	count;
	int			xy[2];
	float		yaw;
	float		pitch;
	t_vec3		right;

	if (count > scalra)
	{
		mlx_mouse_get_pos(view->win, &xy[0], &xy[1]);
		yaw = (xy[0] - view->can.width / 2) * sensitivity;
		pitch = (xy[1] - view->can.height / 2) * sensitivity;
		right = vector_product(view->can.cam_dir, (t_vec3){0.0f, 1.0f, 0.0f});
		view->can.cam_dir = rotate_around_axis(view->can.cam_dir, \
		(t_vec3){0.0f, 1.0f, 0.0f}, -yaw);
		view->can.cam_dir = \
		rotate_around_axis(view->can.cam_dir, right, -pitch);
		view->can.cam_dir = norm_vec(view->can.cam_dir);
		view->cam = camera(view->can);
		if (view->clik_status && view->grep.grep == ON)
		{
			t_sphere	*sp;
			t_cylinder	*cy;
			if (view->grep.type == SP)
			{
				sp = (t_sphere *)view->grep.obj;
				sp->center = sub_vector(sp->center, view->cam.origin);
				sp->center = rotate_around_axis(sp->center, (t_vec3){0.0f, 1.0f, 0.0f}, -yaw);
				sp->center = rotate_around_axis(sp->center, right, -pitch);
				sp->center = add_vector(sp->center, view->cam.origin);
				
			}
			else if (view->grep.type == CY)
			{
				cy = (t_cylinder *)view->grep.obj;
				cy->center = sub_vector(cy->center, view->cam.origin);
				cy->center = rotate_around_axis(cy->center, (t_vec3){0.0f, 1.0f, 0.0f}, -yaw);
				cy->center = rotate_around_axis(cy->center, right, -pitch);
				cy->center = add_vector(cy->center, view->cam.origin);
			}
		}
		newwin(view);
		mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
		count = 0;
	}
	count++;
}
void	pause_system(t_view *view)
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

void	move_hook(int keycode, t_view *view)
{
	if (keycode == A || keycode == D)
		left_right(keycode, view);
	else if (keycode == W || keycode == S)
		foward_back(keycode, view);
}

void	rotate_hook(int keycode, t_view *view)
{
	if (keycode == 125 || keycode == 126)
		rotate_vertical(keycode, view);
	else if (keycode == 124 || keycode == 123)
		rotate_horizontal(keycode, view);
}

void	init_ppm(FILE *f, unsigned int tmp, unsigned char color[3])
{
	color[RED] = (tmp >> 16) & 0xFF;
	color[GREEN] = (tmp >> 8) & 0xFF;
	color[BLUE] = tmp & 0xFF;
	fwrite(&color[RED], 1, 1, f);
	fwrite(&color[GREEN], 1, 1, f);
	fwrite(&color[BLUE], 1, 1, f);
}

void save_image_to_ppm(char *filename, t_view *view)
{
	FILE			*f;
	int				xy[2];
	char			*dst;
	unsigned int	tmp;
	unsigned char	color[3];

	f = fopen(filename, "w");
	if (!f)
	{
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		return;
	}
	fprintf(f, "P6\n%d %d\n255\n", view->can.width, view->can.height);
	xy[1] = -1;
	while (++xy[1] < view->can.height)
	{
		xy[0] = -1;
		while (++xy[0] < view->can.width)
		{
			dst = view->addr + (xy[1] * view->line_length + xy[0] * (view->bits_per_pixel / 8));
			tmp = *(unsigned int *)dst;
			init_ppm(f, tmp, color);
		}
	}
	fclose(f);
}

void	init_rt_color(int color[3], FILE *f)
{
	fprintf(f, " %d,%d,%d\n", color[RED], color[GREEN], color[BLUE]);
}
void	init_rt_vec(t_vec3 vec, FILE *f)
{
	fprintf(f, " %f,%f,%f ", vec.x, vec.y, vec.z);
}

void	init_rt_camera(t_view *view, FILE *f)
{
	fprintf(f, "c	");
	init_rt_vec(view->can.cam.origin, f);
	init_rt_vec(view->can.cam.dir, f);
	fprintf(f, " %f \n", view->can.cam.fov);
}

void	init_rt_light(t_view *view, FILE *f)
{
	int		i;
	t_light	tmp;

	i = 0;
	while (view->can.obj->l_cnt)
	{
		tmp = view->can.obj->l[i];
		fprintf(f, "l	");
		init_rt_vec(tmp.light_orig, f);
		fprintf(f, " %f	", tmp.light_bright);
		init_rt_color(tmp.light_col, f);
		i++;
	}

}

void	save_image_to_rtfile(char *filename, t_view *view)
{
	FILE	*f;

	f = fopen(filename, "w");
	if (!f)
	{
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		return;
	}
	fprintf(f, "R	%d %d", view->can.width, view->can.height);
	fprintf(f, "A	%f ", view->can.amb_bright);
	init_rt_color(view->can.amb_col, f);
	init_rt_camera(view, f);
	init_rt_light(view, f);

}

int	key_hook(int keycode, t_view *view)
{
	if (keycode == 125 || keycode == 126 || keycode == 124 || keycode == 123)
		rotate_hook(keycode, view);
	else if (keycode == A || keycode == S || keycode == D || keycode == W)
		move_hook(keycode, view);
	else if (keycode == 24 || keycode == 27)
		up_down(keycode, view);
	else if (keycode == Q_UP || keycode == Q_DOWN || keycode == T || keycode == Q1 || \
			keycode == Q2)
		quality(keycode, view);
	else if (keycode == H)
		view->flag = !view->flag;
	else if (keycode == 35)
		pause_system(view);
	else if (keycode == 53)
		win_destroy(view);
	if (view->stop && (keycode == W || keycode == S \
		|| keycode == A || keycode == D))
	{
		view->focus = 1;
		move_focus(0, view, 0.007);
	}
	else if (keycode == M)
	{
		view->show_mouse = !view->show_mouse;
		if (view->show_mouse)
			mlx_mouse_hide();
		else
			mlx_mouse_show();
	}
	else if (keycode == PRINT)
		save_image_to_ppm("outfile.ppm", view);
	else if (keycode == MAKE)
		save_image_to_rtfile("outfile.rt", view);
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
