/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 00:50:05 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/06 03:15:41 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	init_sphere_color(t_ray3 *ray, t_sphere *sp, float tmp)
{
	ray->t = tmp;
	ray->color[RED] = sp->color[RED];
	ray->color[GREEN] = sp->color[GREEN];
	ray->color[BLUE] = sp->color[BLUE];
	ray->type = SP;
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
		init_sphere_color(ray, sp, tmp);
		ray->obj = (void *)sp;
		if (hit_shadow(ray, canvas))
			ray->type = SHADOW;
	}
}
