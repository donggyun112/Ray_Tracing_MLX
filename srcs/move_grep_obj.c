/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_grep_obj.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:30:33 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 01:47:17 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3	rotate_around_axis(t_vec3 vec, t_vec3 axis, float angle)
{
	const t_vec3	k = norm_vec(axis);
	const float		cos_angle = cos(angle);
	const float		sin_angle = sin(angle);
	t_vec3			rotated_vec;

	rotated_vec.x = vec.x * (cos_angle + (1 - cos_angle) * k.x * k.x) + \
					vec.y * ((1 - cos_angle) * k.x * k.y - sin_angle * k.z) + \
					vec.z * ((1 - cos_angle) * k.x * k.z + sin_angle * k.y);
	rotated_vec.y = vec.x * ((1 - cos_angle) * k.y * k.x + sin_angle * k.z) + \
					vec.y * (cos_angle + (1 - cos_angle) * k.y * k.y) + \
					vec.z * ((1 - cos_angle) * k.y * k.z - sin_angle * k.x);
	rotated_vec.z = vec.x * ((1 - cos_angle) * k.z * k.x - sin_angle * k.y) + \
					vec.y * ((1 - cos_angle) * k.z * k.y + sin_angle * k.x) + \
					vec.z * (cos_angle + (1 - cos_angle) * k.z * k.z);
	return (rotated_vec);
}

void	move_grep_obj_sphere(t_view *view, t_vec3 right, float pitch, float yaw)
{
	t_sphere	*sp;

	sp = (t_sphere *)view->grep.obj;
	sp->center = sub_vector(sp->center, view->cam.origin);
	sp->center = \
	rotate_around_axis(sp->center, (t_vec3){0.0f, 1.0f, 0.0f}, -yaw);
	sp->center = rotate_around_axis(sp->center, right, -pitch);
	sp->center = add_vector(sp->center, view->cam.origin);
}

void	move_grep_obj_cy(t_view *view, t_vec3 right, float pitch, float yaw)
{
	t_cylinder	*cy;

	cy = (t_cylinder *)view->grep.obj;
	if (view->change_dir)
	{
		cy->dir = \
		rotate_around_axis(cy->dir, (t_vec3){0.0f, 1.0f, 0.0f}, yaw);
		cy->dir = rotate_around_axis(cy->dir, right, pitch);
		norm_vec(cy->dir);
	}
	else
	{
		cy->center = sub_vector(cy->center, view->cam.origin);
		cy->center = \
		rotate_around_axis(cy->center, (t_vec3){0.0f, 1.0f, 0.0f}, -yaw);
		cy->center = rotate_around_axis(cy->center, right, -pitch);
		cy->center = add_vector(cy->center, view->cam.origin);
	}
	make_cylinder_cap2(cy);
}

void	move_grep_obj(t_view *view, t_vec3 right, float pitch, float yaw)
{
	t_plane		*pl;

	if (view->grep.type == SP)
		move_grep_obj_sphere(view, right, pitch, yaw);
	else if (view->grep.type == CY)
		move_grep_obj_cy(view, right, pitch, yaw);
	else if (view->grep.type == PL)
	{
		pl = (t_plane *)view->grep.obj;
		pl->on_plane = sub_vector(pl->on_plane, view->cam.origin);
		pl->on_plane = \
		rotate_around_axis(pl->on_plane, (t_vec3){0.0f, 1.0f, 0.0f}, yaw);
		pl->on_plane = rotate_around_axis(pl->on_plane, right, -pitch);
		pl->on_plane = add_vector(pl->on_plane, view->cam.origin);
	}
}
