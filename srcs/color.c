/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 00:50:37 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/04 06:54:18 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	amb_light(t_canvas canvas, t_ray3 *ray, int idx)
{
	int	ret;

	ret = (double)ray->color[idx] * canvas.amb_bright \
		* ((double)canvas.amb_col[idx] / 255.0);
	return (ret);
}

int	diff_light(t_canvas canvas, t_ray3 *ray, double angle, int idx)
{
	int	ret;

	ret = (double)ray->color[idx] * angle * canvas.light_bright \
		* ((double)canvas.light_col[idx] / 255.0);
	return (ret);
}

int	phong_light(t_canvas canvas, t_ray3 *ray, int idx)
{
}

int	add_color(t_canvas canvas, t_ray3 *ray, double angle, int idx)
{
	int	color;

	color = amb_light(canvas, ray, idx);
	if (ray->type != SHADOW)
		color = color + diff_light(canvas, ray, angle, idx) \
			+ phong_light(canvas, ray, idx);
	if (color > 255)
		color = 255;
	return (color);
}

void	ray_color(t_canvas canvas, t_ray3 *ray)
{
	double	angle;

	if (ray->type == SP)
		angle = cos_sp(ray->obj, ray, canvas);
	else if (ray-> type == PL)
		angle = cos_pl(ray->obj, ray, canvas);
	else
		angle = 0.0;
	ray->color[RED] = add_color(canvas, ray, angle, RED);
	ray->color[GREEN] = add_color(canvas, ray, angle, GREEN);
	ray->color[BLUE] = add_color(canvas, ray, angle, BLUE);
}
