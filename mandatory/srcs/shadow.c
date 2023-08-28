/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 17:48:21 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/28 19:27:41 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	shadow_sphere(t_ray3 *ray, t_sphere *sp)
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
		ray->t = tmp;
}

void	shadow_plane(t_ray3 *ray, t_plane *pl)
{
	double	tmp;
	double	scalar[3];

	pl->norm = check_plane_direction(pl, ray);
	scalar[0] = scalar_product(pl->on_plane, pl->norm);
	scalar[1] = scalar_product(ray->origin, pl->norm);
	scalar[2] = scalar_product(ray->dir, pl->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
		ray->t = tmp;
}

void	shadow_cap(t_ray3 *ray, t_cylinder *cy, t_plane *cap)
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
		ray->t = tmp;
}

void	shadow_cylinder(t_ray3 *ray, t_cylinder *cy)
{
	t_vec3	oc;
	t_vec3	v[2];
	double	coef[3];
	double	tmp;

	make_cylinder_cap(cy);
	shadow_cap(ray, cy, cy->ucap);
	shadow_cap(ray, cy, cy->lcap);
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
		ray->t = tmp;
}

void	shadow_check(t_ray3 *ray, t_canvas canvas)
{
	int	idx;

	idx = 0;
	while (idx < canvas.obj->sp_cnt)
		shadow_sphere(ray, &canvas.obj->sp[idx++]);
	idx = 0;
	while (idx < canvas.obj->pl_cnt)
		shadow_plane(ray, &canvas.obj->pl[idx++]);
	idx = 0;
	while (idx < canvas.obj->cy_cnt)
		shadow_cylinder(ray, &canvas.obj->cy[idx++]);
}

int	hit_shadow(t_ray3 *ray, t_canvas canvas)
{
	t_ray3	hit;
	t_vec3	to_light;
	double	range;

	hit.origin = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	to_light = sub_vector(canvas.light_orig, hit.origin);
	hit.dir = norm_vec(to_light);
	hit.origin = add_vector(hit.origin, multiple_vector(0.01, hit.dir));
	range = size_of_vec2(to_light);
	hit.t = -1.0;
	hit.type = 0;
	hit.obj = NULL;
	hit.color[RED] = 0;
	hit.color[GREEN] = 0;
	hit.color[BLUE] = 0;
	shadow_check(&hit, canvas);
	if (hit.t > 0.0 && hit.t < range)
		return (1);
	else
		return (0);
}
