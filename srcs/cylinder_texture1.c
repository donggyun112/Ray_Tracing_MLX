/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_texture1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 08:04:45 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 08:17:31 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color	get_checker_pattern(t_vec3 p, t_cylinder *cy)
{
	float	u;
	float	v;
	int		u_checker;
	int		v_checker;

	cylindrical_map(p, &u, &v, cy);
	u_checker = (int)(u * 32);
	v_checker = (int)(v * 16);
	if ((u_checker + v_checker) % 2 == 0)
		return ((t_color){0, 0, 0});
	else
		return ((t_color){255, 255, 255});
}

t_color	image_texture_on_cylinder(t_vec3 point, t_cylinder *cy, t_texture *tex)
{
	float	u;
	float	v;

	cylindrical_map(point, &u, &v, cy);
	return (get_texture_color(*tex, u, v));
}

void	cap_texture(t_vec3 point, t_cylinder *cy, t_plane *cap, t_ray3 *ray)
{
	t_color	c;

	if (cy->type == CCY)
		c = checkertexture(point, 5.0, cap);
	else
		c = get_texture_color(cy->texture, fabs(point.x), fabs(point.z));
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
}

void	cylinder_texture(t_ray3 *ray, t_cylinder *cy, float tmp)
{
	t_vec3	hit;
	t_color	c;

	ray->obj = (void *)cy;
	ray->t = tmp;
	hit = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	if (cy->type == CCY)
		c = get_checker_pattern(hit, cy);
	else
		c = image_texture_on_cylinder(hit, cy, &cy->texture);
	ray->color[RED] = c.r;
	ray->color[GREEN] = c.g;
	ray->color[BLUE] = c.b;
	ray->type = CY;
}
