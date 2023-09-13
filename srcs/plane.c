/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 07:35:38 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 08:25:13 by jinhyeop         ###   ########.fr       */
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

int	in_triangle(t_vec3 hit, t_plane *pl)
{
	t_vec3	pos;
	t_vec3	v1_hit;
	t_vec3	v2_hit;
	float	size;

	pos = sub_vector(hit, pl->on_plane);
	v1_hit = vector_product(pl->tr_v1, pos);
	v2_hit = vector_product(pos, pl->tr_v2);
	if (!(scalar_product(pl->norm, v1_hit) > 0.0
			&& scalar_product(pl->norm, v2_hit) > 0.0))
		return (0);
	size = size_of_vec(v1_hit) + size_of_vec(v2_hit);
	if (size > size_of_vec(vector_product(pl->tr_v1, pl->tr_v2)))
		return (0);
	return (1);
}

void	hit_triangle(t_ray3 *ray, t_plane *pl)
{
	float	tmp;
	float	scalar[3];
	t_vec3	hit;

	scalar[0] = scalar_product(pl->on_plane, pl->norm);
	scalar[1] = scalar_product(ray->origin, pl->norm);
	scalar[2] = scalar_product(ray->dir, pl->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		hit = add_vector(ray->origin, multiple_vector(tmp, ray->dir));
		if (in_triangle(hit, pl) == 0)
			return ;
		ray->t = tmp;
		ray->obj = (void *)pl;
		ray->type = TRI;
		init_pl_color(ray, pl);
	}
}

void	hit_plane(t_ray3 *ray, t_plane *pl)
{
	float	tmp;
	float	scalar[3];

	if (pl->type == TRI)
	{
		hit_triangle(ray, pl);
		return ;
	}
	scalar[0] = scalar_product(pl->on_plane, pl->norm);
	scalar[1] = scalar_product(ray->origin, pl->norm);
	scalar[2] = scalar_product(ray->dir, pl->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->obj = (void *)pl;
		ray->type = PL;
		if (pl->type == TPL || pl->type == CPL)
			init_pltexture(ray, pl);
		else
			init_pl_color(ray, pl);
	}
}
