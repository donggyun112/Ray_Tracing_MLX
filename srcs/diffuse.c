/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 06:27:00 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 08:24:55 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

float	cos_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas, int light)
{
	t_vec3	normal;
	t_vec3	l_vec;
	t_vec3	hit;
	float	angle;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (sp->type == TSP && sp->bumppath != NULL)
		normal = bump_sphere(sp, sp->bumtexture, hit);
	else
		normal = norm_vec(sub_vector(hit, sp->center));
	l_vec = norm_vec(sub_vector(canvas.obj->l[light].light_orig, hit));
	angle = scalar_product(normal, l_vec);
	if (angle < 0.0)
		return (0.0);
	return (angle);
}

float	cos_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas, int light)
{
	t_vec3	hit;
	t_vec3	l_vec;
	t_vec3	norm;
	float	angle;

	norm = check_plane_direction(pl, ray);
	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	l_vec = norm_vec(sub_vector(canvas.obj->l[light].light_orig, hit));
	angle = scalar_product(norm, l_vec);
	if (angle < 0.0)
		return (0.0);
	return (angle);
}

float	get_hit_height(t_cylinder *cy, t_vec3 hit)
{
	float	hit_height;
	float	dist;
	t_vec3	center_to_hit;

	center_to_hit = sub_vector(hit, cy->center);
	dist = size_of_vec(center_to_hit);
	if (dist < cy->radius)
		dist = cy->radius;
	hit_height = sqrt(pow(dist, 2) - pow(cy->radius, 2));
	if (scalar_product(center_to_hit, cy->dir) < 0.0)
		return (-1.0 * hit_height);
	else
		return (hit_height);
}

float	cos_cy(t_cylinder *cy, t_ray3 *ray, t_canvas canvas, int light)
{
	t_vec3	normal;
	t_vec3	l_vec;
	t_vec3	hit;
	float	angle;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	normal = sub_vector(hit, add_vector(cy->center, \
		multiple_vector(get_hit_height(cy, hit), cy->dir)));
	if (cy->type == TCY && cy->bumppath != NULL)
		normal = bump_cylinder(cy, cy->bumtexture, hit, normal);
	l_vec = norm_vec(sub_vector(canvas.obj->l[light].light_orig, hit));
	angle = scalar_product(normal, l_vec);
	if (angle < 0.0)
		return (0.0);
	return (angle);
}
