/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 17:48:21 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/09/13 08:25:34 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	shadow_check(t_ray3 *ray, t_canvas canvas)
{
	int	idx;

	idx = 0;
	while (idx < canvas.obj->sp_cnt)
	{
		if (canvas.obj->sp[idx].type != NONE)
			shadow_sphere(ray, &canvas.obj->sp[idx]);
		idx++;
	}
	idx = 0;
	while (idx < canvas.obj->pl_cnt)
	{
		if (canvas.obj->pl[idx].type != NONE)
			shadow_plane(ray, &canvas.obj->pl[idx]);
		idx++;
	}
	idx = 0;
	while (idx < canvas.obj->cy_cnt)
	{
		if (canvas.obj->cy[idx].type != NONE)
			shadow_cylinder(ray, &canvas.obj->cy[idx]);
		idx++;
	}
}

int	hit_shadow(t_ray3 *ray, t_canvas canvas, int light)
{
	t_ray3	hit;
	t_vec3	to_light;
	float	range;

	hit.origin = add_vector(ray->origin, multiple_vector(ray->t, ray->dir));
	to_light = sub_vector(canvas.obj->l[light].light_orig, hit.origin);
	hit.dir = norm_vec(to_light);
	hit.origin = add_vector(hit.origin, multiple_vector(0.1, hit.dir));
	range = size_of_vec(to_light);
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
