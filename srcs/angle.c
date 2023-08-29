/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 14:30:31 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/29 20:13:37 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3 apply_bump_on_sphere(t_sphere *sphere, t_texture bumpmap, t_vec3 hit)
{
    float u, v;

    spherical_map(hit, &u, &v, sphere->center);

    Color c = get_texture_color(bumpmap, u, v); // 앞서 제공한 texture_at 함수 사용

 	double scale_factor = 8.0;
	t_vec3 bump_normal = {
	    scale_factor * ((c.r / 255.0) - 0.5), 
	    scale_factor * ((c.g / 255.0) - 0.5), 
	    scale_factor * ((c.b / 255.0) - 0.5)
	};
    t_vec3 normal = sub_vector(hit, sphere->center);
    normal = norm_vec(normal); 

    t_vec3 perturbed_normal = add_vector(normal, bump_normal);
    perturbed_normal = norm_vec(perturbed_normal);
	return (perturbed_normal);
}

double	cos_sp(t_sphere *sp, t_ray3 *ray, t_canvas canvas)
{
	t_vec3	normal;
	t_vec3	light;
	t_vec3	hit;
	double	angle;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (sp->type == TSP)
	{
		normal = apply_bump_on_sphere(sp, sp->bumtexture, hit);
		printf("bum\n");
	}
	else
		normal = norm_vec(sub_vector(hit, sp->center));
	light = norm_vec(sub_vector(canvas.obj->l[0].light_orig, hit));
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
	light = norm_vec(sub_vector(canvas.obj->l[0].light_orig, hit));
	angle = scalar_product(pl->norm, light);
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
	light = norm_vec(sub_vector(canvas.obj->l[0].light_orig, hit));
	normal = norm_vec(sub_vector(hit, sp->center));
	reflect = norm_vec(reflection(normal, light));
	ret = scalar_product(norm_vec(sub_vector(ray->origin, hit)), norm_vec(reflect));
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
	light = norm_vec(sub_vector(canvas.obj->l[0].light_orig, hit));
	reflect = norm_vec(reflection(pl->norm, light));
	ret = scalar_product(norm_vec(sub_vector(ray->origin, hit)), reflect);
	if (ret < 0.0)
		ret = 0.0;
	return (ret);
}
