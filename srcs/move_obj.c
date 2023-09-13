/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 22:23:30 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/13 22:23:43 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	move_obj_vec(t_vec3 *vec, t_vec3 campos, int keycode)
{
	if (keycode == A || keycode == S)
		*vec = sub_vector(*vec, multiple_vector(0.5, campos));
	else if (keycode == D || keycode == W)
		*vec = add_vector(*vec, multiple_vector(0.5, campos));
}

void	move_sphere(t_view *view, int keycode)
{
	t_sphere	*sp;

	sp = (t_sphere *)view->grep.obj;
	if (keycode == A || keycode == D)
		move_obj_vec(&sp->center, view->cam.r_norm, keycode);
	else
		move_obj_vec(&sp->center, view->cam.dir, keycode);
}

void	move_obj(int keycode, t_view *view)
{
	t_cylinder	*cy;
	t_plane		*pl;

	if (view->grep.type == SP)
		move_sphere(view, keycode);
	else if (view->grep.type == CY)
	{
		cy = (t_cylinder *)view->grep.obj;
		if (keycode == A || keycode == D)
			move_obj_vec(&cy->center, view->cam.r_norm, keycode);
		else
			move_obj_vec(&cy->center, view->cam.dir, keycode);
		make_cylinder_cap2(cy);
	}
	else if (view->grep.type == PL)
	{
		pl = (t_plane *)view->grep.obj;
		if (keycode == A || keycode == D)
			move_obj_vec(&pl->on_plane, view->cam.r_norm, keycode);
		else
			move_obj_vec(&pl->on_plane, view->cam.dir, keycode);
	}
}
