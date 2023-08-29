/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 14:30:31 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/28 21:38:14 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

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

double	get_hit_height(t_cylinder *cy, t_vec3 hit)
{
	double	hit_height;
	double	dist;
	t_vec3	center_to_hit;

	center_to_hit = sub_vector(hit, cy->center);
	dist = size_of_vec2(center_to_hit);
	if (dist < cy->radius)
		dist = cy->radius;
	hit_height = sqrt(pow(dist, 2) - pow(cy->radius, 2));
	if (scalar_product(center_to_hit, cy->dir) < 0.0)
		return (-1.0 * hit_height);
	else
		return (hit_height);
}

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

t_vec3	reflection(t_vec3 normal, t_vec3 light)
{
	t_vec3	ret;
	double	k;

	k = 2 * scalar_product(light, normal);
	ret = sub_vector(multiple_vector(k, normal), light);
	return (ret);
}

double	ref_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	normal;
	t_vec3	light;
	t_vec3	hit;
	t_vec3	reflect;
	double	ret;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	light = norm_vec(sub_vector(canvas.light_orig, hit));
	normal = norm_vec(sub_vector(hit, sp->center));
	reflect = norm_vec(reflection(normal, light));
	ret = scalar_product(norm_vec(sub_vector(ray->origin, hit)), \
		norm_vec(reflect));
	if (ret < 0.0)
		ret = 0.0;
	return (ret);
}

double	ref_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	light;
	t_vec3	hit;
	t_vec3	reflect;
	double	ret;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	light = norm_vec(sub_vector(canvas.light_orig, hit));
	reflect = norm_vec(reflection(pl->norm, light));
	ret = scalar_product(norm_vec(sub_vector(ray->origin, hit)), reflect);
	if (ret < 0.0)
		ret = 0.0;
	return (ret);
}

double	ref_cy(t_cylinder *cy, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	light;
	t_vec3	hit;
	t_vec3	reflect;
	t_vec3	normal;
	double	ret;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	light = norm_vec(sub_vector(canvas.light_orig, hit));
	normal = sub_vector(hit, add_vector(cy->center, \
		multiple_vector(get_hit_height(cy, hit), cy->dir)));
	normal = norm_vec(normal);
	reflect = norm_vec(reflection(normal, light));
	ret = scalar_product(norm_vec(sub_vector(ray->origin, hit)), reflect);
	if (ret < 0.0)
		ret = 0.0;
	return (ret);
}
