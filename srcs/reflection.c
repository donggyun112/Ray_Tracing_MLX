/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 14:30:31 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 06:32:18 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3	reflection(t_vec3 normal, t_vec3 light)
{
	t_vec3	ret;
	float	k;

	k = 2 * scalar_product(light, normal);
	ret = sub_vector(multiple_vector(k, normal), light);
	return (ret);
}

float	ref_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas, int light)
{
	t_vec3	normal;
	t_vec3	l_vec;
	t_vec3	hit;
	t_vec3	reflect;
	float	ret;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	l_vec = norm_vec(sub_vector(canvas.obj->l[light].light_orig, hit));
	if (sp->type == TSP && sp->bumppath != NULL)
		normal = bump_sphere(sp, sp->bumtexture, hit);
	else
		normal = norm_vec(sub_vector(hit, sp->center));
	reflect = norm_vec(reflection(normal, l_vec));
	ret = scalar_product(norm_vec(\
	sub_vector(ray->origin, hit)), norm_vec(reflect));
	if (ret < 0.0)
		ret = 0.0;
	return (ret);
}

float	ref_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas, int light)
{
	t_vec3	l_vec;
	t_vec3	hit;
	t_vec3	reflect;
	t_vec3	norm;
	float	ret;

	norm = check_plane_direction(pl, ray);
	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	l_vec = norm_vec(sub_vector(canvas.obj->l[light].light_orig, hit));
	reflect = norm_vec(reflection(norm, l_vec));
	ret = scalar_product(norm_vec(sub_vector(ray->origin, hit)), reflect);
	if (ret < 0.0)
		ret = 0.0;
	return (ret);
}

float	ref_cy(t_cylinder *cy, t_ray3 *ray, t_canvas canvas, int light)
{
	t_vec3	l_vec;
	t_vec3	hit;
	t_vec3	reflect;
	t_vec3	normal;
	float	ret;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	l_vec = norm_vec(sub_vector(canvas.obj->l[light].light_orig, hit));
	normal = sub_vector(hit, add_vector(cy->center, \
		multiple_vector(get_hit_height(cy, hit), cy->dir)));
	if (cy->type == TCY && cy->bumppath != NULL)
		normal = bump_cylinder(cy, cy->bumtexture, hit, normal);
	reflect = norm_vec(reflection(normal, l_vec));
	ret = scalar_product(norm_vec(sub_vector(ray->origin, hit)), reflect);
	if (ret < 0.0)
		ret = 0.0;
	return (ret);
}
