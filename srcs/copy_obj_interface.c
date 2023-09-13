/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_obj_interface.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:17:56 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 01:21:26 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3	init_copy_vec(t_vec3 *vec, t_vec3 tar)
{
	vec->x = tar.x + 1;
	vec->y = tar.y + 1;
	vec->z = tar.z + 1;
	return (*vec);
}

float	get_rand(float a, float b)
{
	return (a + (b - a) * (float)rand() / (float)RAND_MAX);
}

void	rand_vec(t_vec3 *vec, float max)
{
	vec->x = get_rand(0.0f, max);
	vec->y = get_rand(0.0f, max);
	vec->z = get_rand(0.0f, max);
	*vec = norm_vec(*vec);
}

void	obj_copy(t_view *view, int keycode)
{
	if (view->grep.type == SP)
	{
		if (keycode == F)
			push_obj((t_sphere *)view->grep.obj, NULL, NULL, view);
		else
			copy_sphere(view, keycode);
	}
	else if (view->grep.type == CY)
	{
		if (keycode == F)
			push_obj(NULL, (t_cylinder *)view->grep.obj, NULL, view);
		else
			copy_cylinder(view, keycode);
	}
	else if (view->grep.type == PL)
	{
		if (keycode == F)
			push_obj(NULL, NULL, (t_plane *)view->grep.obj, view);
		else
			copy_plane(view, keycode);
	}
	view->grep.grep = OFF;
	newwin(view);
}
