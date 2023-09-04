/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 14:30:31 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/04 21:09:01 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// t_vec3	bump_sphere(t_sphere *sphere, t_texture bp, t_vec3 hit)
// {
// 	float	u;
// 	float	v;
// 	t_color	c;
// 	t_vec3	bump_normal;
// 	t_vec3	normal;

// 	spherical_map(hit, &u, &v, sphere->center, sphere->angle);
// 	c = get_texture_color(bp, u, v);
// 	bump_normal = (t_vec3)
// 	{
// 		2.0 * (c.r / 255.0) - 0.5,
// 		2.0 * (c.g / 255.0) - 0.5,
// 		2.0 * (c.b / 255.0) - 0.5
// 	};
// 	normal = sub_vector(hit, sphere->center);
// 	normal = norm_vec(normal);
// 	normal = add_vector(normal, bump_normal);
// 	normal = norm_vec(normal);
// 	return (normal);
// }

t_vec3  bump_sphere(t_sphere *sphere, t_texture bp, t_vec3 hit)
{
    float   u;
    float   v;
    t_color c;
    t_vec3  bump_normal;
    t_vec3  normal, tangent, bitangent, bumped_normal;
    spherical_map(hit, &u, &v, sphere->center, sphere->angle);
    c = get_texture_color(bp, u, v);
    bump_normal = (t_vec3){
        (c.r / 127.5) - 1.0,
        (c.g / 127.5) - 1.0,
        (c.b / 127.5) - 1.0
    };
    normal = sub_vector(hit, sphere->center);
    normal = norm_vec(normal);
    tangent = norm_vec(vector_product(normal, (t_vec3){0, 1, 0}));
    bitangent = norm_vec(vector_product(normal, tangent));
    bumped_normal.x = tangent.x * bump_normal.x + bitangent.x * bump_normal.y + normal.x * bump_normal.z;
    bumped_normal.y = tangent.y * bump_normal.x + bitangent.y * bump_normal.y + normal.y * bump_normal.z;
    bumped_normal.z = tangent.z * bump_normal.x + bitangent.z * bump_normal.y + normal.z * bump_normal.z;
    bumped_normal = norm_vec(bumped_normal);
    return (bumped_normal);
}


float	cos_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	normal;
	t_vec3	light;
	t_vec3	hit;
	float	angle;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (sp->type == TSP && sp->bumppath != NULL)
		normal = bump_sphere(sp, sp->bumtexture, hit);
	else
		normal = norm_vec(sub_vector(hit, sp->center));
	light = norm_vec(sub_vector(canvas.obj->l[0].light_orig, hit));
	angle = scalar_product(normal, light);
	if (angle < 0.0)
		return (0.0);
	return (angle);
}

float	cos_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	hit;
	t_vec3	light;
	t_vec3	norm;
	float	angle;

	norm = check_plane_direction(pl, ray);
	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	light = norm_vec(sub_vector(canvas.obj->l[0].light_orig, hit));
	angle = scalar_product(norm, light);
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
	dist = size_of_vec2(center_to_hit);
	if (dist < cy->radius)
		dist = cy->radius;
	hit_height = sqrt(pow(dist, 2) - pow(cy->radius, 2));
	if (scalar_product(center_to_hit, cy->dir) < 0.0)
		return (-1.0 * hit_height);
	else
		return (hit_height);
}

float	cos_cy(t_cylinder *cy, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	normal;
	t_vec3	light;
	t_vec3	hit;
	float	angle;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	normal = sub_vector(hit, add_vector(cy->center, \
		multiple_vector(get_hit_height(cy, hit), cy->dir)));
	normal = norm_vec(normal);
	light = norm_vec(sub_vector(canvas.obj->l[0].light_orig, hit));
	angle = scalar_product(normal, light);
	if (angle < 0.0)
		return (0.0);
	return (angle);
}

t_vec3	reflection(t_vec3 normal, t_vec3 light)
{
	t_vec3	ret;
	float	k;

	k = 2 * scalar_product(light, normal);
	ret = sub_vector(multiple_vector(k, normal), light);
	return (ret);
}

float	ref_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	normal;
	t_vec3	light;
	t_vec3	hit;
	t_vec3	reflect;
	float	ret;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	light = norm_vec(sub_vector(canvas.obj->l[0].light_orig, hit));
	if (sp->type == TSP && sp->bumppath != NULL)
		normal = bump_sphere(sp, sp->bumtexture, hit);
	else
		normal = norm_vec(sub_vector(hit, sp->center));
	reflect = norm_vec(reflection(normal, light));
	ret = scalar_product(norm_vec(\
	sub_vector(ray->origin, hit)), norm_vec(reflect));
	if (ret < 0.0)
		ret = 0.0;
	return (ret);
}

float	ref_pl(t_plane *pl, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	light;
	t_vec3	hit;
	t_vec3	reflect;
	t_vec3	norm;
	float	ret;

	norm = check_plane_direction(pl, ray);
	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	light = norm_vec(sub_vector(canvas.obj->l[0].light_orig, hit));
	reflect = norm_vec(reflection(norm, light));
	ret = scalar_product(norm_vec(sub_vector(ray->origin, hit)), reflect);
	if (ret < 0.0)
		ret = 0.0;
	return (ret);
}

float	ref_cy(t_cylinder *cy, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	light;
	t_vec3	hit;
	t_vec3	reflect;
	t_vec3	normal;
	float	ret;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	light = norm_vec(sub_vector(canvas.obj->l[0].light_orig, hit));
	normal = sub_vector(hit, add_vector(cy->center, \
		multiple_vector(get_hit_height(cy, hit), cy->dir)));
	normal = norm_vec(normal);
	reflect = norm_vec(reflection(normal, light));
	ret = scalar_product(norm_vec(sub_vector(ray->origin, hit)), reflect);
	if (ret < 0.0)
		ret = 0.0;
	return (ret);
}
