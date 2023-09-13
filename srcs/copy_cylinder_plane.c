/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_cylinder_plane.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 01:21:50 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/09/14 08:30:43 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	copy_normal_plane(t_view *view, size_t t, int keycode)
{
	if (keycode == Z)
	{
		view->can.obj->pl[t].type = PL;
		view->can.obj->pl[t].color[RED] = rand() % 255;
		view->can.obj->pl[t].color[GREEN] = rand() % 255;
		view->can.obj->pl[t].color[BLUE] = rand() % 255;
	}
}

void	copy_plane(t_view *view, int keycode)
{
	t_plane			pl;
	const size_t	t = view->can.obj->pl_cnt;

	pl = *(t_plane *)view->grep.obj;
	view->can.obj->pl = \
	(t_plane *)realloc(view->can.obj->pl, sizeof(t_plane) * (t + 1));
	memcpy(&view->can.obj->pl[t], &pl, sizeof(pl));
	copy_normal_plane(view, t, keycode);
	view->can.obj->pl_cnt++;
}

void	copy_normal_cylinder(t_view *view, size_t t, int keycode)
{
	if (keycode == Z)
	{
		view->can.obj->cy[t].color[RED] = rand() % 255;
		view->can.obj->cy[t].color[GREEN] = rand() % 255;
		view->can.obj->cy[t].color[BLUE] = rand() % 255;
	}
}

void	copy_cylinder(t_view *view, int keycode)
{
	t_cylinder		cy;
	const size_t	t = view->can.obj->cy_cnt;

	cy = *(t_cylinder *)view->grep.obj;
	view->can.obj->cy = \
	(t_cylinder *)reallocf(view->can.obj->cy, sizeof(t_cylinder) * (t + 1));
	memcpy(&view->can.obj->cy[t], &cy, sizeof(cy));
	view->can.obj->cy[t].center.x += 1;
	view->can.obj->cy[t].center.y += 1;
	view->can.obj->cy[t].center.z += 1;
	if (keycode == Z)
	{
		view->can.obj->cy[t].height = get_rand(0.5, 3);
		view->can.obj->cy[t].radius = get_rand(0.5, 3);
		view->can.obj->cy[t].dir.x = get_rand(-1.0f, 1.0f);
		view->can.obj->cy[t].dir.y = get_rand(-1.0f, 1.0f);
		view->can.obj->cy[t].dir.z = get_rand(-1.0f, 1.0f);
		view->can.obj->cy[t].dir = norm_vec(view->can.obj->cy[t].dir);
		view->can.obj->cy[t].type = CY;
	}
	if (view->can.obj->cy[t].type == CY)
		copy_normal_cylinder(view, t, keycode);
	make_cylinder_cap(&view->can.obj->cy[t]);
	view->can.obj->cy_cnt++;
}
