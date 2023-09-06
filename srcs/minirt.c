/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:48:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/07 01:39:34 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	intersection(t_ray3 *ray, t_volume *obj)
{
	int	idx;

	idx = 0;
	while (idx < obj->sp_cnt)
		hit_sphere(ray, &obj->sp[idx++]);
	idx = 0;
	while (idx < obj->pl_cnt)
		hit_plane(ray, &obj->pl[idx++]);
	idx = 0;
	while (idx < obj->cy_cnt)
		hit_cylinder(ray, &obj->cy[idx++]);
}

void	color_cal(t_canvas canvas, t_ray3 *ray, t_color *color)
{
	int	light;
	int	idx;

	light = 0;
	idx = 0;
	if (ray->t > 0.0)
	{
		while (light < canvas.obj->l_cnt)
		{
			ray_color(canvas, ray, light);
			light++;
		}
		while (idx < 3)
		{
			ray->real[idx] += amb_light(canvas, ray, idx);
			if (ray->real[idx] > 255)
				ray->real[idx] = 255;
			idx++;
		}
	}
	color->r += ray->real[RED];
	color->g += ray->real[GREEN];
	color->b += ray->real[BLUE];
}

t_color	anti_aliasing(int pix[2], float vp_idx[2], t_view *view, t_ray3 *ray)
{
	t_color	color;
	float	offset[2];
	int		idx[2];

	color = (t_color){0, 0, 0};
	idx[0] = -1;
	while (++idx[0] < view->anti_scalar)
	{
		idx[1] = -1;
		while (++idx[1] < view->anti_scalar)
		{
			offset[0] = (float)idx[1] / view->anti_scalar;
			offset[1] = (float)idx[0] / view->anti_scalar;
			vp_idx[0] = view->can.ratio * 2.0 * ((float)pix[0] + \
			offset[0]) / (float)view->can.width;
			vp_idx[1] = 2.0 * ((float)pix[1] + \
			offset[1]) / (float)view->can.height;
			*ray = create_ray(view->cam, vp_idx[0], vp_idx[1]);
			ray->pix[0] = pix[0];
			ray->pix[1] = pix[1];
			intersection(ray, view->can.obj);
			color_cal(view->can, ray, &color);
		}
	}
	return (color);
}

void	init_backgorund(t_view *view, int pix[2], t_ray3 *ray, int xy[2])
{
	float		po[2];
	t_color		c;

	po[0] = ((float)pix[0] / view->can.width);
	po[1] = ((float)pix[1] / view->can.height);
	c = get_texture_color(view->back, po[0], po[1]);
	ray->real[RED] = c.r;
	ray->real[GREEN] = c.g;
	ray->real[BLUE] = c.b;
	my_mlx_pixel_put(view, xy[0], xy[1], rgb_to_int(ray->real));
}

void	low_quality(int scalar, int pix[2], t_ray3 ray, t_view *view)
{
	int			offset[2];
	int			xy[2];

	offset[0] = 0;
	while (offset[0] < scalar)
	{
		offset[1] = 0;
		while (offset[1] < scalar)
		{
			xy[0] = pix[0] + offset[0];
			xy[1] = pix[1] + offset[1];
			if (xy[0] < view->can.width && xy[1] < view->can.height)
			{
				if (ray.t > 0.0)
					my_mlx_pixel_put(view, xy[0], xy[1], rgb_to_int(ray.real));
				else if (view->can.bgt_filepath)
					init_backgorund(view, pix, &ray, xy);
				else
					my_mlx_pixel_put(view, xy[0], xy[1], 0xFFFFFF);
			}
			offset[1]++;
		}
		offset[0]++;
	}
}

void	set_thread_st_point(int *anti, int pix[2], t_thread *t)
{
	*anti = t->view->anti_scalar * t->view->anti_scalar;
	pix[1] = t->id * (t->canvas.height / NUM_OF_THREAD);
	if (t->id == NUM_OF_THREAD - 1)
		pix[2] = t->canvas.height;
	else
		pix[2] = (t->id + 1) * (t->canvas.height / NUM_OF_THREAD);
}

void	*make_image2(void *m)
{
	int			pix[3];
	float		vp_idx[2];
	t_color		c;
	t_thread	*t;
	int			anti;

	t = (t_thread *)m;
	set_thread_st_point(&anti, pix, t);
	while (pix[1] < pix[2] && pix[1] < t->canvas.height)
	{
		pix[0] = 0;
		while (pix[0] < t->canvas.width)
		{
			c = anti_aliasing(pix, vp_idx, t->view, &t->ray);
			t->ray.real[RED] = c.r / anti;
			t->ray.real[GREEN] = c.g / anti;
			t->ray.real[BLUE] = c.b / anti;
			low_quality(t->view->low_scalar, pix, t->ray, t->view);
			pix[0] += t->view->low_scalar;
		}
		pix[1] += (t->view->low_scalar);
	}
	return (NULL);
}

void	set_quality_scalar(t_view *view)
{
	if (view->quality_scalar > 0)
	{
		view->anti_scalar = view->quality_scalar;
		view->low_scalar = 1;
	}
	else if (view->quality_scalar < 0)
	{
		view->low_scalar = -view->quality_scalar;
		view->anti_scalar = 1;
	}
	else
	{
		view->low_scalar = 1;
		view->anti_scalar = 1;
	}
}

void	make_image(t_view *view, t_canvas canvas)
{
	int		pix[2];
	float	vp_idx[2];
	t_ray3	ray;
	t_color	c;

	set_quality_scalar(view);
	pix[1] = 0;
	while (pix[1] < canvas.height)
	{
		pix[0] = 0;
		while (pix[0] < canvas.width)
		{
			c = anti_aliasing(pix, vp_idx, view, &ray);
			ray.real[RED] = c.r / (view->anti_scalar * view->anti_scalar);
			ray.real[GREEN] = c.g / (view->anti_scalar * view->anti_scalar);
			ray.real[BLUE] = c.b / (view->anti_scalar * view->anti_scalar);
			low_quality(view->low_scalar, pix, ray, view);
			pix[0] += view->low_scalar;
		}
		pix[1] += view->low_scalar;
	}
}

t_thread	*init_thread(t_view *view)
{
	t_thread	*m;
	int			x;

	m = (t_thread *)malloc(sizeof(t_thread) * NUM_OF_THREAD);
	set_quality_scalar(view);
	x = 0;
	while (x < NUM_OF_THREAD)
	{
		m[x].id = x;
		m[x].view = view;
		m[x].canvas = view->can;
		x++;
	}
	return (m);
}

void	multi_rend(t_view *view)
{
	t_thread	*m;
	int			x;

	x = 0;
	m = init_thread(view);
	while (x < NUM_OF_THREAD)
	{
		pthread_create(&m[x].thread, NULL, make_image2, &m[x]);
		x++;
	}
	x = 0;
	while (x < NUM_OF_THREAD)
	{
		pthread_join(m[x].thread, NULL);
		x++;
	}
	free(m);
}

void	set_texture(t_view *view, t_volume *obj)
{
	int	i;

	i = -1;
	while (++i < obj->sp_cnt)
	{
		if (obj->sp[i].type == TSP)
		{
			init_texture(&obj->sp[i].texture, view, obj->sp[i].filepath);
			init_texture(&obj->sp[i].bumtexture, view, obj->sp[i].bumppath);
		}
	}
	i = -1;
	while (++i < obj->pl_cnt)
		if (obj->pl[i].type == TPL)
			init_texture(&obj->pl[i].texture, view, obj->pl[i].filepath);
	i = -1;
	while (++i < obj->cy_cnt)
	{
		if (obj->cy[i].type == TCY)
		{
			init_texture(&obj->cy[i].texture, view, obj->cy[i].filepath);
			init_texture(&obj->cy[i].bumtexture, view, obj->cy[i].bumppath);
		}
	}
}

void	init_view_scale(t_view *view)
{
	view->anti_scalar = 1;
	view->low_scalar = 1;
	view->quality_scalar = -2;
	view->flag = 0;
	view->focus = 0;
	view->stop = 1;
	view->show_mouse = 1;
	view->clik_status = 0;
	if (view->can.bgt_filepath)
		init_texture(&view->back, view, view->can.bgt_filepath);
	mlx_mouse_hide();
	mlx_mouse_move(view->win, view->can.width / 2, view->can.height / 2);
}

t_vec3	rotate_around_specific_point(t_vec3 vec, t_vec3 center, float angle)
{
	t_vec3		translated_vec;
	t_vec3		rotated_vec;
	const float	cos_angle = cos(angle);
	const float	sin_angle = sin(angle);

	translated_vec.x = vec.x - center.x;
	translated_vec.y = vec.y - center.y;
	translated_vec.z = vec.z - center.z;
	rotated_vec.x = translated_vec.x * cos_angle - translated_vec.z * sin_angle;
	rotated_vec.y = translated_vec.y;
	rotated_vec.z = translated_vec.x * sin_angle + translated_vec.z * cos_angle;
	rotated_vec.x += center.x;
	rotated_vec.y += center.y;
	rotated_vec.z += center.z;
	return (rotated_vec);
}

void	change_angle(t_view *view)
{
	int	x;

	x = 0;
	while (x < view->can.obj->sp_cnt || x < view->can.obj->cy_cnt)
	{
		if (x < view->can.obj->sp_cnt)
		{
			if (view->can.obj->sp[x].type == TSP)
				view->can.obj->sp[x].angle += 0.05;
			else if (view->can.obj->sp[x].type == CSP)
				view->can.obj->sp[x].angle += 0.2;
			else
			view->can.obj->sp[x].angle = \
			fmod(view->can.obj->sp[x].angle, 2.0 * M_PI);
		}
		if (x < view->can.obj->cy_cnt)
		{
			if (view->can.obj->cy[x].type == TCY \
			|| view->can.obj->cy[x].type == CCY)
				view->can.obj->cy[x].angle += 0.2;
			view->can.obj->cy[x].angle = \
			fmod(view->can.obj->cy[x].angle, 2.0 * M_PI);
		}
		x++;
	}
}

int	loop_hook(t_view *view)
{
	if (view->flag && view->stop)
	{
		change_angle(view);
		if (view->can.obj->rsp_cnt > 0)
		{
			view->can.obj->rsp[0].sp->center = sub_vector(view->can.obj->\
			rsp[0].sp->center, view->can.obj->rsp[0].r_center);
			view->can.obj->rsp[0].sp->center = \
			rotate_around_axis(view->can.obj->rsp[0] \
			.sp->center, view->can.obj->rsp[0].r_axis, 0.05);
			view->can.obj->rsp[0].sp->center = \
			add_vector(view->can.obj->\
			rsp[0].sp->center, view->can.obj->rsp[0].r_center);
		}
		newwin(view);
		move_focus(0, view, 0.007);
		view->focus = 1;
	}
	else
		view->focus = 0;
	return (0);
}

void	make_cylinder_cap(t_cylinder *cy)
{
	int	idx;

	idx = 0;
	cy->ucap = (t_plane *)malloc(sizeof(t_plane));
	cy->lcap = (t_plane *)malloc(sizeof(t_plane));
	while (idx < 3)
	{
		cy->ucap->color[idx] = cy->color[idx];
		cy->lcap->color[idx] = cy->color[idx];
		idx++;
	}
	cy->ucap->norm = cy->dir;
	cy->lcap->norm = multiple_vector(-1.0, cy->dir);
	cy->ucap->on_plane = add_vector(cy->center, \
		multiple_vector(cy->height / 2.0, cy->dir));
	cy->lcap->on_plane = add_vector(cy->center, \
		multiple_vector(cy->height / -2.0, cy->dir));
}

void	make_obj_cap(t_volume *obj)
{
	int	idx;

	idx = 0;
	while (idx < obj->cy_cnt)
		make_cylinder_cap(&obj->cy[idx++]);
}

int	mouse_motion(int x, int y, t_view *view)
{
	static int	pos[2];

	mlx_mouse_get_pos(view->win, &x, &y);
	if ((pos[0] != x || pos[1] != y) && view->stop)
	{
		if (view->quality_scalar >= -4)
			view->quality_scalar = -4;
		if (view->focus == 0)
			move_focus(10, view, 0.007);
		else
			move_focus(30, view, 0.007);
	}
	pos[0] = x;
	pos[1] = y;
	return (0);
}

int	key_release(int keycode, t_view *view)
{
	if (keycode == 13 || keycode == 1 || keycode == 0 || keycode == 2)
		view->focus = 0;
	return (0);
}

int	is_valid_file_type(char *file_path)
{
	char	**path;
	int		answer;
	int		i;

	answer = 0;
	i = 0;
	path = ft_split(file_path, ".");
	if (!path)
		return (0);
	while (path[i] != 0)
		i++;
	if (ft_strncmp(path[i - 1], "rt", 2) == 0)
		answer = 1;
	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
	return (answer);
}
void	grep_obj(int x, int y, t_view *view);
int	mouse_press(int button, int x, int y, t_view *view)
{
	if (button == 2 || button == 1)
	{
		mlx_mouse_get_pos(view->win, &x, &y);
		grep_obj(x, y, view);
		view->clik_status = 1;
	}
	return (0);
}

int	mouse_release(int button, int x, int y, t_view *view)
{
	(void)x;
	(void)y;
	if (button == 2 || button == 1)
		view->clik_status = 0;
	return (0);
}

void	mlx_engine(t_view *view)
{
	mlx_hook(view->win, 2, 1L << 0, key_hook, view);
	mlx_hook(view->win, 3, 1L << 1, key_release, view);
	mlx_hook(view->win, 17, 1L << 5, win_destroy, view);
	mlx_hook(view->win, 4, 1L << 2, mouse_press, view);
	mlx_hook(view->win, 5, 1L << 3, mouse_release, view);
	mlx_hook(view->win, 6, 1L << 7, mouse_motion, view);
	mlx_loop_hook(view->mlx, loop_hook, view);
}

int	main(int argc, char *argv[])
{
	t_view		view;
	t_canvas	canvas;

	if (argc != 2 || !is_valid_file_type(argv[1]))
	{
		printf("Error\nInput mapfile(*.rt) as argument\n");
		return (1);
	}
	canvas = parse(argv);
	make_obj_cap(canvas.obj);
	view.cam = camera(canvas);
	view.can = canvas;
	view.mlx = mlx_init();
	view.win = mlx_new_window(view.mlx, canvas.width, canvas.height, "miniRT");
	view.img = mlx_new_image(view.mlx, canvas.width, canvas.height);
	view.addr = mlx_get_data_addr(view.img, &view.bits_per_pixel, \
		&view.line_length, &view.endian);
	init_view_scale(&view);
	set_texture(&view, canvas.obj);
	multi_rend(&view);
	mlx_put_image_to_window(view.mlx, view.win, view.img, 0, 0);
	mlx_engine(&view);
	mlx_loop(view.mlx);
	exit(0);
}
