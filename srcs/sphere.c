/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 06:40:53 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 07:33:53 by jinhyeop         ###   ########.fr       */
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

void	hit_sphere(t_ray3 *ray, t_sphere *sp)
{
	const t_vec3	l = sub_vector(sp->center, ray->origin);
	const float		tca = scalar_product(l, ray->dir);
	const float		d2 = scalar_product(l, l) - (tca * tca);
	const float		tnc = sqrt(sp->radius * sp->radius - d2);
	float			tmp;

	if (tca < 0)
		return ;
	if (d2 > sp->radius * sp->radius)
		return ;
	if (tca - tnc < 0.0)
		tmp = tca + tnc;
	else
		tmp = tca - tnc;
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->obj = (void *)sp;
		if (sp->type == TSP || sp->type == CSP)
			sphere_texture(ray, sp);
		else
			init_sp_color(ray, sp);
	}
}
