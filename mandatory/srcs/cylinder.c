/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 21:39:34 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 08:23:45 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	make_cylinder_cap(t_cylinder *cy)
{
	int	idx;

	idx = 0;
	cy->ucap = (t_plane *)malloc(sizeof(t_plane));
	cy->lcap = (t_plane *)malloc(sizeof(t_plane));
	while (idx < 3)
	{
		cy->ucap->color[idx] = cy->color[idx];
		cy->lcap->color[idx] = cy->color[idx];
		idx++;
	}
	cy->ucap->norm = cy->dir;
	cy->lcap->norm = multiple_vector(-1.0, cy->dir);
	cy->ucap->on_plane = add_vector(cy->center, \
		multiple_vector(cy->height / 2.0, cy->dir));
	cy->lcap->on_plane = add_vector(cy->center, \
		multiple_vector(cy->height / -2.0, cy->dir));
}

int	cy_in_range(t_ray3 *ray, double t, t_cylinder *cy)
{
	t_vec3	hit;
	double	height[2];

	hit = add_vector(ray->origin, multiple_vector(t, ray->dir));
	height[0] = scalar_product(sub_vector(hit, cy->center), cy->dir);
	height[1] = scalar_product(sub_vector(hit, cy->center), \
		multiple_vector(-1.0, cy->dir));
	if (height[0] > 0 && height[0] > cy->height / 2)
		return (0);
	if (height[1] > 0 && height[1] > cy->height / 2)
		return (0);
	return (1);
}

void	hit_cap(t_ray3 *ray, t_cylinder *cy, t_plane *cap, t_canvas canvas)
{
	double	tmp;
	double	scalar[3];
	t_vec3	hit;

	scalar[0] = scalar_product(cap->on_plane, cap->norm);
	scalar[1] = scalar_product(ray->origin, cap->norm);
	scalar[2] = scalar_product(ray->dir, cap->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	hit = add_vector(ray->origin, multiple_vector(tmp, ray->dir));
	if (size_of_vec2(sub_vector(hit, cap->on_plane)) > cy->radius)
		return ;
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->type = PL;
		ray->color[RED] = cap->color[RED];
		ray->color[GREEN] = cap->color[GREEN];
		ray->color[BLUE] = cap->color[BLUE];
		ray->obj = (void *)cap;
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}

void	init_cylinder_color(t_ray3 *ray, t_cylinder *cy, float tmp)
{
	ray->t = tmp;
	ray->color[RED] = cy->color[RED];
	ray->color[GREEN] = cy->color[GREEN];
	ray->color[BLUE] = cy->color[BLUE];
	ray->type = CY;
	ray->obj = (void *)cy;
}

void	hit_cylinder(t_ray3 *ray, t_cylinder *cy, t_canvas canvas)
{
	t_vec3	oc;
	t_vec3	v[2];
	double	coef[3];
	double	tmp;

	hit_cap(ray, cy, cy->ucap, canvas);
	hit_cap(ray, cy, cy->lcap, canvas);
	oc = sub_vector(ray->origin, cy->center);
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
		init_cylinder_color(ray, cy, tmp);
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}
