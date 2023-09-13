/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bumpmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 06:21:59 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 06:26:14 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3	get_tangent_bump(t_vec3 normal, t_color c)
{
	t_vec3	bump_normal;
	t_vec3	u;
	t_vec3	v;
	t_vec3	tn[3];

	u = norm_vec(vector_product(normal, vec3(0.0, 0.0, 1.0)));
	v = vector_product(u, normal);
	tn[RED] = multiple_vector((c.r / 255.0) * 2.0 - 1.0, u);
	tn[GREEN] = multiple_vector((c.g / 255.0) * 2.0 - 1.0, v);
	tn[BLUE] = multiple_vector((c.b / 255.0) * 2.0 - 1.0, normal);
	bump_normal = add_three_vector(tn[RED], tn[GREEN], tn[BLUE]);
	return (bump_normal);
}

t_vec3	bump_sphere(t_sphere *sphere, t_texture bp, t_vec3 hit)
{
	float	u;
	float	v;
	t_color	c;
	t_vec3	bump_normal;
	t_vec3	normal;

	spherical_map(hit, &u, &v, sphere);
	c = get_texture_color(bp, u, v);
	normal = norm_vec(sub_vector(hit, sphere->center));
	bump_normal = get_tangent_bump(normal, c);
	normal = norm_vec(add_vector(normal, bump_normal));
	return (normal);
}

t_vec3	bump_cylinder(t_cylinder *cy, t_texture bp, t_vec3 hit, t_vec3 normal)
{
	float	u;
	float	v;
	t_color	c;
	t_vec3	bump_normal;
	t_vec3	new_normal;

	cylindrical_map(hit, &u, &v, cy);
	c = get_texture_color(bp, u, v);
	bump_normal = get_tangent_bump(normal, c);
	new_normal = norm_vec(add_vector(normal, bump_normal));
	return (new_normal);
}
