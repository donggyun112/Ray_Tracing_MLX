/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:48:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/01 23:00:44 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	intersection(t_ray3 *ray, t_volume *obj, t_canvas canvas)
{
	int	idx;

	idx = 0;
	while (idx < obj->sp_cnt)
	{
		hit_sphere(ray, &obj->sp[idx], canvas);
		idx++;
	}
	idx = 0;
	while (idx < obj->pl_cnt)
	{
		hit_plane(ray, &obj->pl[idx], canvas);
		idx++;
	}
	idx = 0;
	while (idx < obj->cy_cnt)
	{
		hit_cylinder(ray, &obj->cy[idx], canvas);
		idx++;
	}
}

void	color_cal(t_view *view, t_canvas canvas, t_ray3 *ray, int pix[])
{
	if (ray->t > 0.0)
		ray_color(canvas, ray);
	else
		my_mlx_pixel_put(view, pix[0], pix[1], 0x00FFFFFF);
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
			intersection(ray, view->can.obj, view->can);
			color_cal(view, view->can, ray, pix);
			color.r += ray->color[RED];
			color.g += ray->color[GREEN];
			color.b += ray->color[BLUE];
		}
	}
	return (color);
}

void	low_quality(int scalar, int pix[2], t_ray3 ray, t_view *view)
{
	int		offset[2];
	int		new_x;
	int		new_y;
	t_color	c;

	offset[0] = -1;
	while (++offset[0] < scalar)
	{
		offset[1] = -1;
		while (++offset[1] < scalar)
		{
			new_x = pix[0] + offset[0];
			new_y = pix[1] + offset[1];
			if (new_x < view->can.width && new_y < view->can.height)
			{
				if (ray.t > 0.0)
					my_mlx_pixel_put(view, new_x, new_y, rgb_to_int(ray.color));
				else
				{
					c = get_texture_color(view->back, ((float)pix[0] / view->can.width), ((float)pix[1] / view->can.height));
					ray.color[RED] = c.r;
					ray.color[GREEN] = c.g;
					ray.color[BLUE] = c.b;
					my_mlx_pixel_put(view, new_x, new_y, rgb_to_int(ray.color));
				}
			}
		}
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
	pix[1] = 0;
	set_quality_scalar(view);
	while (pix[1] < canvas.height)
	{
		pix[0] = 0;
		vp_idx[1] = 2.0 * (float)pix[1] / (float)canvas.height;
		while (pix[0] < canvas.width)
		{
			c = anti_aliasing(pix, vp_idx, view, &ray);
			ray.color[RED] = c.r / (view->anti_scalar * view->anti_scalar);
			ray.color[GREEN] = c.g / (view->anti_scalar * view->anti_scalar);
			ray.color[BLUE] = c.b / (view->anti_scalar * view->anti_scalar);
			low_quality(view->low_scalar, pix, ray, view);
			pix[0] += view->low_scalar;
		}
		pix[1] += view->low_scalar;
	}
}

void	*make_image2(void *m)
{
	int		pix[2];
	float	vp_idx[2];
	t_ray3	ray;
	t_color	c;
	t_view	*view;
	t_canvas	canvas;
	t_thread	*t;

	t = (t_thread *)m;
	canvas = t->canvas;
	view = t->view;
	pix[1] = t->id;
	while (pix[1] < canvas.height)
	{
		// printf("%d\n", t->id);
		pix[0] = 0;
		vp_idx[1] = 2.0 * (float)pix[1] / (float)canvas.height;
		while (pix[0] < canvas.width)
		{
			c = anti_aliasing(pix, vp_idx, view, &ray);
			ray.color[RED] = c.r / (view->anti_scalar * view->anti_scalar);
			ray.color[GREEN] = c.g / (view->anti_scalar * view->anti_scalar);
			ray.color[BLUE] = c.b / (view->anti_scalar * view->anti_scalar);
			low_quality(view->low_scalar, pix, ray, view);
			pix[0] += view->low_scalar;
		}
		pix[1] += (view->low_scalar + 6);
	}
	return (NULL);
}

t_thread	*init_thread(t_view *view)
{
	t_thread	*m;

	m = (t_thread *)malloc(sizeof(t_thread) * 7);
	set_quality_scalar(view);
	for (int x = 0; x < 7; x++)
	{
		m[x].id = x;
		m[x].view = view;
		m[x].canvas = view->can;
	}
	return (m);
}

void	multi_rend(t_view *view)
{
	t_thread	*m;

	m = init_thread(view);
	pthread_mutex_init(&view->mutex, NULL);
	for (int x = 0; x < 7; x++)
		pthread_create(&m[x].thread, NULL, make_image2, &m[x]);
	for (int x = 0; x < 7; x++)
		pthread_join(m[x].thread, NULL);
}

void	set_texture(t_view *view, t_volume *obj)
{
	int i;

	i = -1;
	init_texture(&view->back, view, "space.xpm");
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
	{
		if (obj->pl[i].type == TPL)
			init_texture(&obj->pl[i].texture, view, obj->pl[i].filepath);
	}
	view->anti_scalar = 1;
	view->low_scalar = 1;
	view->quality_scalar = -2;
	view->flag = 0;
}

int	loop_hook(t_view *view)
{
	int	x;

	if (view->flag)
	{
		x = 0;
		while (x < view->can.obj->sp_cnt)
		{
			if (view->can.obj->sp[x].type == TSP)
				view->can.obj->sp[x].angle += 0.05;
			else if (view->can.obj->sp[x].type == CSP)
				view->can.obj->sp[x].angle += 0.2;
			if (view->can.obj->sp[x].angle > 360.1)
				view->can.obj->sp[x].angle = 0.0;
			x++;
		}
		newwin(view);
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

int	main(int argc, char *argv[])
{
	t_view		view;
	t_canvas	canvas;

	if (argc != 2)
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
	set_texture(&view, canvas.obj);
	multi_rend(&view);
	// make_image(&view, canvas);
	mlx_put_image_to_window(view.mlx, view.win, view.img, 0, 0);
	mlx_hook(view.win, 2, 1L << 0, key_hook, &view);
	mlx_hook(view.win, 17, 1L << 5, win_destroy, &view);
	mlx_loop_hook(view.mlx, loop_hook, &view);
	mlx_loop(view.mlx);
	return (0);
}
