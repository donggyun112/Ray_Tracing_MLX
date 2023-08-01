/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:48:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/01 17:19:11 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	intersection(t_ray3 *ray, t_volume *obj)
{
	int	idx;

	idx = 0;
	while (idx < obj->sp_cnt)
	{
		hit_sphere(ray, &obj->sp[idx]);
		idx++;
	}
}

double	test_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	normal;
	t_vec3	light;
	t_vec3	hit;
	double	angle;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	normal = norm_vec(sub_vector(hit, sp->center));
	light = norm_vec(sub_vector(canvas.light_orig, sp->center));
	angle = scalar_product(normal, light);
	angle = (angle + 1.0) / 2.0;
	return (angle);
}

void	color_cal(t_view *view, t_canvas canvas, t_ray3 *ray, int pix[])
{
	double	angle;

	(void)canvas;
	(void)ray;
	if (ray->t > 0.0)
	{
		angle = test_sp(ray->obj, ray, canvas);
		my_mlx_pixel_put(view, pix[0], pix[1], 0x0000FF00 + 0x000000FF * angle);
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
			intersection(&ray, canvas.obj);
			color_cal(view, canvas, &ray, pix); // put pixel info in this fn
			pix[0]++;
		}
		pix[1]++;
	}
	/*
	for (int i = 0; i < canvas.height; i++)
	{
		for (int j = 0; j < canvas.width; j++)
		{
			u,v값을 i,j에 맞게 변환
			ray = create_ray();
			intersection(&ray, canvas.volume);
			color_cal(view, canvas, &ray);	-> my_pixel_put_mlx()
		}
	}
	*/
}

int	main(int argc, char *argv[])
{
	t_view		view;
	t_canvas	canvas;
	t_camera	cam;

	(void)argc;
	(void)argv;
	// canvas = parse(argv);
	canvas = tmp_parse();
	cam = camera(canvas);
	view.mlx = mlx_init();
	view.win = mlx_new_window(view.mlx, 1920, 1080, "miniRT");
	view.img = mlx_new_image(view.mlx, 1920, 1080);
	view.addr = mlx_get_data_addr(view.img, &view.bits_per_pixel, \
		&view.line_length, &view.endian);
	make_image(&view, canvas, cam); // viewport를 향해서 반복문 사용하여 ray 발사
	mlx_put_image_to_window(view.mlx, view.win, view.img, 0, 0);
	mlx_hook(view.win, 17, 1L << 5, win_destroy, &view);
	mlx_hook(view.win, 2, 1L << 0, key_hook, &view);
	mlx_loop(view.mlx);
	return (0);
}
