/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 08:01:55 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 08:24:46 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	cy_in_range(t_ray3 *ray, float t, t_cylinder *cy)
{
	t_vec3		hit;
	const float	condition = cy->height / 2;
	float		height[2];

	hit = add_vector(ray->origin, multiple_vector(t, ray->dir));
	height[0] = scalar_product(sub_vector(hit, cy->center), cy->dir);
	height[1] = scalar_product(sub_vector(hit, cy->center), \
		multiple_vector(-1.0, cy->dir));
	if (height[0] > 0 && height[0] > condition)
		return (0);
	if (height[1] > 0 && height[1] > condition)
		return (0);
	return (1);
}

void	init_cap_color(t_ray3 *ray, t_plane *cap)
{
	ray->color[RED] = cap->color[RED];
	ray->color[GREEN] = cap->color[GREEN];
	ray->color[BLUE] = cap->color[BLUE];
}

void	init_cy_color(t_ray3 *ray, t_cylinder *cy, float tmp)
{
	ray->t = tmp;
	ray->color[RED] = cy->color[RED];
	ray->color[GREEN] = cy->color[GREEN];
	ray->color[BLUE] = cy->color[BLUE];
	ray->type = CY;
	ray->obj = (void *)cy;
}

void	hit_cap(t_ray3 *ray, t_cylinder *cy, t_plane *cap)
{
	float	tmp;
	float	scalar[3];
	t_vec3	hit;

	scalar[0] = scalar_product(cap->on_plane, cap->norm);
	scalar[1] = scalar_product(ray->origin, cap->norm);
	scalar[2] = scalar_product(ray->dir, cap->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	hit = add_vector(ray->origin, multiple_vector(tmp, ray->dir));
	if (size_of_vec(sub_vector(hit, cap->on_plane)) > cy->radius)
		return ;
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->type = CAP;
		ray->obj = (void *)cap;
		if (cy->type == CCY || cy->type == TCY)
			cap_texture(hit, cy, cap, ray);
		else
			init_cap_color(ray, cap);
	}
}

void	hit_cylinder(t_ray3 *ray, t_cylinder *cy)
{
	const t_vec3	oc = sub_vector(ray->origin, cy->center);
	t_vec3			v[2];
	float			coef[3];
	float			tmp;

	hit_cap(ray, cy, cy->ucap);
	hit_cap(ray, cy, cy->lcap);
	v[0] = vector_product(ray->dir, cy->dir);
	v[1] = vector_product(oc, cy->dir);
	coef[0] = scalar_product(v[0], v[0]);
	coef[1] = 2 * scalar_product(v[0], v[1]);
	coef[2] = scalar_product(v[1], v[1]) - cy->radius * cy->radius;
	if (discriminant(coef[0], coef[1], coef[2]))
		tmp = quad_formula(coef[0], coef[1], coef[2]);
	else
		tmp = -1.0;
	if (tmp >= 0.0 && cy_in_range(ray, tmp, cy) == 0)
		return ;
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		if (cy->type == CCY || cy->type == TCY)
			cylinder_texture(ray, cy, tmp);
		else
			init_cy_color(ray, cy, tmp);
	}
}
