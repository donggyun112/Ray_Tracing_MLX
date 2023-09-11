/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:48:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/11 17:24:44 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	intersection(t_ray3 *ray, t_volume *obj)
{
	int	idx;

	idx = 0;
	while (idx < obj->sp_cnt)
	{
		if (obj->sp[idx].type != NONE)
			hit_sphere(ray, &obj->sp[idx]);
		idx++;
	}
	idx = 0;
	while (idx < obj->pl_cnt)
	{
		if (obj->pl[idx].type != NONE)
			hit_plane(ray, &obj->pl[idx]);
		idx++;
	}
	idx = 0;
	while (idx < obj->cy_cnt)
	{
		if (obj->cy[idx].type != NONE)
			hit_cylinder(ray, &obj->cy[idx]);
		idx++;
	}
}

void	init_background(t_view *view, int pix[2], t_ray3 *ray)
{
	float		po[2];
	t_color		c;

	po[0] = ((float)pix[0] / view->can.width);
	po[1] = ((float)pix[1] / view->can.height);
	c = get_texture_color(view->back, po[0], po[1]);
	ray->real[RED] = c.r;
	ray->real[GREEN] = c.g;
	ray->real[BLUE] = c.b;
}

void	hit_color(t_canvas canvas, t_ray3 *ray)
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
}

void	no_hit(t_view *view, t_ray3 *ray, int pix[2])
{
	if (view->can.bgt_filepath)
		init_background(view, pix, ray);
	else
	{
	ray->real[RED] = 255;
	ray->real[GREEN] = 255;
	ray->real[BLUE] = 255;
	}
}

void	color_cal(t_view *view, t_ray3 *ray, t_color *color, int pix[2])
{
	if (ray->t > 0.0)
		hit_color(view->can, ray);
	else
		no_hit(view, ray, pix);
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
			color_cal(view, ray, &color, pix);
		}
	}
	return (color);
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
				my_mlx_pixel_put(view, xy[0], xy[1], rgb_to_int(ray.real));
			offset[1]++;
		}
		offset[0]++;
	}
}

void	set_thread_st_point(int *anti, int pix[2], t_thread *t)
{
	const int	gap = t->canvas.height / NUM_OF_THREAD;
	int			step;
	int			remain;

	step = gap / t->view->low_scalar;
	remain = gap % step;
	step += !!remain;
	*anti = t->view->anti_scalar * t->view->anti_scalar;
	pix[1] = t->id * (step * t->view->low_scalar);
	if (t->id == NUM_OF_THREAD - 1)
		pix[2] = t->canvas.height;
	else
		pix[2] = (t->id + 1) * (step * t->view->low_scalar);
}

void	make_image2(void *m)
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
		pthread_create(&m[x].thread, NULL, (void *)make_image2, &m[x]);
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
	view->click_status = 0;
	view->backup = NULL;
	view->change_dir = 0;
	view->grep.type = -1;
	view->mini_size = view->can.width - 200;
	view->real_size = view->can.width;
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

void	rotate_rsp(int x, t_view *view)
{
	view->can.obj->rsp[x].sp->center = sub_vector(view->can.obj->\
	rsp[x].sp->center, view->can.obj->rsp[x].r_center);
	view->can.obj->rsp[x].sp->center = \
	rotate_around_axis(view->can.obj->rsp[x] \
	.sp->center, view->can.obj->rsp[x].r_axis, 0.05);
	view->can.obj->rsp[x].sp->center = \
	add_vector(view->can.obj->\
	rsp[x].sp->center, view->can.obj->rsp[x].r_center);
}

void	rotate_rl(int x, t_view *view)
{
	view->can.obj->rl[x].light->light_orig = sub_vector(view->can.obj->\
	rl[x].light->light_orig, view->can.obj->rl[x].r_center);
	view->can.obj->rl[x].light->light_orig = \
	rotate_around_axis(view->can.obj->rl[x] \
	.light->light_orig, view->can.obj->rl[x].r_axis, 0.05);
	view->can.obj->rl[x].light->light_orig = \
	add_vector(view->can.obj->\
	rl[x].light->light_orig, view->can.obj->rl[x].r_center);
}

int	loop_hook(t_view *view)
{
	int	i;

	i = -1;
	if (view->flag && view->stop)
	{
		change_angle(view);
		if (view->can.obj->rsp_cnt > 0 || view->can.obj->rl_cnt)
		{
			while (++i < view->can.obj->rsp_cnt)
				rotate_rsp(i, view);
			i = -1;
			while (++i < view->can.obj->rl_cnt)
				rotate_rl(i, view);
		}
		newwin(view);
		move_focus(1, view, 0.005);
	}
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
		if (!view->focus)
			move_focus(2, view, 0.005);
		else
			move_focus(5, view, 0.005);
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
	if (ft_strcmp(path[i - 1], "rt") == 0)
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

void	zomm_in(t_view *view)
{
	t_sphere	*sp;
	t_cylinder	*cy;

	if (view->grep.type == SP)
	{
		sp = (t_sphere *)view->grep.obj;
		sp->radius += 0.2;
	}
	else if (view->grep.type == CY)
	{
		cy = (t_cylinder *)view->grep.obj;
		cy->height -= 0.5;
		cy->radius -= 0.2;
		make_cylinder_cap2(cy);
	}
}

void	zoom_out(t_view *view)
{
	t_sphere	*sp;
	t_cylinder	*cy;

	if (view->grep.type == SP)
	{
		sp = (t_sphere *)view->grep.obj;
		sp->radius -= 0.2;
	}
	else if (view->grep.type == CY)
	{
		cy = (t_cylinder *)view->grep.obj;
		cy->radius += 0.2;
		cy->height += 0.5;
		make_cylinder_cap2(cy);
	}
}

void	zoom_inout(int button, t_view *view)
{
	if (button == 4)
		zomm_in(view);
	else
		zoom_out(view);
	newwin(view);
}

int	mouse_press(int button, int x, int y, t_view *view)
{
	(void)x;
	(void)y;
	if (button == 2 || button == 1)
	{
		mlx_mouse_get_pos(view->win, &x, &y);
		grep_obj(x, y, view);
		view->click_status = 1;
	}
	if ((button == 4 || button == 5) && view->click_status)
		zoom_inout(button, view);
	return (0);
}

int	mouse_release(int button, int x, int y, t_view *view)
{
	(void)x;
	(void)y;
	if (button == 2 || button == 1)
		view->click_status = 0;
	return (0);
}

void	mlx_engine(t_view *view)
{
	mlx_hook(view->win, 2, 1L << 0, key_hook, view);
	mlx_hook(view->win, 4, 1L << 2, mouse_press, view);
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
	srand((unsigned int)time(NULL));
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
