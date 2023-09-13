/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_texture2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 20:29:45 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 08:56:30 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3	main_axis(t_vec3 dir)
{
	t_vec3	abs_dir;

	abs_dir.x = fabsf(dir.x);
	abs_dir.y = fabsf(dir.y);
	abs_dir.z = fabsf(dir.z);
	if (abs_dir.x > abs_dir.y && abs_dir.x > abs_dir.z)
		return ((t_vec3){1, 0, 0});
	if (abs_dir.y > abs_dir.x && abs_dir.y > abs_dir.z)
		return ((t_vec3){0, 1, 0});
	return ((t_vec3){0, 0, 1});
}

void	cylindrical_map(t_vec3 p, float *u, float *v, t_cylinder *cy)
{
	const t_vec3	relative_point = sub_vector(p, cy->center);
	float			projected_y;
	float			theta;
	const t_vec3	proj = sub_vector(relative_point, \
	multiple_vector(scalar_product(relative_point, cy->dir), cy->dir));
	const t_vec3	axis = main_axis(cy->dir);

	if (axis.x == 1)
		theta = atan2f(proj.z, proj.y);
	else if (axis.y == 1)
		theta = atan2f(proj.z, proj.x);
	else
		theta = atan2f(proj.y, proj.x);
	theta += cy->angle;
	theta = fmod(theta + 2.0f * M_PI, 2.0f * M_PI);
	*u = (theta + M_PI) / (2.0f * M_PI);
	projected_y = scalar_product(relative_point, cy->dir);
	*v = (projected_y + cy->height / 2) / cy->height;
}
