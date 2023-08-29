/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 21:39:34 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/29 20:48:40 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3	check_plane_direction(t_plane *pl, t_ray3 *ray)
{
	t_vec3	orig_to_pl;

	orig_to_pl = sub_vector(ray->origin, pl->on_plane);
	if (scalar_product(orig_to_pl, pl->norm) < 0.0)
		return (multiple_vector(-1.0, pl->norm));
	else
		return (pl->norm);
}

void	hit_plane(t_ray3 *ray, t_plane *pl, t_canvas canvas)
{
	double	tmp;
	double	scalar[3];

	pl->norm = check_plane_direction(pl, ray);
	scalar[0] = scalar_product(pl->on_plane, pl->norm);
	scalar[1] = scalar_product(ray->origin, pl->norm);
	scalar[2] = scalar_product(ray->dir, pl->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->type = PL;
		ray->color[RED] = pl->color[RED];
		ray->color[GREEN] = pl->color[GREEN];
		ray->color[BLUE] = pl->color[BLUE];
		ray->obj = (void *)pl;
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}

void	hit_sphere(t_ray3 *ray, t_sphere *sp, t_canvas canvas)
{
	t_vec3	l;
	double	tca;
	double	tnc;
	double	d2;
	double	tmp;

	l = sub_vector(sp->center, ray->origin);
	tca = scalar_product(l, ray->dir);
	if (tca < 0)
		return ;
	d2 = scalar_product(l, l) - (tca * tca);
	if (d2 > sp->radius * sp->radius)
		return ;
	tnc = sqrt(sp->radius * sp->radius - d2);
	if (tca - tnc < 0.0)
		tmp = tca + tnc;
	else
		tmp = tca - tnc;
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->color[RED] = sp->color[RED];
		ray->color[GREEN] = sp->color[GREEN];
		ray->color[BLUE] = sp->color[BLUE];
		ray->type = SP;
		ray->obj = (void *)sp;
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}

int	discriminant(double a, double b, double c)
{
	if ((b * b) - (4 * a * c) >= 0.0)
		return (1);
	else
		return (0);
}

double	quad_formula(double a, double b, double c)
{
	double	sol1;
	double	sol2;

	sol1 = ((-1) * b - sqrt(b * b - (4 * a * c))) / (2.0 * a);
	sol2 = ((-1) * b + sqrt(b * b - (4 * a * c))) / (2.0 * a);
	if (sol1 > 0.0 && sol2 > 0.0)
		return (sol1);
	else if (sol1 < 0.0 && sol2 > 0.0)
		return (sol2);
	else
		return (-1.0);
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

void	hit_cylinder(t_ray3 *ray, t_cylinder *cy, t_canvas canvas)
{
	t_vec3	oc;
	t_vec3	v[2];
	double	coef[3];
	double	tmp;

	make_cylinder_cap(cy);
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
		ray->t = tmp;
		ray->color[RED] = cy->color[RED];
		ray->color[GREEN] = cy->color[GREEN];
		ray->color[BLUE] = cy->color[BLUE];
		ray->type = CY;
		ray->obj = (void *)cy;
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}
