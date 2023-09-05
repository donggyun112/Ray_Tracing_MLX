/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angle_cos.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 00:31:12 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/06 01:14:00 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

double	cos_cy(t_cylinder *cy, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	normal;
	t_vec3	light;
	t_vec3	hit;
	double	angle;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	normal = sub_vector(hit, add_vector(cy->center, \
		multiple_vector(get_hit_height(cy, hit), cy->dir)));
	normal = norm_vec(normal);
	light = norm_vec(sub_vector(canvas.light_orig, hit));
	angle = scalar_product(normal, light);
	if (angle < 0.0)
		return (0.0);
	return (angle);
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
	if (angle < 0.0)
		return (0.0);
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
	if (angle < 0.0)
		return (0.0);
	return (angle);
}
