/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:48:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/03 02:09:13 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	intersection(t_ray3 *ray, t_volume *obj, t_canvas canvas)
{
	int	idx;

	idx = 0;
	while (idx < obj->sp_cnt)
	{
		hit_sphere(ray, &obj->sp[idx]);
		idx++;
	}
	idx = 0;
	while (idx < obj->pl_cnt)
	{
		hit_plane(ray, &obj->pl[idx], canvas);
		idx++;
	}
}

double	cos_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	normal;
	t_vec3	light;
	t_vec3	hit;
	double	angle;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	normal = norm_vec(sub_vector(hit, sp->center));
	light = norm_vec(sub_vector(canvas.light_orig, hit));
	angle = scalar_product(normal, light);
	angle = (angle + 1.0) / 2.0;
	return (angle);
}

double	cos_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	hit;
	t_vec3	light;
	double	angle;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	light = norm_vec(sub_vector(canvas.light_orig, hit));
	angle = scalar_product(pl->norm, light);
	angle = (angle + 1.0) / 2.0;
	return (angle);
}

void	color_cal(t_view *view, t_canvas canvas, t_ray3 *ray, int pix[])
{
	double	angle;

	if (ray->t > 0.0)
	{	
		if (ray->type == SP)
			angle = cos_sp(ray->obj, ray, canvas);
		else if (ray->type == PL)
			angle = cos_pl(ray->obj, ray, canvas);
		else
			angle = -1.0;
		if (angle > 0.999)
			my_mlx_pixel_put(view, pix[0], pix[1], 0x00FFFFFF);
		else if (ray->type ==  PL && ray->color[RED] == 100 && ray->color[BLUE] == 100)
		{
			my_mlx_pixel_put(view, pix[0], pix[1], 0x00000000);
		}
		else
		{
			my_mlx_pixel_put(view, pix[0], pix[1], 0x0000FF00 + 0x000000FF * angle);
		}
	}
	else
		my_mlx_pixel_put(view, pix[0], pix[1], 0x00FFFFFF);
}

void	make_image(t_view *view, t_canvas canvas, t_camera cam)
{
	int		pix[2];
	double	vp_idx[2];
	t_ray3	ray;

	pix[1] = 0;
	while (pix[1] < canvas.height)
	{
		pix[0] = 0;
		vp_idx[1] = 2.0 * (double)pix[1] / (double)canvas.height;
		while (pix[0] < canvas.width)
		{
			vp_idx[0] = canvas.ratio * 2.0 * (double)pix[0] \
				/ (double)canvas.width;
			ray = create_ray(cam, vp_idx[0], vp_idx[1]);
			intersection(&ray, canvas.obj, canvas);
			color_cal(view, canvas, &ray, pix); // put pixel info in this fn
			pix[0]++;
		}
		pix[1]++;
	}
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
	view.mlx = mlx_init();
	view.win = mlx_new_window(view.mlx, 1920, 1080, "miniRT");
	view.img = mlx_new_image(view.mlx, 1920, 1080);
	view.addr = mlx_get_data_addr(view.img, &view.bits_per_pixel, \
		&view.line_length, &view.endian);
	canvas.obj->sp->color[RED] = 100;
	canvas.obj->sp->color[GREEN] = 100;
	canvas.obj->sp->color[BLUE] = 100;
	printf ("%d\n,", rgb_to_int(canvas.obj->sp->color));
	make_image(&view, canvas, cam); // viewport를 향해서 반복문 사용하여 ray 발사
	mlx_put_image_to_window(view.mlx, view.win, view.img, 0, 0);
	mlx_hook(view.win, 17, 1L << 5, win_destroy, &view);
	mlx_hook(view.win, 2, 1L << 0, key_hook, &view);
	mlx_loop(view.mlx);
	return (0);
}
