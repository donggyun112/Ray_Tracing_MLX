/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 07:30:41 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 07:40:46 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color	uv_grid_pattern_at(t_checker pattern, float u, float v)
{
	const int	u2 = (int)(u * pattern.width);
	const int	v2 = (int)(v * pattern.height);

	if ((u2 + v2) % 2)
		return (pattern.color_a);
	else
		return (pattern.color_b);
}

void	spherical_map(t_vec3 p, float *u, float *v, t_sphere *sp)
{
	float	theta;
	float	vecmagnitude;
	float	phi;
	float	raw_u;
	t_vec3	relative_point;

	relative_point = \
	(t_vec3){p.x - sp->center.x, p.y - sp->center.y, p.z - sp->center.z};
	theta = atan2f(relative_point.z, relative_point.x);
	vecmagnitude = sqrtf(relative_point.x * relative_point.x + \
	relative_point.y * relative_point.y + relative_point.z * relative_point.z);
	phi = acosf(relative_point.y / vecmagnitude);
	theta += sp->angle;
	raw_u = theta / (2.0f * M_PI);
	*u = (raw_u + 0.5f);
	*v = phi / M_PI;
}

t_color	image_texture_on_sphere(t_vec3 point, t_sphere *sp, t_texture *texture)
{
	float	u;
	float	v;

	spherical_map(point, &u, &v, sp);
	return (get_texture_color(*texture, u, v));
}

t_color	grid_texture_on_sphere(t_vec3 point, t_checker pattern, t_sphere *sp)
{
	float	u;
	float	v;

	spherical_map(point, &u, &v, sp);
	return (uv_grid_pattern_at(pattern, u, v));
}
