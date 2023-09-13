/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 00:50:37 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 06:33:53 by jinhyeop         ###   ########.fr       */
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

int	diff_light(t_canvas canvas, t_ray3 *ray, int idx, int light)
{
	int	ret;

	ret = (float)ray->color[idx] * ray->angle[DIFF] \
		* canvas.obj->l[light].light_bright \
		* ((float)canvas.obj->l[light].light_col[idx] / 255.0);
	return (ret);
}

int	phong_light(t_canvas canvas, t_ray3 *ray, int idx, int light)
{
	int	ret;

	ret = (float)canvas.obj->l[light].light_col[idx] * \
		canvas.obj->l[light].light_bright * pow(ray->angle[REF], 30);
	return (ret);
}

int	add_color(t_canvas canvas, t_ray3 *ray, int idx, int light)
{
	int	color;

	color = 0;
	if (hit_shadow(ray, canvas, light) == 0)
		color = diff_light(canvas, ray, idx, light) + \
		phong_light(canvas, ray, idx, light);
	return (color);
}

void	ray_color(t_canvas canvas, t_ray3 *ray, int light)
{
	if (ray->type == SP)
	{
		ray->angle[DIFF] = cos_sp(ray->obj, ray, canvas, light);
		ray->angle[REF] = ref_sp(ray->obj, ray, canvas, light);
	}
	else if (ray->type == PL || ray->type == TRI || ray->type == CAP)
	{
		ray->angle[DIFF] = cos_pl(ray->obj, ray, canvas, light);
		ray->angle[REF] = ref_pl(ray->obj, ray, canvas, light);
	}
	else if (ray->type == CY)
	{
		ray->angle[DIFF] = cos_cy(ray->obj, ray, canvas, light);
		ray->angle[REF] = ref_cy(ray->obj, ray, canvas, light);
	}
	else
	{
		ray->angle[DIFF] = 0.0;
		ray->angle[REF] = 0.0;
	}
	ray->real[RED] += add_color(canvas, ray, RED, light);
	ray->real[GREEN] += add_color(canvas, ray, GREEN, light);
	ray->real[BLUE] += add_color(canvas, ray, BLUE, light);
}
