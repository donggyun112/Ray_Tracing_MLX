/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:48:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/29 22:06:34 by dongkseo         ###   ########.fr       */
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


t_color	anti_aliasing(int pix[2], double vp_idx[2], t_view *view, t_ray3 *ray)
{
	t_color	color;
	double	offset[2];
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
			vp_idx[0] = view->can.ratio * 2.0 * ((double)pix[0] + \
			offset[0]) / (double)view->can.width;
			vp_idx[1] = 2.0 * ((double)pix[1] + \
			offset[1]) / (double)view->can.height;
			*ray = create_ray(view->cam, vp_idx[0], vp_idx[1]);
			intersection(ray, view->can.obj, view->can);
			color_cal(view, view->can, ray, pix);
			color.r += ray->color[RED];
			color.g += ray->color[GREEN];
			color.b += ray->color[BLUE];
		}
	}
	return (color);
}

void	low_quality(int scalar, int pix[2], t_ray3 ray, t_canvas canvas, t_view *view)
{
	int	offset[2];
	int	new_x;
	int	new_y;

	offset[0] = -1;
	while (++offset[0] < scalar)
	{
		offset[1] = -1;
		while (++offset[1] < scalar)
		{
			new_x = pix[0] + offset[0];
			new_y = pix[1] + offset[1];
			if (new_x < canvas.width && new_y < canvas.height)
			{
				if (ray.t > 0.0)
					my_mlx_pixel_put(view, new_x, new_y, rgb_to_int(ray.color));
				else
					my_mlx_pixel_put(view, new_x, new_y, 0x00FFFFFF);
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
	double	vp_idx[2];
	t_ray3	ray;
	t_color	c;

	pix[1] = 0;
	set_quality_scalar(view);
	while (pix[1] < canvas.height)
	{
		pix[0] = 0;
		vp_idx[1] = 2.0 * (double)pix[1] / (double)canvas.height;
		while (pix[0] < canvas.width)
		{
			c = anti_aliasing(pix, vp_idx, view, &ray);
			ray.color[RED] = c.r / (view->anti_scalar * view->anti_scalar);
			ray.color[GREEN] = c.g / (view->anti_scalar * view->anti_scalar);
			ray.color[BLUE] = c.b / (view->anti_scalar * view->anti_scalar);
			low_quality(view->low_scalar, pix, ray, canvas, view);
			pix[0] += view->low_scalar;
		}
		pix[1] += view->low_scalar;
	}
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
	{
		if (obj->pl[i].type == TPL)
			init_texture(&obj->pl[i].texture, view, obj->pl[i].filepath);
	}
	view->anti_scalar = 1;
	view->low_scalar = 1;
	view->quality_scalar = -2;
}

int	main(int argc, char *argv[])
{
	t_view		view;
	t_canvas	canvas;
	t_camera	cam;

	if (argc != 2)
	{
		printf("Error\nInput mapfile(*.rt) as argument\n");
		return (1);
	}
	canvas = parse(argv);
	cam = camera(canvas);
	view.cam = cam;
	view.can = canvas;
	view.mlx = mlx_init();
	view.win = mlx_new_window(view.mlx, canvas.width, canvas.height, "miniRT");
	view.img = mlx_new_image(view.mlx, canvas.width, canvas.height);
	view.addr = mlx_get_data_addr(view.img, &view.bits_per_pixel, \
		&view.line_length, &view.endian);
	set_texture(&view, canvas.obj);
	make_image(&view, canvas);
	mlx_put_image_to_window(view.mlx, view.win, view.img, 0, 0);
	mlx_hook(view.win, 2, 1L << 0, key_hook, &view);
	mlx_hook(view.win, 17, 1L << 5, win_destroy, &view);
	mlx_loop(view.mlx);
	return (0);
}
