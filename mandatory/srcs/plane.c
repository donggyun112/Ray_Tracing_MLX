/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 20:45:44 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/06 01:16:57 by seodong-gyu      ###   ########.fr       */
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
