/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 06:40:53 by jinhyeop          #+#    #+#             */
/*   Updated: 2024/03/11 02:05:30 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	sphere_texture(t_ray3 *ray, t_sphere *sp)
{
	t_color			c;
	const t_checker	pattern = {{255, 255, 255}, {100, 100, 0}, 32, 16};
	t_vec3			hit;

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (sp->type == TSP)
		c = image_texture_on_sphere(hit, sp, &sp->texture);
	else
		c = grid_texture_on_sphere(hit, pattern, sp);
	ray->type = SP;
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
}

void	init_sp_color(t_ray3 *ray, t_sphere *sp)
{
	ray->type = SP;
	ray->color[RED] = sp->color[RED];
	ray->color[GREEN] = sp->color[GREEN];
	ray->color[BLUE] = sp->color[BLUE];
}

void	swap(float *t0, float *t1)
{
	float	tmp;

	tmp = *t0;
	*t0 = *t1;
	*t1 = tmp;
}

float	get_t(float tca, float d2, float tnc, float radius)
{
	float	t0;
	float	t1;

	if (d2 > radius * radius)
		return (-1.0);
	if (tca < 0)
	{
		t0 = tca + tnc;
		t1 = tca - tnc;
	}
	else
	{
		t0 = tca - tnc;
		t1 = tca + tnc;
	}
	if (t0 > t1)
		swap(&t0, &t1);
	if (t0 < 0)
	{
		t0 = t1;
		if (t0 < 0)
			return (-1.0);
	}
	return (t0);
}

void	hit_sphere(t_ray3 *ray, t_sphere *sp)
{
	const t_vec3 l = sub_vector(sp->center, ray->origin);
	const float tca = scalar_product(l, ray->dir);
	const float d2 = scalar_product(l, l) - (tca * tca);
	const float tnc = sqrt(sp->radius * sp->radius - d2);
	float t0;

	t0 = get_t(tca, d2, tnc, sp->radius);
	if (t0 < 0)
		return ;
	if ((ray->t < 0.0 && t0 > 0.0) || (t0 > 0.0 && ray->t > t0)) {
		ray->t = t0;
		ray->obj = (void *)sp;
		if (sp->type == TSP || sp->type == CSP)
			sphere_texture(ray, sp);
		else
			init_sp_color(ray, sp);
	}
}
