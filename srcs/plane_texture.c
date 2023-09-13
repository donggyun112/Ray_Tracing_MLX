/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 07:37:59 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 08:17:34 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	get_plane_uv(t_vec3 point, t_plane *pl, float scale, float *uv)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	t_vec3	relative_pos;

	relative_pos = sub_vector(point, pl->on_plane);
	u_axis = get_u_v_axis(pl->norm);
	v_axis = vector_product(pl->norm, u_axis);
	uv[0] = scalar_product(relative_pos, u_axis) * scale;
	uv[1] = scalar_product(relative_pos, v_axis) * scale;
}

void	init_pl_color(t_ray3 *ray, t_plane *pl)
{
	ray->color[RED] = pl->color[RED];
	ray->color[GREEN] = pl->color[GREEN];
	ray->color[BLUE] = pl->color[BLUE];
}

void	init_pltexture(t_ray3 *ray, t_plane *pl)
{
	t_color	c;
	t_vec3	hit;
	float	uv[2];

	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (pl->type == TPL)
	{
		get_plane_uv(hit, pl, 0.02, uv);
		uv[0] = uv[0] - floorf(uv[0]);
		uv[1] = uv[1] - floorf(uv[1]);
		c = get_texture_color(pl->texture, uv[0], uv[1]);
	}
	else
		c = checkertexture(hit, 1, pl);
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
}
