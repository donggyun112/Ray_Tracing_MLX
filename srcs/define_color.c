/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_color.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:48:07 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 01:48:25 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

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
