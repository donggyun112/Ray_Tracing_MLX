/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:48:10 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/25 21:09:27 by seodong-gyu      ###   ########.fr       */
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
}

void	color_cal(t_view *view, t_canvas canvas, t_ray3 *ray, int pix[])
{
	if (ray->t > 0.0)
	{	
		ray_color(canvas, ray);
		// my_mlx_pixel_put(view, pix[0], pix[1], rgb_to_int(ray->color));
	}
/*
		if (ray->type == 111 && ray->color[RED] != 255)
		{
			my_mlx_pixel_put(view, pix[0], pix[1], rgb_to_int(ray->color));
		}
		else
		{
			my_mlx_pixel_put(view, pix[0], pix[1], 0x0000FF00 + 0x000000FF * angle);
		}
		if (angle > 0.999)
			my_mlx_pixel_put(view, pix[0], pix[1], 0xAAFFFFFF);
	}
*/
	else
		my_mlx_pixel_put(view, pix[0], pix[1], 0x00FFFFFF);
}

void	make_image(t_view *view, t_canvas canvas, t_camera cam)
{
	int		pix[2];
	double	vp_idx[2];
	t_ray3	ray;
	int		samplesPerPixel = 2;  // 예: 각 픽셀당 4x4 슈퍼샘플링
	int	color2[3];

	pix[1] = 0;
	while (pix[1] < canvas.height)
	{
		pix[0] = 0;
		// vp_idx[1] = 2.0 * (double)pix[1] / (double)canvas.height;
		while (pix[0] < canvas.width)
		{
			Color accumulatedColor = {0, 0, 0};

			for (int sy = 0; sy < samplesPerPixel; sy++) {
				for (int sx = 0; sx < samplesPerPixel; sx++) {
					double u_offset = (float)sx / samplesPerPixel;
					double v_offset = (float)sy / samplesPerPixel;

					vp_idx[0] = canvas.ratio * 2.0 * ((double)pix[0] + u_offset) / (double)canvas.width;
					vp_idx[1] = 2.0 * ((double)pix[1] + v_offset) / (double)canvas.height;

					ray = create_ray(cam, vp_idx[0], vp_idx[1]);
					intersection(&ray, canvas.obj, canvas);
					color_cal(view, canvas, &ray, pix);
					accumulatedColor.r += ray.color[RED];
					accumulatedColor.g += ray.color[GREEN];
					accumulatedColor.b += ray.color[BLUE];
				}
			}

			// 평균 색상 계산
			color2[RED] = accumulatedColor.r / (samplesPerPixel * samplesPerPixel);
			color2[GREEN] = accumulatedColor.g / (samplesPerPixel * samplesPerPixel);
			color2[BLUE] = accumulatedColor.b / (samplesPerPixel * samplesPerPixel);

			// 최종 색상을 이미지 버퍼에 저장
			if (ray.t > 0.0)
				my_mlx_pixel_put(view, pix[0], pix[1], rgb_to_int(color2));
			else
				my_mlx_pixel_put(view, pix[0], pix[1], 0x00FFFFFF);

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
	view.cam = cam;
	view.can = canvas;
	view.mlx = mlx_init();
	view.win = mlx_new_window(view.mlx, canvas.width, canvas.height, "miniRT");
	view.img = mlx_new_image(view.mlx, canvas.width, canvas.height);
	view.addr = mlx_get_data_addr(view.img, &view.bits_per_pixel, \
		&view.line_length, &view.endian);
	make_image(&view, canvas, cam); // viewport를 향해서 반복문 사용하여 ray 발사
	mlx_put_image_to_window(view.mlx, view.win, view.img, 0, 0);
	mlx_hook(view.win, 2, 1L << 0, key_hook, &view);
	mlx_hook(view.win, 17, 1L << 5, win_destroy, &view);
	mlx_loop(view.mlx);
	return (0);
}
