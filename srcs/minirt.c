/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:48:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/04 23:38:59 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include <OpenGL/OpenGL.h>

#define	num_of_thread 7

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
	(void)view;
	(void)pix;
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
	const float	po[2] = {((float)pix[0] / view->can.width), ((float)pix[1] / view->can.height)};
	int		new_x;
	int		new_y;
	t_color	c;

	offset[0] = 0;
	while (offset[0] < scalar)
	{
		offset[1] = 0;
		while (offset[1] < scalar)
		{
			new_x = pix[0] + offset[0];
			new_y = pix[1] + offset[1];
			if (new_x < view->can.width && new_y < view->can.height)
			{
				if (ray.t > 0.0)
					my_mlx_pixel_put(view, new_x, new_y, rgb_to_int(ray.color));
				else if (view->can.bgt_filepath)
				{
					c = get_texture_color(view->back, po[0], po[1]);
					ray.color[RED] = c.r;
					ray.color[GREEN] = c.g;
					ray.color[BLUE] = c.b;
					my_mlx_pixel_put(view, new_x, new_y, rgb_to_int(ray.color));
				}
				else
					my_mlx_pixel_put(view, new_x, new_y, 0xFFFFFF);
			}
			offset[1]++;

		}
		offset[0]++;
	}
}

void	*make_image2(void *m)
{
	int		pix[3];
	float	vp_idx[2];
	t_ray3	ray;
	t_color	c;
	t_thread	*t;
	int	anti;

	t = (t_thread *)m;
	anti = t->view->anti_scalar * t->view->anti_scalar;
	pix[1] = t->id * (t->canvas.height / num_of_thread);
	if (t->id == num_of_thread - 1)
		pix[2] = t->canvas.height;
	else
		pix[2] = (t->id + 1) * (t->canvas.height / num_of_thread);
	while (pix[1] < pix[2] && pix[1] < t->canvas.height)
	{
		pix[0] = 0;
		while (pix[0] < t->canvas.width)
		{
			c = anti_aliasing(pix, vp_idx, t->view, &ray);
			ray.color[RED] = c.r / anti;
			ray.color[GREEN] = c.g / anti;
			ray.color[BLUE] = c.b / anti;
			low_quality(t->view->low_scalar, pix, ray, t->view);
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
			ray.color[RED] = c.r / (view->anti_scalar * view->anti_scalar);
			ray.color[GREEN] = c.g / (view->anti_scalar * view->anti_scalar);
			ray.color[BLUE] = c.b / (view->anti_scalar * view->anti_scalar);
			low_quality(view->low_scalar, pix, ray, view);
			pix[0] += view->low_scalar;
		}
		pix[1] += view->low_scalar;
	}
}

t_thread	*init_thread(t_view *view)
{
	t_thread	*m;

	m = (t_thread *)malloc(sizeof(t_thread) * num_of_thread);
	set_quality_scalar(view);
	for (int x = 0; x < num_of_thread; x++)
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
	for (int x = 0; x < num_of_thread; x++)
		pthread_create(&m[x].thread, NULL, make_image2, &m[x]);
	for (int x = 0; x < num_of_thread; x++)
		pthread_join(m[x].thread, NULL);
}

void	set_texture(t_view *view, t_volume *obj)
{
	int i;

	i = -1;
	if (view->can.bgt_filepath)
		init_texture(&view->back, view, view->can.bgt_filepath);
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
	while (++i < obj->cy_cnt)
	{
		if (obj->cy[i].type == TCY)
		{
			printf("%s\n", obj->cy[i].filepath);
			init_texture(&obj->cy[i].texture, view, obj->cy[i].filepath);
		}
	}
	view->anti_scalar = 1;
	view->low_scalar = 1;
	view->quality_scalar = -2;
	view->flag = 0;
	view->focus = 0;
	view->stop = 1;
}

t_vec3 rotate_around_specific_point(t_vec3 vec, t_vec3 center, float angle)
{
	// 1. 이동 단계: 회전 중심 좌표를 원점으로 이동
	t_vec3 translated_vec;
	translated_vec.x = vec.x - center.x;
	translated_vec.y = vec.y - center.y;
	translated_vec.z = vec.z - center.z;

	// 2. 회전 단계: 원점을 중심으로 벡터를 Y축 방향으로 회전
	const float cos_angle = cos(angle);
	const float sin_angle = sin(angle);

	t_vec3 rotated_vec;
	rotated_vec.x = translated_vec.x * cos_angle - translated_vec.z * sin_angle;
	rotated_vec.y = translated_vec.y;  // Y축을 중심으로 회전하기 때문에 Y 값은 변하지 않습니다.
	rotated_vec.z = translated_vec.x * sin_angle + translated_vec.z * cos_angle;

	// 3. 복원 단계: 원래 위치로 벡터를 이동
	rotated_vec.x += center.x;
	rotated_vec.y += center.y;
	rotated_vec.z += center.z;

	return rotated_vec;
}


int	loop_hook(t_view *view)
{
	int	x;

	if (view->flag && view->stop)
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
		// view->can.obj->sp[2].center = sub_vector(view->can.obj->sp[2].center, view->can.obj->sp[1].center);

		
	
		view->can.obj->sp[1].center = sub_vector(view->can.obj->sp[1].center, view->can.obj->sp[0].center);
		view->can.obj->sp[1].center = rotate_around_axis(view->can.obj->sp[1].center, (t_vec3){0.0f, 1.0f, 0.0f}, 0.05);
		view->can.obj->sp[1].center = add_vector(view->can.obj->sp[1].center, view->can.obj->sp[0].center);

		// view->can.obj->sp[2].center = sub_vector(view->can.obj->sp[2].center, view->can.obj->sp[1].center);
		// view->can.obj->sp[2].center = rotate_around_axis(view->can.obj->sp[2].center, (t_vec3){0.0f, 1.0f, 0.0f}, 0.05);
		// view->can.obj->sp[2].center = add_vector(view->can.obj->sp[2].center, view->can.obj->sp[1].center);
		// view->can.obj->sp[1].center = rotate_around_specific_point(view->can.obj->sp[1].center, view->can.obj->sp[0].center, 0.05);
	
		// view->can.obj->sp[2].center = add_vector(view->can.obj->sp[2].center, view->can.obj->sp[1].center);
		// view->can.obj->sp[2].center = rotate_around_specific_point(view->can.obj->sp[2].center, view->can.obj->sp[1].center, 0.05);
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

int mouse_motion(int x, int y, t_view *view)
{
	static int	pos[2];

	mlx_mouse_get_pos(view->win, &x, &y);
	if ((pos[0] != x || pos[1] != y) && view->stop)
	{
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
	mlx_mouse_hide();
	mlx_mouse_move(view.win, view.can.width / 2, view.can.height / 2);
	mlx_put_image_to_window(view.mlx, view.win, view.img, 0, 0);
	mlx_hook(view.win, 2, 1L << 0, key_hook, &view);
	mlx_hook(view.win, 3, 1L << 1, key_release, &view);
	mlx_hook(view.win, 17, 1L << 5, win_destroy, &view);
	mlx_hook(view.win, 6, 1L << 7, mouse_motion, &view);
	mlx_loop_hook(view.mlx, loop_hook, &view);
	mlx_loop(view.mlx);
	exit(0);
}
