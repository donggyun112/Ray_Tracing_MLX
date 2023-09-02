/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 00:50:37 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/03 02:24:51 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	amb_light(t_canvas canvas, t_ray3 *ray, int idx)
{
	int	ret;

	ret = (float)ray->color[idx] * canvas.amb_bright \
		* ((float)canvas.amb_col[idx] / 255.0);
	return (ret);
}

int	diff_light(t_canvas canvas, t_ray3 *ray, float angle, int idx)
{
	int	ret;

	ret = (float)ray->color[idx] * angle * canvas.obj->l[0].light_bright \
		* ((float)canvas.obj->l[0].light_col[idx] / 255.0);
	return (ret);
}

int	phong_light(t_canvas canvas, t_ray3 *ray, float angle, int idx)
{
	int	ret;

	(void)ray;
	ret = (float)canvas.obj->l[0].light_col[idx] * \
	canvas.obj->l[0].light_bright * pow(angle, 30);
	return (ret);
}

int	add_color(t_canvas canvas, t_ray3 *ray, float *angle, int idx)
{
	int	color;

	color = amb_light(canvas, ray, idx);
	if (ray->type != SHADOW)
		color = color + diff_light(canvas, ray, angle[0], idx) + \
		phong_light(canvas, ray, angle[1], idx);
	if (color > 255)
		color = 255;
	return (color);
}

void	ray_color(t_canvas canvas, t_ray3 *ray)
{
	float	angle[2];

	if (ray->type == SP)
	{
		angle[0] = cos_sp(ray->obj, ray, canvas);
		angle[1] = ref_sp(ray->obj, ray, canvas);
	}
	else if (ray-> type == PL)
	{
		angle[0] = cos_pl(ray->obj, ray, canvas);
		angle[1] = ref_pl(ray->obj, ray, canvas);
	}
	else if (ray->type == CY)
	{
		angle[0] = cos_cy(ray->obj, ray, canvas);
		angle[1] = ref_cy(ray->obj, ray, canvas);
	}
	else
	{
		angle[0] = 0.0;
		angle[1] = 0.0;
	}
	ray->color[RED] = add_color(canvas, ray, angle, RED);
	ray->color[GREEN] = add_color(canvas, ray, angle, GREEN);
	ray->color[BLUE] = add_color(canvas, ray, angle, BLUE);
}
